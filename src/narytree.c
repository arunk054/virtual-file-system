/*
 * vfs_nary.c
 *
 *  Created on: Oct 22, 2009
 *      Author: Arun K
 */
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include "narytree.h"
#include "string_tok.h"
#include "vfs_error_codes.h"

//Just a few constant strings
static char rootStr[2] = {'/','\0'};
static char delimitor[2] = {'/','\0'};

//Used in move_node method
static int source_path_len;
static char *move_to_dest_path;

/*
 * Helper method to check if the path is present in the list
 * pointed by head, and rightSibling is the next pointer.
 * We return the last occurrence of the node because, an undeleted node,
 * has to be added after all the other nodes.
 */
static t_node* findNodeInSiblings(t_node *head, char *file){

	t_node *retNode = NULL;

	if(head == NULL){
		return NULL;
	}

	//Here we take the last match, because an undeleted node will always occur last.
	while(head != NULL){
		if (strcmp(head->data->file_name,file) == 0)
			retNode = head;
		head = head->right_siblings;
	}

	//If it is null, implies not found
	return retNode;
}

//Helper to check if the give fd corresponds to the root node
static int isRootNode(fd *data){
	return (strcmp(data->path, rootStr) == 0 && strcmp(data->file_name, rootStr) == 0);
}

/* Given the path, it returns the node from the tree
 * We return it irrespective of is_deleted or not. The caller has to
 * check if the node returned is a deleted node or not.
 * Also Note that the path in fd should not end with '/'*/
 t_node* returnnode(t_node* root, char* fullpath)
{
	char **tokenList;
	int count, i, level = 0;
	char *currentPath;

	//Error check
	if (root == NULL)
		return NULL;

	//Now tokenize the fullpath
	count = get_tokens(&tokenList,fullpath,delimitor[0]);
	if (count <= 0)
		return NULL;

	//this will be updated incrementally as we traverse the tree
	currentPath = (char*) malloc((strlen(fullpath)+1)*sizeof(char));

	//Check if the root is '/' in tree. ie both path and fileName of root is '/'
	//And check token[0] is empty i.e. the fullpath starts with '/'
	//Also we assume that root has no siblings and is_deletet cannot be true.
	if (tokenList[0][0] != '\0' || !isRootNode(root->data)) {
		free_tokens(tokenList,count);
		return NULL;
	}

	//Deletion of root is not allowed, so we assume it to be present
	strcpy(currentPath,rootStr);

	//We now know that the first token in fullPath is '/' hence loop from 1
	for (i = 1; i < count; ++i) {

		//Skip tokens that are empty, eg /home//abc
		if(tokenList[i][0]=='\0')
			continue;

		//This will check if the node with path and file exists in the children
		root = findNodeInSiblings(root->left_child,tokenList[i]);

		//Path does not exist
		if (root == NULL)
			break;
		//Append '/' only for nodes after crossing one level
		if (level > 0) {
			strcat(currentPath,delimitor);
		}
		strcat(currentPath,tokenList[i]);

		//level is not used at the moment, it is only for the above if condition
		level++;

	}

	//Free the memory as it was allocated in get_tokens
	free_tokens(tokenList, count);
	free(currentPath);
	return  root;
}


/* Given the path, it returns the fd of the node from the tree, ANd the node
 * in the tree should not be deleted ie is_deleted != true*/
fd* get_node(t_node** root,char* fullpath) {
	t_node *node;
	node = returnnode(*root,fullpath);
	if (node != NULL && node->is_deleted == 0)
		return node->data;
	return NULL;
}


/*
 * returns the count and only the children whose is_deleted is false.
 */
int get_children(t_node** root,char* fullpath,fd **data[]) {

	fd **retData;
	t_node *node;
	//Size of the data array and the current Max memory that has been allocated to it.
	int size, curMax;

	if (root == NULL || *root == NULL)
		return -1;

	node = returnnode(*root,fullpath);

	//ERROR: Path does not exist or it has been deleted
	if (node == NULL || node->is_deleted != 0)
		return -1;

	//Start with the left most child
	node = node->left_child;

	//Initially allocate memory to store 2 fd's
	size = 0; curMax = 2;
	retData = (fd**)malloc(curMax * sizeof(fd*));

	while(node != NULL){
		//Note we store only those fd's which are in t_node with is_deleted = false
		if (node->is_deleted == 0){
			//We double the array, every time the size reaches max
			if (size == curMax) {
				curMax = curMax * 2;
				retData  = (fd**)realloc(retData,curMax * sizeof(fd*));
			}
			//Store the fd in the array
			retData[size++] = node->data;
		}
		node = node->right_siblings;
	}

	//Just a best practice step, because the caller may not free it if size is 0
	if (size == 0) {
		free(retData);
	} else {
		*data = retData;
	}

	return size;
}
/*
 * Helper function to create a t_node and set its fields
 */
static t_node* getNewNode(fd *data, t_node *parent){
	t_node *newNode;

	newNode = (t_node*)malloc(sizeof(t_node));
	newNode->parent = parent;
	newNode->data = data;
	//Since it is a new node it obviously cannot have any children
	//or any other node in the same level, ie right sibling.
	newNode->left_child = NULL;
	newNode->right_siblings=NULL;
	newNode->is_deleted = 0;

	return newNode;
}

int insert_node_with_action(t_node** root, fd* data, int (*on_duplicate_in_trash)(t_node *node)){


	t_node *parent;
	t_node *temp;
	//This will point to the addr of the link into which the new node will be stored

	//First, err check and get the path from fd
	if (data == NULL || root == NULL)
		return -1;

	//Special case of inserting the root node, ie either root is null
	//OR the data corresponds to a root node
	if (*root == NULL) {
		//In general we can insert anything into the nary tree, but in this case,
		//it has been asked to check if the root node has path and file as '/'
		if (!isRootNode(data))
		{
			//The node being inserted at the root is not a valid root,
			//because either its file or path is not '/'
			return ERROR_INVALID_PATH;
		}
		//Get a new node allocated and make it the root.
		*root = getNewNode(data,NULL);
		return 0;
	} else if (isRootNode(data)){
		//This means root exists and we are trying to insert a root node again
		return ERROR_INVALID_PATH;
	}

	//First we see if the path in fd exists
	parent = returnnode(*root,data->path);

	//Either path does not exist or it is deleted.
	if (parent == NULL || parent->is_deleted != 0)
		return ERROR_INVALID_PATH;

	//Now check if the node we are trying to insert already exists.
	temp = findNodeInSiblings(parent->left_child,data->file_name);

	//The node exists and it is not deleted, so our data is a duplicate
	//if the node exists and it is deleted = true, then delete it from trash
	//Future, this needs to be taken care outside, and let the user decide
	//whether to delete permanently or not insert.
	if (temp != NULL) {
		if (temp->is_deleted == 0)
			return ERROR_FILE_ALREADY_EXISTS;
		else if (temp->is_deleted == 1){
			if (on_duplicate_in_trash != NULL) {
				on_duplicate_in_trash(temp);
			}
		}//the other case is if it is already permanently deleted then nothing to do
	}

	//We now finally try to actually insert it...
	temp = parent->left_child;

	//It has no children, hence insert at head ie left_child
	if (temp == NULL) {
		parent->left_child = getNewNode(data,parent);
		return 0;
	}

	//Traverse all the children and insert at the end.
	while (temp->right_siblings != NULL){
		temp = temp->right_siblings;
	}
	temp->right_siblings = getNewNode(data,parent);
	return 0;

}

/*Note that the path in fd should not end with '/'
 * except for the root node.
 */
//todo : check if the path exceeds max path len, may be do it in main.
int insert_node(t_node** root, fd* data){
	return insert_node_with_action(root,data,NULL);
}

/* Removes a node from the tree. The information about the node to be removed could be determined using the path and the name. */
int delete_node(t_node** root,char* fullpath){

	t_node *toDelete;

	//Although this is not possible
	if (root == NULL)
		return -1;

	//Return error if we are trying to delete the root ie '/'

	if (strcmp(fullpath,rootStr) == 0) {
		//May be we can return some specific error code,
		//saying OPERATION_NOT_ALLOWED etc
		return 1;
	}

	toDelete = returnnode(*root, fullpath);

	//Path does not exist or it is already deleted
	if (toDelete == NULL || toDelete->is_deleted != 0) {
		return -1;
	}

	return delete_recursively(toDelete);
}


/*
 *This is an interesting preorder function, the second param is a FUNCTION POINTER.
 *The idea is to do some processing when we visit a node, so instead of just printing the node,
 * We will call the processNode function on the node. For now this serves 3 purposes. 1. Set isDelete to true,
 * 2. print the tree, 3. write the node to a file. I call it height of re-usability.
 * The accumulator holds the result of processNode, it is to avoid stack when checking error status.
 */
static int preOrder (t_node *root, int (*processNode)(t_node *node)){
	t_node *child;
	int retVal;

	if (root == NULL){
		return 0;
	}

	//Here we cud use an accumulator to reduce the stack a bit, instead of retVal
	//Anyway the retVal is not required to be stored in the stack, because the retVal is overwritten
	//after the recursive call, not sure if the compiler can figure this out. Need to check it...
	retVal = processNode(root);
	if (retVal != 0) {
		return retVal;
	}
	child = root->left_child;
	while(child != NULL) {
		retVal = preOrder(child, processNode);
		//Error condition, hence return, same as doing above
		if (retVal != 0){
			return retVal;
		}
		child = child->right_siblings;
	}

	return 0 ;

}

//Exposed method to access nary preorder method.
int nary_preOrder_process(t_node *root, int (*processNode)(t_node *node)){
	return preOrder(root, processNode);
}

//This function addr is passed to preOrder as 2nd param
int nary_deleteHelper (t_node *node){
	//THis check is needed, because we have introduced a new flag called PERMANENTLY_DELETED
	//eg: /home/def/file1 might be permanenly deleted, but now we may want to move def to trash...
	if (node->is_deleted == 0)
		node->is_deleted = 1;

	//Always a success, no issues.
	return 0;
}

int delete_recursively(t_node* node)
{
	return preOrder(node,nary_deleteHelper);

}

/*
 * Helper method to change the path of source node and its children to dest node path
 * Will be called from within preOrder.
 */

int change_path(t_node *node) {

	char temp_path[MAX_PATH_SIZE];

	strcpy(temp_path, move_to_dest_path);
	//eg: if say node path is /home/def and node file = abcd and dest path is /usr/bin and source len = 5
	strcat(temp_path,node->data->path + source_path_len);

	//Now set the path
	strcpy(node->data->path,temp_path);

	return 0;
}

/*
 * Move the node at source_path and all its children to under the dest_path
 * Hence dest_path should be a directory. A totally diff implementation from what was given
 * in the free code.
 */
//todo : test this
int move_node(t_node* root,char* source_path,char* destination_path)
{
	t_node *source = NULL, *dest = NULL;
	//curr and prev are used to manipulate the links after moving
	t_node *current, *prev;

	//Check if the source and dest exist
	source = returnnode(root,source_path);

	if (source == NULL) {
		//Invalid path
		return -1;
	}

	dest = returnnode(root,destination_path);

	if (dest == NULL) {
		return -1;
	}

	//both source and dest are same
	if (source == dest) {
		return ERROR_SOURCE_DEST_SAME;
	}

	//check if there is already a node with the same file_name as source, under dest
	//Return error in that case.
	if (findNodeInSiblings(dest->left_child,source->data->file_name) != NULL) {
		return ERROR_SOURCE_EXISTS_UNDER_DEST;
	}

	//todo : check if the path is exceeding the max path, before moving the nodes.

	//populate the global variables, which will be used in the processNode
	//Store the len, so that we can change the path when it is moved
	source_path_len = strlen(source->data->path);

	move_to_dest_path = (char*)malloc(strlen(dest->data->path) + strlen(dest->data->file_name) + 2);//2 => '/' and '\0'
	strcpy(move_to_dest_path,dest->data->path);
	strcat(move_to_dest_path,"/");
	strcat(move_to_dest_path,dest->data->file_name);

	//Traverse the source in preOrder and change the path of all children to dest
	preOrder(root,change_path);

	//Now, Change the links so that the source is under dest

	current = source->parent->left_child;
	prev = NULL;
	while(current != source) {
		prev = current;
		current = current->right_siblings;
	}

	//Remove source from the link
	if (prev != NULL) {
		prev->right_siblings = source->right_siblings;
	} else {//implies source is the left child of its parent
		source->parent->left_child = source->right_siblings;
	}

	//Make source point to the new parent
	source->parent = dest;
	source->right_siblings = NULL;

	//Insert the source under dest node
	if (dest->left_child == NULL) {
		//case when dest has no child, poor thing
		dest->left_child = source;
	} else {
		//Insert at the end of all the children of dest
		current = dest->left_child;
		while(current->right_siblings != NULL) {
			current = current->right_siblings;
		}
		current->right_siblings = source;
	}

	free(move_to_dest_path);
	move_to_dest_path = NULL;

	return 0;

}

//This function addr is passed to inOrder as 2nd param
static int displayHelper(t_node *node){
	printf("Path= %s  File = %s isdelete = %d\n",node->data->path,node->data->file_name, node->is_deleted);
	return 0;
}

int print_tree(t_node* root){

	return preOrder(root,displayHelper);
}

//redundant method in spec
int printtree(t_node* root){
	return print_tree(root);
}

