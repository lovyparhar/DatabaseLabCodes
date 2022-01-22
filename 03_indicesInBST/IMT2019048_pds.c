#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

#include "pds.h"
#include "bst.h"

struct PDS_RepoInfo repo_handle;


int pds_create(char *repo_name)
{

	// operning the file and handling file error
	char dtf[100];
	strcpy(dtf, repo_name);
	strcat(dtf, ".dat");

	FILE * fp = fopen(dtf, "w");
	if(fp == NULL) {
		return PDS_FILE_ERROR;
	}

	// closing the file and returning success
	fclose(fp);
	return PDS_SUCCESS;
}


int pds_open(char* repo_name, int rec_size)
{
	// checking if any file is already open
	if(repo_handle.repo_status == PDS_REPO_OPEN) {
		return PDS_REPO_ALREADY_OPEN;
	}

	// Open the data file rb+ mode and handle error
	char dtf[100];
	strcpy(dtf, repo_name);
	strcat(dtf, ".dat");

	repo_handle.pds_data_fp = fopen(dtf, "rb+");
	if(repo_handle.pds_data_fp == NULL) {
		return PDS_FILE_ERROR;
	}

	// Open index file in ab+ mode and handle error
	char ndf[100];
	strcpy(ndf, repo_name);
	strcat(ndf, ".ndx");

	repo_handle.pds_ndx_fp = fopen(ndf, "ab+");
	if(repo_handle.pds_ndx_fp == NULL) {
		return PDS_FILE_ERROR;
	}

	// If file opening successful, Update the fields of PDS_RepoInfo appropriately
	strcpy(repo_handle.pds_name, repo_name);
	repo_handle.rec_size = rec_size;
	repo_handle.repo_status = PDS_REPO_OPEN;
	repo_handle.pds_bst = NULL;


	// Load the index file entries into a bst
	if(pds_load_ndx()) {
		return PDS_LOAD_NDX_FAILED;
	}

	// Close only the index file
	fclose(repo_handle.pds_ndx_fp);

	return PDS_SUCCESS;
}






int pds_load_ndx()
{

	// Internal function used by pds_open to read index entries into BST
	fseek(repo_handle.pds_ndx_fp, 0, SEEK_SET);	
	int problem = 0;

	while(1) {

		struct PDS_NdxInfo * ndxinfo = (struct PDS_NdxInfo *)malloc(1 * sizeof(struct PDS_NdxInfo));
		int stat = fread(ndxinfo, sizeof(struct PDS_NdxInfo), 1, repo_handle.pds_ndx_fp);	

		if(stat != 1) {
			free(ndxinfo);
			break;
		}

		problem = bst_add_node(&(repo_handle.pds_bst), ndxinfo->key, ndxinfo);
		if(problem) break;
	}

	return problem;
}








int put_rec_by_key(int key, void*rec)
{

	// Check if some file is open or not
	if(repo_handle.repo_status != PDS_REPO_OPEN) {
		return PDS_FILE_ERROR;
	}

	// Check if the key is already there
	struct BST_Node * node = bst_search(repo_handle.pds_bst, key);
	if(node != NULL) {
		return PDS_ADD_FAILED;
	}


	// Seek to the end of the data file
	fseek(repo_handle.pds_data_fp, 0, SEEK_END);

	// Create an index entry with the current data file location using ftell
	struct PDS_NdxInfo * ndxinfo = (struct PDS_NdxInfo *)malloc(1 * sizeof(struct PDS_NdxInfo));
	ndxinfo->key = key;
	ndxinfo->offset = ftell(repo_handle.pds_data_fp);

	// Add index entry to BST using offset returned by ftell
	int problem = bst_add_node(&(repo_handle.pds_bst), ndxinfo->key, ndxinfo);
	if(problem) {
		return PDS_ADD_FAILED;
	}


	// Write the key at the current data file location
	int wkey = fwrite(&key, sizeof(key), 1, repo_handle.pds_data_fp);

	// Write the record after writing the key
	int wrec = fwrite(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);

	// check if written successfully or not
	if(wkey != 1 || wrec != 1) {
		return PDS_ADD_FAILED;
	}

	return PDS_SUCCESS;	
}



int get_rec_by_key(int key, void*rec)
{
	// Search for index entry in BST
	struct BST_Node * node = bst_search(repo_handle.pds_bst, key);
	if(node == NULL) {
		return PDS_REC_NOT_FOUND;
	}

	// Seek to the file location based on offset in index entry
	struct PDS_NdxInfo * ndx = node->data;
	fseek(repo_handle.pds_data_fp, ndx->offset, SEEK_SET);


	// Read the key at the current file location 
	int ckey;
	int kread = fread(&ckey, sizeof(key), 1, repo_handle.pds_data_fp);

	// Read the record after reading the key
	int rread = fread(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp); 

	if(rread != 1) {
		return PDS_FILE_ERROR;
	}
}





int bst_unload(struct BST_Node *root)
{
	if(root == NULL ) {
		return 0;
	}
	else {

		int stat = fwrite(root->data, sizeof(struct PDS_NdxInfo), 1, repo_handle.pds_ndx_fp);

		if(stat != 1) {
			return 1;
		}

		int s1 = bst_unload(root->left_child);
		int s2 = bst_unload(root->right_child);

		return s1 || s2;
	}
}



int pds_close()
{
	if(repo_handle.repo_status != PDS_REPO_OPEN) {
		return PDS_REPO_NOT_OPEN;
	}

	// Open the index file in wb mode (write mode, not append mode)
	char ndf[100];
	strcpy(ndf, repo_handle.pds_name);
	strcat(ndf, ".ndx");

	repo_handle.pds_ndx_fp = fopen(ndf, "wb");
	if(repo_handle.pds_ndx_fp == NULL) {
		return PDS_NDX_SAVE_FAILED;
	}

	// Unload the BST into the index file by traversing it in PRE-ORDER (overwrite the entire index file)
	int stat = bst_unload(repo_handle.pds_bst);
	if(stat == 1) {
		return PDS_NDX_SAVE_FAILED;
	}

	// Free the BST by calling bst_destroy()
	bst_destroy(repo_handle.pds_bst);

	// Close the index file and data file
	fclose(repo_handle.pds_ndx_fp);
	fclose(repo_handle.pds_data_fp);
	repo_handle.repo_status = PDS_REPO_CLOSED;
	repo_handle.rec_size = 0;
	strcpy(repo_handle.pds_name, "");

	return PDS_SUCCESS;
}







