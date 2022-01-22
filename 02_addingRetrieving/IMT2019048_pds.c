#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>

#include "pds.h"
#include "contact.h"

// Define the global variable
struct PDS_RepoInfo repo_handle;

int pds_create(char *repo_name)
{ 
 	// operning the file and handling file error
	FILE * fp = fopen(repo_name, "w");
	if(fp == NULL) {
		return PDS_FILE_ERROR;
	}

	// closing the file and returning success
	fclose(fp);
	return PDS_SUCCESS;
}





int pds_open( char *repo_name, int rec_size )
{
	// checking if any file is already open
	if(repo_handle.repo_status == PDS_REPO_OPEN) {
		return PDS_REPO_ALREADY_OPEN;
	}

	// trying to open the file
	repo_handle.pds_data_fp = fopen(repo_name, "rb+");
	if(repo_handle.pds_data_fp == NULL) {
		return PDS_FILE_ERROR;
	}

	// updating all the details in repo_handle if file opens successfully
	strcpy(repo_handle.pds_name, repo_name);
	repo_handle.rec_size = rec_size;
	repo_handle.repo_status = PDS_REPO_OPEN;

	return PDS_SUCCESS;
}


int put_rec_by_key( int key, void *rec )
{
	// check if some file is open or not
	if(repo_handle.repo_status != PDS_REPO_OPEN) {
		return PDS_FILE_ERROR;
	}

	// set pointer to end of file
	fseek(repo_handle.pds_data_fp, 0, SEEK_END);

	// writing key and record to file
	int wkey = fwrite(&key, sizeof(key), 1, repo_handle.pds_data_fp);
	int wrec = fwrite(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);

	// check if written successfully or not
	if(wkey != 1 || wrec != 1) {
		return PDS_ADD_FAILED;
	}

	return PDS_SUCCESS;

}


int get_rec_by_key( int key, void *rec )
{
	// check if some file is open or not
	if(repo_handle.repo_status != PDS_REPO_OPEN) {
		return PDS_FILE_ERROR;
	}

	// got to the beginning
	fseek(repo_handle.pds_data_fp, 0, SEEK_SET);


	// check if the mentioned key is there and find the corresponding record
	int found = 0;
	int ckey;

	int kread = fread(&ckey, sizeof(key), 1, repo_handle.pds_data_fp);
	int rread = fread(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp); 

	struct Contact * c = rec;
	
	while(kread == 1 && rread == 1) {

		if(ckey == key) {
			found = 1;
			break;
		}

		kread = fread(&ckey, sizeof(key), 1, repo_handle.pds_data_fp);
		rread = fread(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp); 		
	}

	// if record not contained in the file and/or some error was there finding it
	if(!found) {
		return PDS_REC_NOT_FOUND;
	}

	return PDS_SUCCESS;

}

int pds_close()
{
	// if no file is open, no need to close anything
	if(repo_handle.repo_status != PDS_REPO_OPEN) {
		return PDS_REPO_CLOSED;
	}

	// if some file is open, close it and reset repo_handle and fill status as closed
	fclose(repo_handle.pds_data_fp);
	repo_handle.repo_status = PDS_REPO_CLOSED;
	repo_handle.rec_size = 0;
	strcpy(repo_handle.pds_name, "");

	return PDS_SUCCESS;
}

