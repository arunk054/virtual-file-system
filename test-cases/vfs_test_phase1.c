// You could use this sample code to test your C functions

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../src/llist.h"
#include "../src/hash.h"

static char* get_string_from_void(void *data, char type) {
	switch (type) {
		case 'h':
			return ((h_node *)data)->file_name;
		case 'f':
			return ((fd *)data)->file_name;
		default:
			return ((char *)data);
	}
}

static void print_list (l_node *head, char type) {
	printf("The following are the contents of the list:\n");
	while(head != NULL){
		printf("%s\n",get_string_from_void(head->data, type));
		head=head->next;
	}

	printf("==============================\n\n");

}

int test_linked_list() {
	l_node *head = NULL;

	fd* dir1 = (fd*)malloc(sizeof(fd));
  	dir1->is_dir=1;
  	dir1->fd_id=1;
  	dir1->size=10;
  	strcpy(dir1->file_name,"abc");
  	strcpy(dir1->path,"/");
  	dir1->blocks[0]=12;
  	h_node *d1 = (h_node*)malloc(sizeof(h_node));
  	d1->file_desc = dir1;
  	strcpy(d1->file_name,dir1->file_name);
  	strcpy(d1->path,dir1->path);

  	fd* dir2 = (fd*)malloc(sizeof(fd));
  	dir2->is_dir=1;
  	dir2->fd_id=2;
  	dir2->size=10;
  	strcpy(dir2->file_name,"bcd");
  	strcpy(dir2->path,"/abc/");
  	dir2->blocks[0]=12;
  	h_node *d2 = (h_node*)malloc(sizeof(h_node));
  	d2->file_desc = dir2;
  	strcpy(d2->file_name ,dir2->file_name);
  	strcpy(d2->path,dir2->path);

  	fd* dir3 = (fd*)malloc(sizeof(fd));
  	dir3->is_dir=1;
  	dir3->fd_id=3;
  	dir3->size=10;
  	strcpy(dir3->file_name,"aa");
  	strcpy(dir3->path,"/dir/");
  	dir3->blocks[0]=12;
  	h_node *d3 = (h_node*)malloc(sizeof(h_node));
  	d3->file_desc = dir3;
  	strcpy(d3->file_name,dir3->file_name);
  	strcpy(d3->path,dir3->path);

  	fd* dir4 = (fd*)malloc(sizeof(fd));
  	dir4->is_dir=1;
  	dir4->fd_id=3;
  	dir4->size=10;
  	strcpy(dir4->file_name,"123");
  	strcpy(dir4->path,"/abc/");
  	dir4->blocks[0]=12;
  	h_node *d4 = (h_node*)malloc(sizeof(h_node));
  	d4->file_desc = dir4;
  	strcpy(d4->file_name,dir4->file_name);
  	strcpy(d4->path,dir4->path);


  	fd* dir5 = (fd*)malloc(sizeof(fd));
  	dir5->is_dir=0;
  	dir5->fd_id=4;
  	dir5->size=10;
  	strcpy(dir5->file_name,"123");
  	strcpy(dir5->path,"/dir/");
  	dir5->blocks[0]=12;
  	h_node *d5 = (h_node*)malloc(sizeof(h_node));
  	d5->file_desc = dir5;
  	strcpy(d5->file_name,dir5->file_name);
  	strcpy(d5->path,dir5->path);

  	head=list_insert_head(head,d1);
  	head=list_insert_head(head,d2);
  	head=list_insert_head(head,d3);
  	head=list_insert_sortorder(head,d4, d4->file_name);
  	head=list_insert_sortorder(head,d5, d5->file_name);

	print_list(head, 'h');

	head = list_delete_head(head);

	print_list(head, 'h');

	head = list_delete_node(head, d4);

	print_list(head, 'h');
	//Now test the get data list
	printf ("Testing get data list...\n");

	h_node *h_arr[10];
	int i;
	for (i = 0; i < 10; ++i) {
	  	h_arr[i] = (h_node*)malloc(sizeof(h_node));
	  	h_arr[i]->file_desc = dir5;
	  	strcpy(h_arr[i]->file_name,dir5->file_name);
	  	strcpy(h_arr[i]->path,dir5->path);
	  	head = list_insert_head(head,h_arr[i]);
	}

	print_list(head,'h');

	h_node **h_data;
	int count = list_get_data_list(head,"123",&h_data);
	for (i = 0; i < count; ++i) {
		printf("path = %s file=%s\n",h_data[i]->path, h_data[i]->file_name);
	}


	return 0;


}

int test_hash_table() {
	l_node *h_bucket[NO_OF_BUCKETS];
	fd **list;
	hash_initialize(h_bucket);

	fd* dir1 = (fd*)malloc(sizeof(fd));
  	dir1->is_dir=1;
  	dir1->fd_id=1;
  	dir1->size=10;
  	strcpy(dir1->file_name,"abc");
  	strcpy(dir1->path,"/");
  	dir1->blocks[0]=12;
  	h_node *start;

  	fd* dir2 = (fd*)malloc(sizeof(fd));
  	dir2->is_dir=1;
  	dir2->fd_id=2;
  	dir2->size=10;
  	strcpy(dir2->file_name,"bcd");
  	strcpy(dir2->path,"/abc/");
  	dir2->blocks[0]=12;

  	fd* dir3 = (fd*)malloc(sizeof(fd));
  	dir3->is_dir=1;
  	dir3->fd_id=3;
  	dir3->size=10;
  	strcpy(dir3->file_name,"a");
  	strcpy(dir3->path,"/ss/");
  	dir3->blocks[0]=12;

  	fd* dir4 = (fd*)malloc(sizeof(fd));
  	dir4->is_dir=1;
  	dir4->fd_id=3;
  	dir4->size=10;
  	strcpy(dir4->file_name,"a");
  	strcpy(dir4->path,"/dir/");
  	dir4->blocks[0]=12;


  	fd* dir5 = (fd*)malloc(sizeof(fd));
  	dir5->is_dir=0;
  	dir5->fd_id=4;
  	dir5->size=10;
  	strcpy(dir5->file_name,"bbc");
  	strcpy(dir5->path,"/dir/");
  	dir5->blocks[0]=12;

  	fd* dir6 = (fd*)malloc(sizeof(fd));
  	dir6->is_dir=0;
  	dir6->fd_id=4;
  	dir6->size=10;
  	strcpy(dir6->file_name,"321");
  	strcpy(dir6->path,"/loop/abc/");
  	dir6->blocks[0]=12;

	int first=hash_insert(h_bucket,dir1);
	first=hash_insert(h_bucket,dir2);
	first=hash_insert(h_bucket,dir3);
	first=hash_insert(h_bucket,dir4);
 	first=hash_insert(h_bucket,dir5);
 	first=hash_insert(h_bucket,dir6);

	hash_print(h_bucket);

	int count = hash_search(h_bucket,"321",&list);

	int i;
	printf("count = %d\n",count);

	for (i = 0; i < count; ++i) {
		printf("path: %s\n",list[i]->path);
	}

	int delete_check=hash_delete(h_bucket,"/sas/","po");
	printf("\nOutput of Delete :%d\n" , delete_check);
	hash_print(h_bucket);
	delete_check=hash_delete(h_bucket,"/","abc");

	printf("\nOutput of Delete :%d\n" , delete_check);
	hash_print(h_bucket);

	return 0;
}

int main()
{
	test_linked_list();
	test_hash_table();
	return 0;
}
