/*
 * vfs_test_integ.c
 *
 *  Created on: Oct 24, 2009
 *      Author: Arun K
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "../src/vfs.h"
#include "../src/globals.h"

extern int size_of_header_1, max_fd, no_of_fd;
extern l_node *start;
extern t_node *root;
extern b_node *bst_root;
extern l_node *h_bucket[NO_OF_BUCKETS];

extern int size_of_header_2;
extern int current_fd_id;

#define MAX 1024*20

void display_data_structures (){
	l_node *head;

	printf("\n\n");
	printf("Print Nary\n");
	print_tree(root);

	printf("\nPrint BST\n");
	print_inorder(bst_root);

	printf("\nPrint HASH\n");
	hash_print(h_bucket);

	printf("\nPrint FREE LIST\n");
	head = start;
	while(head != NULL) {
		printf("%d ",*((int*)head->data));
		head=head->next;
	}
	printf("\n\n");
}

void display_meta_header(){
	printf("\nDislaying metaheader\n");
	printf("Head-1 = %d, head-2 = %d, max_fd=%d, no_fd=%d, current_fd=%d\n",size_of_header_1,size_of_header_2,max_fd,no_of_fd,current_fd_id);
	printf("\n");
}

int main(int argc, char **argv) {

	int ret, size, i;

	char *label;
	//A large data array for data
	char data_array[MAX+(2*1024)];//just allocate enuf space

	label = (char*)malloc(20);
	strcpy(label,"test-1");

	printf("Creating label %s, size = 20k\n",label);
	ret = create(label,25);
	printf("create return = %d\n",ret);

	ret = mount(label);
	printf("mount ret = %d\n",ret);

	display_meta_header();
	display_data_structures();

	printf("creating dir /abc\n");
	ret = create_dir("/","abc");
	printf("create dir ret = %d\n",ret);

	printf("creating dir /home\n");
	ret = create_dir("/","home");
	printf("create dir ret = %d\n",ret);

	printf("creating dir /root\n");
	ret = create_dir("/","root");
	printf("create dir ret = %d\n",ret);

	printf("creating dir /home/def\n");
	ret = create_dir("/home","def");
	printf("create dir ret = %d\n",ret);

	printf("creating dir /abc again...\n");
	ret = create_dir("/","abc");
	printf("create dir ret = %d\n",ret);

	display_meta_header();
	display_data_structures();

	printf("Creating some files....\n");

	printf("Create file /home/file1 size 1998\n");
	size = 21*1024;
	//write some dummy data
	for (i = 0; i < size; ++i) {
		if (i%2 == 0)
			data_array[i]='Z';
		else
			data_array[i]='Y';
	}
	data_array[i]='\0';
	ret = add_file("/home","file1",data_array);
	printf("add_file ret = %d\n",ret);


	printf("Create file /home/def/t-file size 4096\n");
	size = 4096;
	//write some dummy data
	for (i = 0; i < size; ++i) {
		if (i%2 == 0)
			data_array[i]='T';
		else
			data_array[i]='B';
	}
	data_array[i]='\0';
	ret = add_file("/home/def","t-file",data_array);
	printf("add_file ret = %d\n",ret);


	printf("Create file /abc/k-file size 500\n");
	size = 500;
	//write some dummy data
	for (i = 0; i < size; ++i) {
		if (i%2 == 0)
			data_array[i]='K';
		else
			data_array[i]='N';
	}
	data_array[i]='\0';
	ret = add_file("/abc","k-file",data_array);
	printf("add_file ret = %d\n",ret);


	printf("Create file /home/def/t-file again...\n");
	size = 4096;
	//write some dummy data
	for (i = 0; i < size; ++i) {
		if (i%2 == 0)
			data_array[i]='T';
		else
			data_array[i]='B';
	}
	data_array[i]='\0';
	ret = add_file("/home/def","t-file",data_array);
	printf("add_file ret = %d\n",ret);

	display_meta_header();
	display_data_structures();

	//Reset the data
	for (i = 0; i < MAX; ++i) {
		data_array[i]=1;
	}


	printf("Performing remove file operations...\n");

	printf("Remove file /home/file1\n");
	ret = remove_file("/home/file1");
	printf("remove_file ret = %d\n",ret);

	printf("Remove file /home/file1 again...\n");
	ret = remove_file("/home/file1");
	printf("remove_file ret = %d\n",ret);

	display_meta_header();
	display_data_structures();

	printf("Performing remove DIR operations...\n");

	printf("Remove dir /home\n");
	ret = remove_dir("/home");
	printf("remove_dir ret = %d\n",ret);

	printf("Re-create dir /home\n");
	ret = create_dir("/","home");
	printf("create dir ret = %d\n",ret);

	display_meta_header();
	display_data_structures();

	printf("Re create file /home/file1 size 2049\n");
	size = 2049;
	//write some dummy data
	for (i = 0; i < size; ++i) {
		if (i%2 == 0)
			data_array[i]='F';
		else
			data_array[i]='G';
	}
	data_array[i]='\0';

	ret = add_file("/home","file1",data_array);
	printf("add_file ret = %d\n",ret);

	printf("Perform the get file for the two files /home/file1 and /abc/k-file\n");
	size = get_file("/abc/k-file",data_array);
	if (size != 500)
		printf("ERROR in getting /abc/k-file\n");
	for (i = 0; i < size; ++i) {
		if (i%2 ==0 && data_array[i]!= 'K') {
			printf("ERROR in /abc/k-file\n");
			break;
		}else if(i%2 == 1 && data_array[i]!= 'N') {
			printf("ERROR in /abc/k-file\n");
			break;
		}
	}

	size = get_file("/home/file1",data_array);
	if (size != 2049)
		printf("ERROR in getting /home/file-1\n");
	for (i = 0; i < size; ++i) {
		if (i%2 ==0 && data_array[i]!= 'F') {
			printf("ERROR in /home/file-1\n");
			break;
		}else if(i%2 == 1 && data_array[i]!= 'G') {
			printf("ERROR in /home/file-1\n");
			break;
		}
	}
	printf("IF no error printed above, then get_file is a success\n");


	display_data_structures();

	printf("\nPerform list_files...\n");
	printf("\nFollowing are files of /\n");
	list_files("/");

	printf("\nFollowing are files of /root\n");
	list_files("/root");

	printf("\nFollowing are files of /home\n");
	list_files("/home");

	ret = unmount(label);
	printf("ret = %d\n",ret);

	display_meta_header();
	return 0;
}
