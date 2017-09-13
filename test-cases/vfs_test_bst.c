/*
 * vfs_test_bst.c
 *
 *  Created on: Oct 21, 2009
 *      Author: Arun K
 */

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/VFS_BST.h"

int main()
{
    b_node *root;
    root=bst_create();

    fd* dir1 = (fd*)malloc(sizeof(fd));
    dir1->is_dir=1;
    dir1->fd_id=1;
    dir1->size=10;
    strcpy(dir1->file_name,"BIN.txt");
    strcpy(dir1->path,"/bin/");
    dir1->blocks[0]=12;

    fd* dir4 = (fd*)malloc(sizeof(fd));
    dir4->is_dir=1;
    dir4->fd_id=1;
    dir4->size=10;
    strcpy(dir4->file_name,"CIN.txt");
    strcpy(dir4->path,"/bin/");
    dir4->blocks[0]=12;

    fd* dir5 = (fd*)malloc(sizeof(fd));
    dir5->is_dir=1;
    dir5->fd_id=1;
    dir5->size=10;
    strcpy(dir5->file_name,"YFS.txt");
    strcpy(dir5->path,"/usr/");
    dir5->blocks[0]=12;

    int return_insert=bst_insert ( &root,dir5);
    return_insert=bst_insert ( &root,dir1);
    return_insert=bst_insert ( &root,dir4);
    return_insert=bst_insert ( &root,dir1);
    printf("return value for dup key = %d\n", return_insert);

    b_node *found =bst_search( root, "/usr/YFS.txt");
    if(found != NULL)
        printf("\nFilname :%s\n", found->full_path);

    print_inorder(root);
    int return_delete=bst_delete(&root, "/usr/YFS.txt");
    printf("\nReturn Value for successfull removal = %d\n", return_delete);

    return_delete=bst_delete(&root, "/usr/UFS.txt");
        printf("\nReturn Value for failure in removal = %d\n", return_delete);
}
