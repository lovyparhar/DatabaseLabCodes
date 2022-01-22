#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

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
}

int pds_load_ndx() // Same as before
{
// Internal function used by pds_open to read index entries into BST
}

int put_rec_by_key(int key, void*rec)
{
  // Seek to the end of the data file
  // Create an index entry with the current data file location using ftell
  // (NEW) ENSURE is_deleted is set to 0 when creating index entry
  // Add index entry to BST using offset returned by ftell
  // Write the key at the current data file location
  // Write the record after writing the key
}

int get_rec_by_ndx_key(int key, void*rec)
{
  // Search for index entry in BST
  // (NEW) Check if the entry is deleted and if it is deleted, return PDS_REC_NOT_FOUND
  // Seek to the file location based on offset in index entry
  // Read the key at the current file location 
  // Read the record after reading the key
}



int pds_close() 
{
// Open the index file in wb mode (write mode, not append mode)
// Unload the BST into the index file by traversing it in PRE-ORDER (overwrite the entire index file)
// (NEW) Ignore the index entries that have already been deleted. 
// Free the BST by calling bst_destroy()
// Close the index file and data file

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
}

int delete_rec_by_ndx_key( int key) // New Function
{
  // Search for the record in the BST using the key
  // If record not found, return PDS_DELETE_FAILED
  // If record is found, check if it has already been deleted, if so return PDS_DELETE_FAILED
  // Else, set the record to deleted and return PDS_SUCCESS
}