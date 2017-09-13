#ifndef HASH_H
#define HASH_H

#include "globals.h"

//Maximum number of buckets as per our hash function is 62 = 26(upper case) + 26(lower case) + 10 (numbers)
#define NO_OF_BUCKETS 62

//typedef struct hash_node_struct{
    //int value;
    //bst_node *root;
//}hash_node;

/* initialize all the l_node *  in h_bucket to NULL */
void hash_initialize(l_node **hash);

int hash_insert(l_node *h_bucket[], fd* data);

int hash_delete(l_node *h_bucket[],char *path, char *file_name);
/* Return 0 on success and non-zero on failure */

int hash_search(l_node *h_bucket[], char *file_name, fd **list[]);
/* list of fds as search result in *list */

static int hash_value(char key);
/*Gets the hash value of the key in argument */

void hash_print(l_node *h_bucket[]);

#endif

