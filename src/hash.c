/*
 * vfs_hashtable.c
 *
 *  Created on: Sep 28, 2009
 *      Author: arunk
 */

#include "vfs_error_codes.h"
#include "hash.h"
#include "llist.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* initialize all the l_node *  in h_bucket to NULL */
void hash_initialize(l_node **hash) {
	int i;

	//Error checking
	if (hash == NULL)
		return;

	for (i = 0; i < NO_OF_BUCKETS; ++i) {
		hash[i] = NULL;
	}
}

/*Gets the hash value of the key in argument */
static int hash_value(char key) {
		int ch;

		//Numbers
		ch = key - '0';
		if (ch>=0 && ch <= 9)
			return ch;

		//Upper case chars
		ch=key-'A';
		if (ch>=0 && ch<26)
			return ch + 10;

		//Lower case chars
		ch=key-'a';
		if (ch>=0 && ch < 26)
			return ch+ 10 +26;

		return ERROR_HASH_KEY_INVALID;
}


static h_node* get_h_node_from_fd(fd *data){
	h_node *hash_node;
	hash_node = (h_node*)malloc(sizeof(h_node));
	hash_node->fd_id = data->fd_id;
	hash_node->file_desc = data;
	strncpy(hash_node->file_name, data->file_name, MAX_FILEDIR_NAME_SIZE);
	strncpy(hash_node->path, data->path, MAX_PATH_SIZE);
	return hash_node;

}

int hash_insert(l_node *h_bucket[], fd* data){

	h_node *hash_node;
	int key;

	//Error checking
	if (h_bucket == NULL || data == NULL) {
		return ERROR_NULL_POINTER;
	}

	//Find the key
	key = hash_value(data->file_name[0]);

	if (key == ERROR_HASH_KEY_INVALID)
		return ERROR_HASH_KEY_INVALID;

	//Now create the h_node
	hash_node = get_h_node_from_fd(data);

	h_bucket[key] = list_insert_sortorder(h_bucket[key], hash_node, hash_node->file_name);
	return NO_ERROR;
}

/* Return 0 on success and non-zero on failure */
int hash_delete(l_node *h_bucket[],char *path, char *file_name) {

	int key, count, i;
	h_node **hash_data;

	//Error checking
	if (h_bucket == NULL || file_name == NULL) {
		return ERROR_NULL_POINTER;
	}

	//Find the key
	key = hash_value(file_name[0]);

	if (key == ERROR_HASH_KEY_INVALID)
		return ERROR_HASH_KEY_INVALID;

	//Use the linked list method to get list of hash_nodes
	count  = list_get_data_list(h_bucket[key], file_name, &hash_data);
	for (i = 0; i < count; ++i) {
		if (strcmp(path, hash_data[i]->path) == 0) {
			h_bucket[key] = list_delete_node(h_bucket[key], hash_data[i]);
			//Just return here because there cannot be two files with same name and path
			return NO_ERROR;
		}
	}

	//The file does not exist in the entire file system
	if (count <= 0) {
		return ERROR_FILE_NOT_FOUND;
	}

	//The file exists, but in a different path
	//free the array
	free (hash_data);
	return 0;
}

/* list of fds as search result in *list */
int hash_search(l_node *h_bucket[], char *file_name, fd **list[]){

	int key, count, i;
	h_node **hash_data;
	fd **fd_list;

	//Error checking
	if (h_bucket == NULL || file_name == NULL) {
		return 0;
	}

	//Find the key
	key = hash_value(file_name[0]);

	//Use the linked list method to get list of hash_nodes
	count  = list_get_data_list(h_bucket[key], file_name, &hash_data);
	if (count == 0) {
	    return 0;
	}

	fd_list = (fd**)malloc(count * sizeof(fd*));
	for (i = 0; i < count; ++i) {
		fd_list[i] = hash_data[i]->file_desc;
	}
	*list = fd_list;

	//free the array
	free (hash_data);
	return count;
}

static h_node* get_h_node_from_void(void *data) {
	return ((h_node *)data);
}

void hash_print(l_node *h_bucket[]) {

	l_node *head;
	int i;

	for (i = 0; i < NO_OF_BUCKETS; ++i) {
		if (h_bucket[i] != NULL) {
			printf("The following are the contents of the bucket with key = %d :\n",i);
			head = h_bucket[i];
			while(head != NULL){
				printf("File name is: %s\n",get_h_node_from_void(head->data)->file_name);
				printf("Path is: %s\n\n",get_h_node_from_void(head->data)->path);
				head=head->next;
			}
			printf("==============================\n\n");
		}
	}
}


