/*
 * vfs_test_nary.c
 *
 *  Created on: Oct 23, 2009
 *      Author: Arun K
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "../src/narytree.h"
#include "../src/string_tok.h"

fd* getData(char *path, char *file, int isDir){
    fd* dir = (fd*)malloc(sizeof(fd));

    dir->is_dir=isDir;
    dir->fd_id=1;
    dir->size=10;
    strcpy(dir->file_name,file);
    strcpy(dir->path,path);
    dir->blocks[0]=12;
    return dir;

}
int main(int argc, char **argv) {
	t_node *root, *temp;
	fd *dir, *fd1;
	int ret;

	int i,j;
	char **list;

	//Testing gettokens:
	printf("testing get tokens\n");
	j = get_tokens(&list,"/home/world",'/');
	printf("tokens of /home/world are:\n");
	printf("tokens = %d\n",j);
	for (i = 0; i < j; ++i) {
		printf("%s\n",list[i]);
	}
	free(list);


	j = get_tokens(&list,"/home/world/foo/",'/');
	printf("tokens of /home/world/foo/ are:\n");
	printf("tokens = %d\n",j);
	for (i = 0; i < j; ++i) {
		printf("%s\n",list[i]);
	}
	free(list);

	j = get_tokens(&list,"/",'/');
	printf("tokens of / are:\n");
	printf("tokens = %d\n",j);
	for (i = 0; i < j; ++i) {
		printf("%s\n",list[i]);
	}
	free(list);

	printf("testing get Filename...\n");
	printf("filename for /home/world is : %s\n", get_filename("/home/world"));
	printf("filename for /home/world/foo/ is : %s\n", get_filename("/home/world/foo/"));
	printf("filename for / is : %s\n", get_filename("/"));

	//============Following is for testing nary============
	root = NULL;

	dir = getData("/","/as",1);
	ret = insert_node(&root,dir);
	printf("ret = %d\n",ret);

	dir = getData("/","/",1);
	ret = insert_node(&root,dir);
	printf("ret = %d\n",ret);

	dir = getData("/","home",1);
	ret = insert_node(&root,dir);
	printf("ret = %d\n",ret);

	dir = getData("/","usr",1);
	ret = insert_node(&root,dir);
	printf("ret = %d\n",ret);

	dir = getData("/usr","bin",1);
	ret = insert_node(&root,dir);
	printf("ret = %d\n",ret);

	dir = getData("/usr/bin","abc",1);
	ret = insert_node(&root,dir);
	printf("ret = %d\n",ret);

	dir = getData("/usr/bin","bin1",1);
	ret = insert_node(&root,dir);
	printf("ret = %d\n",ret);

	dir = getData("/usr/bin","bin2",1);
	ret = insert_node(&root,dir);
	printf("ret = %d\n",ret);

	dir = getData("/home","def",1);
	ret = insert_node(&root,dir);
	printf("ret = %d\n",ret);

	dir = getData("/home","cp",1);
	ret = insert_node(&root,dir);
	printf("ret = %d\n",ret);

	dir = getData("/home/def","ghi",1);
	ret = insert_node(&root,dir);
	printf("ret = %d\n",ret);

	dir = getData("/home/def","klm",1);
	ret = insert_node(&root,dir);
	printf("ret = %d\n",ret);

	dir = getData("/home/def","mno",1);
	ret = insert_node(&root,dir);
	printf("ret = %d\n",ret);

	dir = getData("/usr/bin","bin3",1);
	ret = insert_node(&root,dir);
	printf("ret = %d\n",ret);

	delete_node(&root,"/home/def/mno");
	delete_node(&root,"/usr");

	//Intrersting, trying to create the root again
	dir = getData("/","/",1);
	ret = insert_node(&root,dir);
	printf("ret = %d\n",ret);

	fd1 = get_node(&root,"///home////def/");
	printf("fd1 = %s %s\n", fd1->path, fd1->file_name);


	print_tree(root);


	free(dir);
	return 0;
}
