#ifndef BOOK_H
#define BOOK_H

#define BOOK_SUCCESS 0
#define BOOK_FAILURE 1
 
struct Book{
	int book_id;
	char book_name[40];
	char author[30];
	char genre[30];
};

extern struct PDS_RepoInfo *repoHandle;
int createStore(char *store_name);
int openStore(char *store_name);
int add_book( struct Book *c );
void print_books();
void print_book(struct Book *b);
int search_book( int book_id, struct Book *c );
int search_book_by_title(char *title, struct Book *b, int *io_count);
int match_book_title(void *rec, void *key);
int delete_book( int book_id );
int update_book(int book_id, struct Book * newbook);
int close_store();

#endif
