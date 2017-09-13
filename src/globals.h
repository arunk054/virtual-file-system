/*
 * globals.h
 *
 *  Created on: Oct 27,Sep 2009
 *      Author: Arun k
 */


#ifndef STRUCTS_H
#define STRUCTS_H

#define MAX_PATH_SIZE 100
#define MAX_FILEDIR_NAME_SIZE 10
#define MAX_BLOCKS 20
#define BLOCK_SIZE 1024
#define MAX_FILENAME_SIZE 10

typedef struct file_descriptor
{
	int is_dir;
	int fd_id;
	int size;
	char file_name[MAX_FILEDIR_NAME_SIZE];
	char path[MAX_PATH_SIZE];
	int blocks[MAX_BLOCKS];
}fd;

typedef struct hash_node
{
	int fd_id;
	char file_name[MAX_FILEDIR_NAME_SIZE];
	char path[MAX_PATH_SIZE];
	fd *file_desc;
}h_node;

typedef struct list_node{
	void *data;
	struct list_node *next;
}l_node;


#endif

