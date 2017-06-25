/*
 * tokenizer.c
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct TokenizerT_
{
	char *separate; /* under the assumption that the user input will be in characters.  */
	char *steam;
};

typedef struct TokenizerT_ TokenizerT;

/*
 * TKCreate creates a new TokenizerT object for a given set of separator
 * characters (given as a string) and a token stream (given as a string).
 *
 * TKCreate should copy the two arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */


TokenizerT *TKCreate(char *separators, char *ts)
{

	TokenizerT *token = (TokenizerT *)malloc(sizeof(TokenizerT));
	token->separate = malloc(strlen(separators)+1); /* 1 is added due to a null byte character */
	strcpy(token->separate,separators); /* copy the arguments */

	token->steam = malloc(strlen(ts)+1); /* 1 is added for the null byte character as well */
	strcpy(token ->steam,ts); /* copy the arguments */

	if (strlen(ts) == 0)		/* if the length of ts is 0 (has no arguments) then it should return NULL, as there is nothing to be done. */
	{
		return NULL;
	}

	return token;

}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy(TokenizerT *tk) /* usage of free function is needed */
{
	free(tk->separate); /* Deallocates memory previously allocated by malloc. */
	free(tk->steam);
	free(tk);
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken(TokenizerT *tk)
{
	int dlLength, sLength;
	sLength = strlen (tk->steam);
	dlLength = strlen(tk->separate);

	char *token2 = (char *)calloc(sLength + 1, sizeof(char));

	if (dlLength == 0)	 /* if there are currently no delimiters, all that you need to do is copy the token steam into token. */
	{
		strcpy(token2, tk->steam);
		*(tk->steam) = '\0';  /* "delimited by '\0' */
		return token2;
	}


	int characterstoadd;
	characterstoadd = 0;
	int y;

	while (*(tk->steam) != '\0')
	{
		for (y=0; y<dlLength; y++)
		{
			if (*(tk->steam) == '\0')  /* If no steam to process, (delimiter reached) break. */
				break;
			if (*(tk->steam) != tk->separate[y])
			{
				if (y == dlLength - 1)	/* End of the delimiter string.*/
				{
					token2[characterstoadd++] = *(tk->steam);
					tk->steam += 1;
				}
			}
			else
			{
				tk->steam += 1;
				if (characterstoadd)
				{
					token2[characterstoadd + 1] = '\0';
					token2 = (char *) realloc(token2, characterstoadd + 1);  /* realloc token with characterstoadd + 1 for the null byte. */
					return token2;
				}
				y = -1;  /* We need to look at the beginning of delimiters, as it will loop increment from y to 0. */
			}
		}
	}
	return token2;
}

/*
 * main will have two string arguments (in argv[1] and argv[2]).
 * The first string contains the separator characters.
 * The second string contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv)
{
	if (argc > 3)
	{
		printf("Error: Too Many Arguments.\n");
		return 1;
	}
	else if (argc < 3)
	{
		printf("Error: Two Few Arguments.\n");
		return 1;
	}

	if (strcmp(argv[2], "") == 0 )
		{
			return 1;
		}

	TokenizerT *tokenizer;
	tokenizer = TKCreate(argv[1], argv[2]); /* two string arguments, one for characters, and another for tokens. */
	char *Original;
	Original = tokenizer->steam;
	char *token;
	int x, y;

	if (tokenizer == NULL)
	{
		return 1;
	}


	while (*(tokenizer->steam) != '\0')
	{
		token = TKGetNextToken(tokenizer);
		if (*(token) == '\0')  /* if all characters match delimiters, there just end. */
		{
			free (token);
			break;
		}

		for (x=0, y='0'; y !='\0'; x++)
		{
			y = token[x];
			if (y == '\\')
			{
				if (token [x+1] == '\0')
					break;
				switch (token[x+1])  /*printing tokens*/
				{
				case 'n': /* newline case */
					printf("[0x0a}");
					++x;
					break;
				case 't': /* horizontal tab case */
				    printf("[0x09]");
					++x;
				    break;
				case 'v': /* vertical tab case */
					printf("[0x0b]");
					++x;
					break;
                case 'b': /*backspace case */
                    printf("[0x08]");
					++x;
                    break;
                case 'r': /*carriage return case */
                    printf("[0x0d]");
					++x;
                    break;
                case 'a': /*audible alert case */
                    printf("[0x07]");
					++x;
                    break;
                case 'f': /* form feed case */
                    printf("[0x0c]");
					++x;
                    break;
                case '\\': /*backslash case */
                    printf("[0x5c]");
					++x;
                    break;
                case '\"': /*double quote case */
                    printf("[0x22]");
                	++x;
                    break;
				default:
					continue;
				}
			}
			else
				putchar(y);
		}
		putchar('\n');
		free(token);
	}
	tokenizer->steam = Original;
	TKDestroy(tokenizer); /* set the steam back to its original place so destroy can take place. */
	return 0;
}


