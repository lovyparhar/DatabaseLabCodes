#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "pds.h"
#include "book.h"
#include "bst.h"


int display_menu2() {
	printf("\n1.  Add Book\n");
	printf("2.  Modify Book\n");
	printf("3.  Delete Book\n");
	printf("4.  Search Book by book-id\n");
	printf("5.  Search Book by title\n");
	printf("6.  Print Books\n");
	printf("0.  Save and go back\n\n");

	int ch; scanf("%d", &ch);
	const int n = ch;

	switch(n) {

		case 1: {

			struct Book * nb = (struct Book *)malloc(sizeof(struct Book));

			printf("Give the id of the book:\n");
			scanf("%d", &(nb->book_id));

			printf("Give the name of the book:\n");
			scanf("%s", nb->book_name);

			printf("Give the author of the book:\n");
			scanf("%s", nb->author);

			printf("Give the genre of the book:\n");
			scanf("%s", nb->genre);

			if(add_book(nb) == 0) {
				printf("Book added successfully\n");
			}
			
			free(nb);
			break;
		}


		case 2: {

			printf("Give the id of the book to update\n");
			int inpid;
			scanf("%d", &inpid);

			struct Book * nb = (struct Book *)malloc(sizeof(struct Book));
			nb->book_id = inpid;

			printf("New name of the book:\n");
			scanf("%s", nb->book_name);

			printf("New author of the book:\n");
			scanf("%s", nb->author);

			printf("New genre of the book:\n");
			scanf("%s", nb->genre);

			if(update_book(inpid, nb) == 0) {
				printf("Book updated successfully\n");
			}

			free(nb);

			break;
		}

		case 3: {

			printf("Give the book id of the book to delete:\n");
			int inpid;
			scanf("%d", &inpid);

			if(delete_book(inpid) == 0) {
				printf("Book deleted successfully\n");
			}

			break;
		}

		case 4: {

			printf("Book id:\n");
			int inpid;
			scanf("%d", &inpid);

			struct Book * nb = (struct Book *)malloc(sizeof(struct Book));

			if(search_book(inpid, nb) == 0) {
				print_book(nb);
			}
			else {
				printf("Unable to find the book with book id %d\n", inpid);
			}

			free(nb);

			break;
		}

		case 5: {

			printf("Book title:\n");
			char inptitle[40];
			scanf("%s", inptitle);

			struct Book * nb = (struct Book *)malloc(sizeof(struct Book));

			int iocount = 0;
			if(search_book_by_title(inptitle, nb, &iocount) == 0) {
				print_book(nb);
			}
			else {
				printf("Unable to find the book with title %s\n", inptitle);
			}

			free(nb);

			break;
		}

		case 6: {
			print_books();
			break;
		}

		case 0: {
			close_store();
			return 1;
		}
	}

	return 0;
}


int display_menu1() {
	printf("\n1. Create a new store\n");
	printf("2. Open existing store\n");
	printf("0. Quit\n\n");

	int ch; scanf("%d", &ch);
	const int n = ch;

	switch(n) {

		case 1: {
			char sname[30];
			printf("Give the store name for the new store:\n");
			scanf("%s", sname);
			if(createStore(sname) == 0) {
				printf("Store created successfully\n");
			}
			else {
				printf("Unable to create store\n");
			}

			break;
		}


		case 2: {

			char sname[30];
			printf("Give the store name to open:\n");
			scanf("%s", sname);
			if(openStore(sname) == 0) {
				printf("Store opened successfully\n");
				
				while(1) {
					int endm2 = display_menu2();
					if(endm2) break;
				}

			}
			else {
				printf("Unable to open store with name %s\n", sname);
			}

			break;
		}

		case 0: {
			return 1;
		}
	}

	return 0;
}


int main()
{
	while(1) {
		int endm1 = display_menu1();
		if(endm1) break;
	}
}



