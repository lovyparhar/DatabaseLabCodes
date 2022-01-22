#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "pds.h"
#include "contact.h"

// Load all the contacts from a CSV file
int store_contacts( char *contact_data_file )
{
	FILE *cfptr;
	char contact_line[500], token;
	struct Contact c, dummy;

	cfptr = (FILE *) fopen(contact_data_file, "r");
	while(fgets(contact_line, sizeof(contact_line)-1, cfptr)){
		//printf("line:%s",contact_line);
		sscanf(contact_line, "%d%s%s", &(c.contact_id),c.contact_name,c.phone);
		print_contact( &c );
		add_contact( &c );
	}
}

void print_contact( struct Contact *c )
{
	printf("%d,%s,%s\n", c->contact_id,c->contact_name,c->phone);
}

// Use get_rec_by_key function to retrieve contact
int search_contact( int contact_id, struct Contact *c )
{
	return get_rec_by_ndx_key( contact_id, c );
}

// Add the given contact into the repository by calling put_rec_by_key
int add_contact( struct Contact *c )
{
	int status;

	status = put_rec_by_key( c->contact_id, c );

	if( status != PDS_SUCCESS ){
		fprintf(stderr, "Unable to add contact with key %d. Error %d", c->contact_id, status );
		return CONTACT_FAILURE;
	}
	return status;
}

// Use get_rec_by_non_ndx_key function to retrieve contact
// Hint: get_rec_by_non_ndx_key( phone, c, &match_contact_phone, io_count );
int search_contact_by_phone( char *phone, struct Contact *c, int *io_count )
{
	// Call function
	int status;

	status = get_rec_by_non_ndx_key(phone, c, &match_contact_phone, io_count);

	if( status == PDS_SUCCESS )
		status = CONTACT_SUCCESS;
	else
		status = CONTACT_FAILURE;

	return status;
}

/* Return 0 if phone of the contact matches with phone parameter */
/* Return 1 if phone of the contact does NOT match */
/* Return > 1 in case of any other error */
int match_contact_phone( void *rec, void *key )
{
	// Store the rec in a struct contact pointer
    // Store the key in a char pointer
    // Compare the phone values in key and record
    // Return 0,1,>1 based on above condition

    if(rec == NULL || key == NULL) {
		return 2;
	}

    struct Contact * c = (struct Contact *)rec;
    char * phno = (char *)key;

   	if(strcmp(c->phone, phno) == 0) {
   		return 0;
   	} 
   	else {
   		return 1;
   	}
}

// Function to delete a record based on ndx_key
int delete_contact ( int contact_id )
{
	// Call the delete_contact_ndx_key function
	// Return CONTACT_SUCCESS or CONTACT_FAILURE based on status of above call

	int status;

	status = delete_rec_by_ndx_key(contact_id);

	if( status != PDS_SUCCESS ){
		return CONTACT_FAILURE;
	}
	return status;
}


int overwrite_contact( struct Contact *c ) {

	int status;

	status = pds_overwrite(c->contact_id, c);

	if( status != PDS_SUCCESS ){
		return CONTACT_FAILURE;
	}
	
	return status;
}


