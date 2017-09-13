
#ifndef NARYTREE_H
#define NARYTREE_H

#include "globals.h"


#define ERROR_SOURCE_DEST_SAME 127
#define ERROR_SOURCE_EXISTS_UNDER_DEST 126
#define PERMANENTLY_DELETED 2


typedef struct tree_node
{
fd *data;

int is_deleted;
struct tree_node *left_child;
struct tree_node *right_siblings;
struct tree_node *parent;
}t_node;


int insert_node(t_node** root, fd* data);
int insert_node_with_action(t_node** root, fd* data, int (*on_duplicate_in_trash)(t_node *node));

/* Inserts a node into the tree. It gets the relevant information from the file descriptor passed to it. If the path is invalid, appropriate error message is returned*/
int delete_node(t_node** root,char* fullpath);
/* Removes a node from the tree. The infrmation about the node to be removed could be determined using the path and the name. */
int nary_deleteHelper (t_node *node);
int delete_recursively(t_node* node);

t_node* returnnode(t_node* root, char* fullpath);
/* Given the path, it returns the node from the tree*/

fd* get_node(t_node** root,char* fullpath);
/* Given the path, it returns the fd of the node from the tree*/

int get_children(t_node** root,char* fullpath,fd **data[]);

int move_node(t_node* root,char* source_path,char* destination_path);

int print_tree(t_node* root); /* Prints the tree.*/

int printtree(t_node* root); //DEVS: Added here

int nary_preOrder_process (t_node *root, int (*processNode)(t_node *node));

#endif


