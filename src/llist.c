/*
 * vfs_linkedlist.c
 *
 *  Created on: Sep 28, 2009
 *      Author: arunk
 */

#include "llist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

l_node *list_insert_head( l_node *head, void *data ) {

	l_node *current;

	//No need to check if head is null, even if it is our next wil be nulll

	//First allocate memory to store data
	current = (l_node*)malloc(sizeof(l_node));
	current->data = data;
	//Our current node will become the head hence it's next is head
	current->next = head;

	return current;

}
l_node *list_insert_tail( l_node *head,void *data){
	l_node *current, *prev, *new_node;

	//First check if head is null, if so then just insert it using insert_head method
	if (head == NULL) {
		return list_insert_head(head, data);
	}

	prev = NULL;
	current = head;

	//Now traverse the list
	while(current != NULL) {
		prev = current;
		current = current->next;
	}
	//We have reached the end of the list, hence the new node has to be inserted at the end
	//Allocate memory to the node to store data
	new_node = (l_node*)malloc(sizeof(l_node));
	new_node->data = data;
	new_node->next = NULL; // as it is the last nodt
	prev->next=new_node;
	return head;

}

l_node *list_insert_sortorder( l_node *head,void *data,char *filename){

	l_node *current, *prev, *new_node;

	//Workaround due to problem in devsquare
	return list_insert_tail(head, data);

	//First check if head is null, if so then just insert it using insert_head method
	if (head == NULL) {
		return list_insert_head(head, data);
	}

	prev = NULL;
	current = head;

	//Now traverse the list
	while(current != NULL) {
		//compare the filenames, and see if the argument is less than or equal to the list node
		if (strcmp(filename, ((h_node*)(current->data))->file_name) <= 0) {
			//Insert the node after prev
			//But now, if prev is null, it is just like inserting in head
			if (prev == NULL) {
				return list_insert_head(head,data);
			}
			//Allocate memory to the node to store data
			new_node = (l_node*)malloc(sizeof(l_node));
			new_node->data = data;
			//Just manipulate the links, prev-->new_node-->current
			new_node->next=current;
			prev->next = new_node;
			return head;
		}
		prev = current;
		current = current->next;
	}
	//We have reached the end of the list, hence the new node has to be inserted at the end
	//Allocate memory to the node to store data
	new_node = (l_node*)malloc(sizeof(l_node));
	new_node->data = data;
	new_node->next = NULL; // as it is the last nodt
	prev->next=new_node;
	return head;
}


l_node *list_delete_head( l_node *head ) {

	l_node *new_head;

	//Error checking, if head is null
	if (head == NULL) {
		return NULL;
	}

	//Once we delete head, the next element in head becomes the new head
	new_head = head->next;
	//?? Here do we need to free head->data also??
	free(head);

	return new_head;
}


l_node *list_delete_node( l_node *head, void *data ) {

	l_node *prev, *current;

	//Error checking, if head is null
	if (head == NULL) {
		return NULL;
	}

	prev = NULL;
	current = head;

	//Now traverse the list
	while(current != NULL) {
		//compare the data, and see if the data in arg is equal to data in list node
		if (current->data == data) {
			//Delete the node pointed by current
			//But now, if prev is null, it is just like deleting head
			if (prev == NULL) {
				return list_delete_head(head);
			}
			//Just manipulate the links, remove current and prev-->current.next
			prev->next=current->next;
			free(current);
			break;
		}
		prev = current;
		current = current->next;
	}

	return head;
}

int list_get_data_list( l_node *head , char *filename,h_node **data[]) {

	//current_max maintains the total size of array
	//size maintains the number of valid elements in the array
	int current_max, size;

	h_node **list;

	/*The idea here is that data will contain the base address of the
	 * array of pointers, and these pointers are pointing to h_node.
	 */

	//As always we do the most basic error checking
	if (head == NULL) {
		return 0;
	}

	/*Now we know that *data is an array of pointers
	 * but how do we get the size of the array, it solely depends on the number
	 * of nodes in the list that have the same filename.
	 */
	/*So we start with an array of size 2 and then later, if there are more nodes,
	 * we just create a new array with double the size and copy these elements.
	 * Similar to the implementation of ArrayList/Vector in java.
	 */

	list = (h_node**)malloc(sizeof(h_node*) * 2);

	//current_max is 2 because the total size of the array is only 2
	//So, at the max only two elements can be stored in this, unless we realloc
	current_max = 2;
	//But size is 0, why?? because we havent found any node which matches our filename.
	size = 0;

	//Now traverse the list
	while(head != NULL) {
		//compare the filenames, and see if the arg filename is equal to the one in list node
		if (strcmp(filename, ((h_node*)(head->data))->file_name) == 0) {
			//We need to check our bounds, ie. size should be less than current_max
			if (size == current_max) {
				//we have already exceeded our limit, hence re-alloc
				//The new array will be double the previous size.
				current_max = current_max * 2;
				list = (h_node**)realloc(list, sizeof(h_node*) * (current_max));
			}
			list[size] = (h_node*)head->data;
			size++;
		}
		head = head->next;
	}
	if (size == 0) {
		free(list);
	} else {
		*data = list;
	}
	return size;
}

/*
 *This writes the data as int and returns the number of integers written.
 * Returns -1 on write failure
 */
int list_write_data_int (l_node *head, FILE *fp){
	int count = 0;

	while(head != NULL) {
		if (fwrite(head->data,sizeof(int),1,fp) != 1) {
			return -1;
		}
		head = head->next;
		count++;
	}

	return count;
}

