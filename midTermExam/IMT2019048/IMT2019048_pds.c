#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include "contact.h"
#include "pds.h"
#include "bst.h"

struct PDS_RepoInfo repo_handle;


int pds_create(char *repo_name) 
{
  char filename[30], indexfile[30];
  strcpy(filename,repo_name);
  strcpy(indexfile,repo_name);
  strcat(filename,".dat");
  strcat(indexfile,".ndx");
  FILE *fp = fopen(filename,"wb+");
  FILE *ifp = fopen(indexfile,"wb+");
  if(fp  == NULL || ifp == NULL) return PDS_FILE_ERROR;\
  fclose(fp);
  fclose(ifp);
  
  return PDS_SUCCESS;
}




int pds_open(char* repo_name, int rec_size) // Same as before
{
    // Open the data file and index file in rb+ mode
    // Update the fields of PDS_RepoInfo appropriately
    // Build BST and store in pds_bst by reading index entries from the index file
    // Close only the index file

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

    repo_handle.pds_ndx_fp = fopen(ndf, "rb+");
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






int pds_load_ndx() // Same as before
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
    // Seek to the end of the data file
    // Create an index entry with the current data file location using ftell
    // (NEW) ENSURE is_deleted is set to 0 when creating index entry
    // Add index entry to BST using offset returned by ftell
    // Write the key at the current data file location
    // Write the record after writing the key

    // Check if some file is open or not
    if(repo_handle.repo_status != PDS_REPO_OPEN) {
        return PDS_FILE_ERROR;
    }

    // Check if the key is already there
    struct BST_Node * node = bst_search(repo_handle.pds_bst, key);
    if(node != NULL) {

        struct PDS_NdxInfo * ndx = node->data;
        if(ndx->is_deleted == 0) {

            return PDS_ADD_FAILED;
        }
        else {

            fseek(repo_handle.pds_data_fp, 0, SEEK_END);
            ndx->key = key;
            ndx->offset = ftell(repo_handle.pds_data_fp);
            ndx->is_deleted = 0;

            int wkey = fwrite(&key, sizeof(key), 1, repo_handle.pds_data_fp);
            int wrec = fwrite(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);

            if(wkey != 1 || wrec != 1) {
                return PDS_ADD_FAILED;
            }
        }
    }



    // Seek to the end of the data file
    fseek(repo_handle.pds_data_fp, 0, SEEK_END);

    // Create an index entry with the current data file location using ftell
    struct PDS_NdxInfo * ndxinfo = (struct PDS_NdxInfo *)malloc(1 * sizeof(struct PDS_NdxInfo));
    ndxinfo->key = key;
    ndxinfo->offset = ftell(repo_handle.pds_data_fp);
    ndxinfo->is_deleted = 0;

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




int get_rec_by_ndx_key(int key, void*rec)
{
    // Search for index entry in BST
    // (NEW) Check if the entry is deleted and if it is deleted, return PDS_REC_NOT_FOUND
    // Seek to the file location based on offset in index entry
    // Read the key at the current file location 
    // Read the record after reading the key

    // Search for index entry in BST
    struct BST_Node * node = bst_search(repo_handle.pds_bst, key);
    if(node == NULL) {
        return PDS_REC_NOT_FOUND;
    }


    // Seek to the file location based on offset in index entry
    struct PDS_NdxInfo * ndx = node->data;
    if(ndx->is_deleted == 1) {
        return PDS_REC_NOT_FOUND;
    }

    fseek(repo_handle.pds_data_fp, ndx->offset, SEEK_SET);

    // Read the key at the current file location 
    int ckey;
    int kread = fread(&ckey, sizeof(key), 1, repo_handle.pds_data_fp);

    // Read the record after reading the key
    int rread = fread(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp); 

    if(rread != 1) {
    return PDS_FILE_ERROR;
    }

    return PDS_SUCCESS;
}




int bst_unload(struct BST_Node *root)
{
    if(root == NULL ) {
        return 0;
    }
    else {

        int stat = 1;
        struct PDS_NdxInfo * ndx = root->data;
        if(!(ndx->is_deleted)) {
            stat = fwrite(root->data, sizeof(struct PDS_NdxInfo), 1, repo_handle.pds_ndx_fp);
        }

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
    // Open the index file in wb mode (write mode, not append mode)
    // Unload the BST into the index file by traversing it in PRE-ORDER (overwrite the entire index file)
    // (NEW) Ignore the index entries that have already been deleted. 
    // Free the BST by calling bst_destroy()
    // Close the index file and data file

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

int get_rec_by_non_ndx_key(void *key, void *rec, int (*matcher)(void *rec, void *key), int *io_count)
{
    // Seek to beginning of file
    // Perform a table scan - iterate over all the records
    //   Read the key and the record
    //   Increment io_count by 1 to reflect count no. of records read
    //   Use the function in function pointer to compare the record with required key
    //   (NEW) Check the entry of the record in the BST and see if it is deleted. If so, return PDS_REC_NOT_FOUND
    // Return success when record is found

    fseek(repo_handle.pds_data_fp, 0, SEEK_SET);

    int found = 0;
    while(1) {

        int numkey;
        int kread = fread(&numkey, sizeof(int), 1, repo_handle.pds_data_fp);
        int rread = fread(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);  

        if(kread != 1 || rread != 1) {
            break;
        }   

        (*io_count)++;
        if(matcher(rec, key) == 0) {

            struct BST_Node * node = bst_search(repo_handle.pds_bst, numkey);
            struct PDS_NdxInfo * ndx = node->data;
            if(ndx->is_deleted == 1) {
                return PDS_REC_NOT_FOUND;
            }

            found = 1;
            break;
        }

    }

    if(found) {
        return PDS_SUCCESS;
    }
    else {
        return PDS_REC_NOT_FOUND;
    }
}

int delete_rec_by_ndx_key( int key) // New Function
{
    // Search for the record in the BST using the key
    // If record not found, return PDS_DELETE_FAILED
    // If record is found, check if it has already been deleted, if so return PDS_DELETE_FAILED
    // Else, set the record to deleted and return PDS_SUCCESS

    struct BST_Node * node = bst_search(repo_handle.pds_bst, key);
    if(node == NULL) {
        return PDS_DELETE_FAILED;
    }


    // If record is found, check if it has already been deleted, if so return PDS_DELETE_FAILED
    struct PDS_NdxInfo * ndx = node->data;
    if(ndx->is_deleted == 1) {
        return PDS_DELETE_FAILED;
    }

    ndx->is_deleted = 1;

    return PDS_SUCCESS;
}



int pds_overwrite( int key, void *rec ) {
    // Overwrite an existing record with a new record
    // Return relevant error code if record does not exist or any other error
    // This implementation overwrites existing record with the given object

    // Check if a record with this key exists
    struct BST_Node * node = bst_search(repo_handle.pds_bst, key);
    if(node == NULL) {
        return PDS_UPDATE_FAILED;
    }

    // If record is found, check if it has already been deleted
    struct PDS_NdxInfo * ndx = node->data;
    if(ndx->is_deleted == 1) {
        return PDS_UPDATE_FAILED;
    }

    // Go to that record in the data file
    fseek(repo_handle.pds_data_fp, ndx->offset, SEEK_SET);

    // Read the key
    int ckey;
    int kread = fread(&ckey, sizeof(int), 1, repo_handle.pds_data_fp);

    ftell(repo_handle.pds_data_fp);

    // Check if ckey read properly and is equal to key
    if(kread != 1 || key != ckey) {
        return PDS_UPDATE_FAILED;
    }

    // Overwrite record
    int wrec = fwrite(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);

    if(wrec != 1) {
        return PDS_UPDATE_FAILED;
    }

    return PDS_SUCCESS;
}




















