// C program named mainreturn.c to demonstrate the working
// of command line arguement
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include "utils.h"
#include <errno.h>

void loop();
void get_user_input();
void parse_command(char *);
int execute_command(char **);
char line_in[200];
char line_out[200];
FILE *fp;
int history = 0;
// defining main with arguments
int main(int argc, char* argv[])
{
    
    if (argc > 1)
    {
	    fprintf(stderr, "error\n");
	    exit(1);
    }
    	
	fp = fopen(".421sh", "w+");
	if (fp == NULL){
		printf("Error creating file\n");
		exit(EXIT_FAILURE);
	}
    loop();
    return 0;
}

/*void file_reader(char **argv)
{
	// 1. create a file in write mode
	while (fgets(line, 40, fp) != NULL)
	{
		i++;
		fputs(line,60,fp);
		if (i == 10)
			fseek(file, 0, SEEK_SET);
	}
}*/
int execute_command(char **argv)
{
	char temp[200];
	pid_t pid;
	sprintf(temp, "%s%s", "/usr/bin:/bin", argv[0]);
	char *path = getenv("PATH");
	char pathenv[strlen(path) + sizeof("PATH=")];
	sprintf(pathenv, "PATH=%s", path);
	char *envp[] = {pathenv, NULL};
	pid = fork();
	if (pid < 0)
	{
		return -1;
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		char c; int i = 0;
		//int num = execvpe(argv[0], argv, envp);
		//int num = execve(temp, argv, NULL);

		fclose(fp);
		fp = fopen(".421sh", "r");	
		if (strcmp(argv[0],"history")== 0){
			while ((c = fgetc(fp)) != EOF) {
        			//printf("%c", c);
				putchar(c);
				//fputc(c, stdout_copy);
    			}
			fclose(fp);
		//	exit(0);
		}
		else
		{
		int num = execvp(argv[0], argv);
		if (num == -1)
		{	
			perror("Error");
			exit(EXIT_FAILURE);	
		}
		}	
	}
	else
	{
		wait(NULL);
		free(argv);
	}	
}

void parse_command(char * to_be_parsed)
{
	int i = 0; char **argu;
	char *cpy = strdup(to_be_parsed);
	char *token = strtok(to_be_parsed, " ");
	while(token)
	{
		token = strtok(NULL," ");
		i++;
	}
	//i = 0;
	argu = malloc(sizeof(char *) * i + 8);
	free(to_be_parsed);
	i = 0;
	token = strtok(cpy, " ");	
	while(token)
	{
		argu[i] = token;
		token = strtok(NULL," ");
		i++;
	}
	argu[i] = NULL;
	if ((strcmp(argu[0],"exit") == 0))
	{		
		free(cpy); free(argu); //free(token);
		exit(0);
	}
	//else if(strcmp(argu[0], "cat") == 0)
	//	execute_proc_command(argu);
	else{
	execute_command(argu);	
	free(cpy); //free(argu); //free(token);
}
}

void get_user_input()
{
	if (errno == EINVAL)
		printf("Invalid Arguments");
	int initial_len = 10;
	char ch; int tin = 0;
	FILE *keyb = stdin;
	char *commands = (char *)malloc(initial_len*sizeof(char));
	if (keyb == NULL)
	{
		printf("Error reading from keyboard");
		exit(1);
	}
	
	while(( ch = fgetc(keyb)) != '\n')
	
	{
		line_out[tin] = ch; // here is our history buffer
		commands[tin] = ch;
		tin++;
		if (tin > initial_len)
		{
			initial_len = initial_len * 4;
			commands = (char *)realloc(commands, initial_len*sizeof(char));
		}
			
	}
	line_out[tin] = '\n';
	history++;
	fprintf(fp,line_out);
	line_out[0] = '\0';
	commands[tin] = '\0';
	tin = 0; int check = 0;line_out[0] = '\0';
	for(int z = 0; z < strlen(commands); z++)
	{
		if (ispunct(commands[z]) || isalnum(commands[z])) ++check;
	}
	if (strlen(commands) == 0 || check == 0)
		goto bypass;
        parse_command(commands);
	bypass:		
	{
		//free(commands);
		initial_len = 10;
		//commands = (char *)malloc(initial_len*sizeof(char));
	}
	//free(commands);
	//initial_len = 10;
	//commands = (char *)malloc(initial_len*sizeof(char));
		
	

}


void loop()
{
	while(1){
		printf("$ ");
		get_user_input();	
	}	
}
