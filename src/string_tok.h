/*
 * string_tok.h
 *
 *  Created on: Oct 22, 2009
 *      Author: Arun K
 */

#ifndef STRING_TOK_H
#define STRING_TOK_H

//The function returns the count of components. The count could be initialized to -1 to indicate error condition.
/* returns the tokens in token_list[] found in the input string */
int get_tokens(char **token_list[], char *string, char delimitor);

void free_tokens(char **tokens, int count);

char * get_filename( char *fullpath);

#endif /* STRING_TOK_H*/

