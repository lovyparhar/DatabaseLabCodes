#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "pds.h"
#include "book.h"
#include "bst.h"

struct PDS_RepoInfo repo_handle;

int createStore(char *store_name) {
	return pds_create(store_name);
}

int openStore(char *store_name) {
	return pds_open(store_name, sizeof(struct Book));
}

// Add the given book into the repository by calling put_rec_by_key
int add_book( struct Book *b )
{
	int status;
	status = put_rec_by_key(b->book_id, b);

	if(status != PDS_SUCCESS) {
		fprintf(stderr, "Unable to add book with id %d. Error %d", b->book_id, status);
		return BOOK_FAILURE;
	}
	return status;
}







void print_book(struct Book *b) {
	printf("%d \t %s \t %s \t %s\n", b->book_id, b->book_name, b->author, b->genre);
}

void bst_pr(struct BST_Node *root)
{
    if(root == NULL ) {
        return;
    }
    else {

        int stat = 1;
        struct PDS_NdxInfo * ndx = root->data;

        if(!(ndx->is_deleted)) {

        	struct Book *b = (struct Book *)malloc(sizeof(struct Book));
            int stat = get_rec_by_ndx_key(ndx->key, b);
            
            if(stat == 0) {
            	print_book(b);
            }

            free(b);
        }

        bst_pr(root->left_child);
        bst_pr(root->right_child);
    }
}

void print_books() {
	bst_pr(repo_handle.pds_bst);
}






int delete_book(int book_id)
{
	int status;
	status = delete_rec_by_ndx_key(book_id);

	if( status != PDS_SUCCESS ){
		fprintf(stderr, "Unable to delete book with id %d. Error %d\n", book_id, status);
		return BOOK_FAILURE;
	}
	return status;
}



int update_book(int book_id, struct Book * newbook)
{
	int status;
	status = delete_rec_by_ndx_key(book_id);

	if(status != PDS_SUCCESS){
		fprintf(stderr, "Unable to update book with id %d. Error %d\n", book_id, status);
		return BOOK_FAILURE;
	}

	status = put_rec_by_key(book_id, newbook);

	if(status != PDS_SUCCESS) {
		fprintf(stderr, "Unable to update book with id %d. Error %d\n", book_id, status);
		return BOOK_FAILURE;
	}

	return status;
}


int close_store() {
	return pds_close();
}

int search_book( int book_id, struct Book *b )
{
	return get_rec_by_ndx_key( book_id, b );
}


int search_book_by_title( char *title, struct Book *b, int *io_count )
{
	int status;
	status = get_rec_by_non_ndx_key(title, b, &match_book_title, io_count);

	if(status == PDS_SUCCESS) {
		status = BOOK_SUCCESS;
	}
	else {
		status = BOOK_FAILURE;
	}

	return status;
}

int match_book_title(void *rec, void *key)
{
    if(rec == NULL || key == NULL) {
		return 2;
	}

    struct Book * b = (struct Book *)rec;
    char * title = (char *)key;

   	if(strcmp(b->book_name, title) == 0) {
   		return 0;
   	} 
   	else {
   		return 1;
   	}
}
