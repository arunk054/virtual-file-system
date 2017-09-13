/*
 * str_process.c
 *
 *  Created on: Oct 22, 2009
 *      Author: Arun K
 */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "string_tok.h"

/*
 * The tokenizer has been implemented similar to the split(regex, str.length) method
 * in String class in java. It does not remove any leading empty strings.
 * Hence /home/abc/ gives 4 tokens = "",home,abc,"".
 */
int get_tokens(char **token_list[], char *string, char delimitor) {
	//No. of tokens in the string
	int size = 0;
	//current max number of tokens, we will change this dynamically.
	//Start with 2 and it is doubled everytime after size reaches curMax.
	int curMax = 2;
	//This is to index each character in the string
	int i = 0;
	char ch;
	//Create a temp array to store the intermediate tokens
	char *tempToken;
	//This is to keep track of the individual token length
	int tokenLength = 0;
	//This is the one which is has the addr of array of strings
	//And we have to storethis addr in *token_list
	char **myTokenList;

	if (string == NULL || string[i] == '\0') {
		return -1;
	}

	tempToken = (char*)malloc((strlen(string)+1)*sizeof(char));

	//First allocate memory to store curMax=2 strings, then we will realloc
	//as the no. of tokens increases.
	myTokenList = (char**)malloc(curMax*sizeof(char*));
	i=0;size=0;tokenLength = 0;

	//Idea is to check each character and create a new string when we
	//encounter a delimitor. Break the look when we reach \0 character
	while (1){
		ch = string[i];
		if (ch == delimitor || ch == '\0') {

			tempToken[tokenLength] = '\0';
			//Our array has reached max, hence realloc
			if (size == curMax) {
				curMax = curMax * 2;
				myTokenList = (char**)realloc(myTokenList, curMax*sizeof(char*));
			}

			myTokenList[size] = (char*)malloc((tokenLength+1)*sizeof(char));
			//tempToken actually holds the curr token string
			strcpy(myTokenList[size],tempToken);
			//reset tokenLen and incr size of tokens
			tokenLength = 0;
			size++;
		} else {
			//We havent found a delimitor hence just populate the tempToken
			tempToken[tokenLength] = ch;
			tokenLength++;
		}
		if (ch == '\0')
			break;
		i++;
	}

	*token_list = myTokenList;
	return size;
}

/*
 * Extracts the filename from the fullpath, returns null if the
 */

char* get_filename( char *fullpath) {
	int len, i;
	char *retStr;

	//Some error checks for null empty string
	if (fullpath == NULL || fullpath[0] == '\0')
		return NULL;
	//not a valid path if it does not even begin with '/'
	if (fullpath[0] !='/')
		return NULL;

	len = strlen(fullpath);
	i = len-1;

	//if it ends with '/', we will return an empty string

	//Traverse the string in reverse till we find the '/'
	while(fullpath[i] != '/')
	{ i--; }

	retStr = (char*)malloc((len-i) * sizeof(char));
	strcpy(retStr, (fullpath+i+1));
	return retStr;
}

void free_tokens(char **tokens, int count){

	int i;

	if (tokens == NULL)
		return;
	for (i = 0; i < count; ++i) {
		free(tokens[i]);
	}
	free (tokens);
}
