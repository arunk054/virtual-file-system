#ifndef LLIST_H
#define LLIST_H

#include "globals.h"
#include <stdio.h>

#define C_OR_CPP 1

l_node *list_insert_head( l_node *head, void *data );

l_node *list_insert_tail( l_node *head,void *data);

l_node *list_insert_sortorder( l_node *head,void *data,char *filename);

l_node *list_delete_head( l_node *head );

l_node *list_delete_node( l_node *head, void *data );

int list_get_data_list( l_node *head , char *filename,h_node **data[]);

int list_write_data_int (l_node *head, FILE *fp);

#endif

