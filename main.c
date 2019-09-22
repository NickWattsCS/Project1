// Robert Smith, Brian Thervil, Nick Watts
// Fall 2019
// COP4610 Operating Systems
// Project 1

#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct utilized for commands and parameters
typedef struct
{
  char** tokens;
  int numTokens;
} instruction;

// Declaring Functions
void loop();
void addToken(instruction* instr_ptr, char* tok);
void printTokens(instruction* instr_ptr);
void clearInstruction(instruction* instr_ptr);
void addNull(instruction* instr_ptr);
void shortcutRes(instruction* i_ptr);
void execInstruction(instruction* instr_ptr);
void execCommand(instruction * ptr);

// Main funcion that loops the process
int main()
{
  loop();
  
  return 0;
}

// Function that loops user input, parsing the input, then execute
// Code from parser_help.c
void loop()
{
  char* token = NULL;
  char* temp = NULL;
  
  instruction instr;
  instr.tokens = NULL;
  instr.numTokens = 0;
  
  while(1)
  {
    printf(getenv("USER"));
    printf("@");
    printf(getenv("MACHINE"));
    printf(" : ");
    printf(getenv("PWD"));
    printf(">");

  
    	do
    	{
    	  scanf("%ms", &token);
	  printf("caught token: %s\n", token);
    	  temp = (char*)malloc((strlen(token) + 1) * sizeof(char));
    	  int i;
     	  int start = 0;
      	  for (i = 0; i < strlen(token); i++)
      	  {
        	if (token[i] == '|' || token[i] == '>' || token[i] == '<' || token[i] == '&')
        	{
          		if (i-start > 0)
          		{
            		memcpy(temp, token + start, i - start);
            		temp[i - start] = '\0';
            		addToken(&instr, temp);
          		}
          
          		char specialChar[2];
          		specialChar[0] = token[i];
          		specialChar[1] = '\0';
          		addToken(&instr, specialChar);
          
          		start = i+1;
          	}
	  }

      		if (start < strlen(token))
      		{
        		memcpy(temp, token + start, strlen(token) - start);
        		temp[i - start] = '\0';
        		addToken(&instr, temp);
      		}
      	free(token);
      	free(temp);
      
      	token = NULL;
      	temp = NULL;
    	} while ('\n' != getchar());
  		addNull(&instr);
		printTokens(&instr);
		shortcutRes(&instr);

   		//execInstruction(&instr);	// Our code to execute commands after entry
		printTokens(&instr);
	    	clearInstruction(&instr);
    }
}

void addToken(instruction* instr_ptr, char* tok)
{
	//extend token array to accomodate an additional token
	if (instr_ptr->numTokens == 0)
		instr_ptr->tokens = (char**) malloc(sizeof(char*));
	else
		instr_ptr->tokens = (char**) realloc(instr_ptr->tokens, (instr_ptr->numTokens+1) * sizeof(char*));

	//allocate char array for new token in new slot
	instr_ptr->tokens[instr_ptr->numTokens] = (char *)malloc((strlen(tok)+1) * sizeof(char));
	strcpy(instr_ptr->tokens[instr_ptr->numTokens], tok);

	instr_ptr->numTokens++;
}

void addNull(instruction* instr_ptr)
{
	//extend token array to accomodate an additional token
	if (instr_ptr->numTokens == 0)
		instr_ptr->tokens = (char**)malloc(sizeof(char*));
	else
		instr_ptr->tokens = (char**)realloc(instr_ptr->tokens, (instr_ptr->numTokens+1) * sizeof(char*));

	instr_ptr->tokens[instr_ptr->numTokens] = (char*) NULL;
	instr_ptr->numTokens++;
}

void printTokens(instruction* instr_ptr)
{
	int i;
	printf("Tokens:\n");
	printf("size of array is %d\n" ,instr_ptr->numTokens);
	for (i = 0; i < instr_ptr->numTokens; i++) {
		if ((instr_ptr->tokens)[i] != NULL)
			printf("%s\n", (instr_ptr->tokens)[i]);
	}
}

void clearInstruction(instruction* instr_ptr)
{
	printf("Clearing");
	int i;
	for (i = 0; i < instr_ptr->numTokens; i++)
	{
		free(instr_ptr->tokens[i]);
		instr_ptr->tokens[i] = NULL;
	}

	free(instr_ptr->tokens);

	instr_ptr->tokens = NULL;
	instr_ptr->numTokens = 0;
}



void shortcutRes(instruction* i_ptr)
{
	char* look = NULL;
	char* fin_dir = NULL;
	char* buf_dir = NULL;
	char* append = NULL;
	char* par_dir = NULL;
	int k = 0, j = 0, y = 0;
	int i = 0;
	size_t first = 1;
	for (i; i < i_ptr->numTokens - 1; i++)
	{
		look = strchr((i_ptr->tokens)[i], '/');

		printf("Looking\n");		

		if (look != NULL && i_ptr->tokens[i][0] != '/')
		{
			printf("Look is not NULL\n");

			for (j; j < strlen(i_ptr->tokens[i]); j++)
			{
				printf("Entered the j for loop\n");

				if ((i_ptr->tokens)[i][j] == '.')
				{	
					printf("Entered the first . if-statement\n");

					if(i_ptr->tokens[i][j+1] == '.')
					{
						j++;
						int copy = 0, slash = 0;
						append = NULL;					//append, par_dir, and int variables 
						par_dir = NULL;					//made null
						//buf_dir = NULL;
						if (first == 1)					
							par_dir = getenv("PWD");		//if it's the first time through, the PWD
						else						//is stored in par_dir, if not then the current 
							par_dir = buf_dir;			//entry in buf_dir is stored

						printf("par_dir after first = %d : %s\n", first, par_dir);
						append = (char*) malloc (sizeof(char));
						append[copy] = '\0';
						if (first == 1)
							k = j + 2;
						else
							k = j + 1;
						for (k; i_ptr->tokens[i][k] != '\0'; k++)
						{
							append = realloc(append, (copy + 2) * sizeof(char*));
												//this copies what hasn't yet been scanned 
							append[copy] = i_ptr->tokens[i][k];	//and processed from i_ptr->tokens and places
							copy++;					//it into the append char*
						}
						append[copy] = '\0';

						printf("append: %s\n", append);

						for (y = 0; par_dir[y] != '\0'; y++)
						{						//This finds the location of the final
							if (par_dir[y] == '/')			//forward-slash in the code
								slash = y;
						}
						k = 0;
						

						/* This if statement prepares buf_dir to reconfigure its contents. If the next
						directory in i_ptr->tokens[i] is also a .. shortcut, then the contents of par_dir
						are inserted into buf_dir as they are. */

						if (append[0] == '.' || append[1] == '.')
						{
							printf("Next dir is a shortcut\n");
							if(first == 1)
								buf_dir = (char*) malloc((slash + 1) * sizeof(char));
							else
								buf_dir = (char*) realloc(buf_dir, (slash + 1) * sizeof(char));
							memcpy(buf_dir, par_dir, slash);
							buf_dir[slash+1] = '\0';
						}
						
						/* If the next directory is not a . or .. shortcut, then the next directory from 
						i_ptr->tokens[i] is copied into buf_dir */

						else
						{
							if (first == 1)
							{
								buf_dir = (char*) malloc((slash + 1) * sizeof(char));
								memcpy(buf_dir, par_dir, slash + 1);
								buf_dir[++slash] = '\0';
							}
							printf("%s + %s = ", buf_dir, append);
							copy = slash;
							for(k = 0; append[k] != '\0'; k++)
							{
								buf_dir = (char*) realloc(buf_dir, (copy + 3) * sizeof(char));
								buf_dir[copy] = append[k];
								buf_dir[++copy] = '\0';
								j++;
								if (append[k+1] == '/')
									break;
								printf("%s\n", buf_dir);
							}
						}
						if (strcmp(par_dir, getenv("PWD")) == 0)
							first = 2;
						printf("buf_dir after processing: %s\n", buf_dir);
					}
					else
					{
						//This runs when attaching a pwd shortcut to the extending buffer directory
						if (j==0)
						{
							printf("Adding . shortcut res\n");
							par_dir = getenv("PWD");
							buf_dir = (char*) malloc(sizeof(char));
							for(y = 0; par_dir[y] != '\0'; y++)
							{
								buf_dir = (char*) realloc(buf_dir, (y + 2) * sizeof(char));
								buf_dir[y] = par_dir[y];
								buf_dir[y+1] = '\0';
							}		
						}		
					}
				}
				else if((i_ptr->tokens)[i][0] == '~')
				{
				}
				else if(i_ptr->tokens[i][j] == '~' && j != 0)
				{
					printf("Not valid directory: %s", (i_ptr->tokens)[i]);	
				}
			}
		}
		else if (look != NULL && i_ptr->tokens[i][j] == '/')
		{
			int copy = j - 1;
			int end = strlen(buf_dir);
			while(i_ptr->tokens[i][++copy] != '\0' && i_ptr->tokens[i][copy] != '/')
			{
				buf_dir = (char*) realloc(buf_dir, (end + 2) * sizeof(char));
				buf_dir[end] = i_ptr->tokens[i][copy];
				buf_dir[++end] = '\0';
			}
		}
		else if (look == NULL && strcmp((i_ptr->tokens)[i], "..") == 0)
		{
			char* temper = NULL;
			char* par_dir = getenv("PWD");
			int x = 0, last_sl = 0;
			for (x; x < strlen(par_dir); x++)
				if(par_dir[x] == '/')
					last_sl = x;
			temper = (char*) malloc(last_sl+1 * sizeof(char));
			memcpy(temper, par_dir, last_sl);
			temper[last_sl] = '\0';
			(i_ptr->tokens)[i] = (char*) realloc ((i_ptr->tokens)[i], strlen(temper) * sizeof(char));
			i_ptr->tokens[i] = temper;
			printf("%s\n", i_ptr->tokens[i]);
			free(temper);
		}
		else if (look == NULL && strcmp((i_ptr->tokens)[i], ".") == 0)
		{
		}
		else if (look == NULL && strcmp((i_ptr->tokens)[i], "~")  == 0)
		{
		}

		if (i_ptr->tokens[i][j+1] == '\0')
		{
			i_ptr->tokens[i] = (char*) realloc (i_ptr->tokens[i], (strlen(buf_dir)+1 * sizeof(char)));
			memcpy(i_ptr->tokens[i], buf_dir, strlen(buf_dir));
			i_ptr->tokens[i][strlen(buf_dir)] = '\0';
		}

		printf("FREE FIN_DIR AND APPEND\n");
		printf("Final path: %s\n", i_ptr->tokens[i]);
		free(fin_dir);
		free(append);
		free(buf_dir);
	}
}


// Function Block that handles the execution of commands
void execInstruction(instruction* instr_ptr)
{
	instruction command;	// Created an incstruction object
	command.tokens = NULL;	// which will hold individual commands
	command.numTokens = 0;	// before being cleared every | or NULL

	int i = 0;
	
	for(i = 0; i < instr_ptr->numTokens - 1; i++)	// For loop to go through the whole line of commands
	{	
		char bin[]="/bin/"; 
		if (command.numTokens == 0)	// If individual instruction is empty, then adds address of commands before being added as token
		{
			strcat(bin, instr_ptr->tokens[i]);
			addToken(&command,bin);
		}
		else if ((char)instr_ptr->tokens[i][0] != (char)'|')	// Collects arguments before | or end of whole instruction
		{
			addToken(&command, instr_ptr->tokens[i]);
		}
		else	// Executes individual command if separator cones through and resets command for next group
		{
			addNull(&command);
			execCommand(&command);
			clearInstruction(&command);
		}
	}

	if (command.numTokens != 0)	// Executes last command
	{
		addNull(&command);
		execCommand(&command);
		clearInstruction(&command);
	}
}

// Function to execute a set of commands
void execCommand(instruction * ptr)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == 0)
	{
		if (execv(ptr->tokens[0], ptr->tokens) == -1)
		{
			perror("Invalid Command");
		}
	}
	else if (pid < 0)
	{
		perror("Error");
	}
	else
	{
		waitpid(pid, &status, 0);
	}
}
