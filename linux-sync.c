#include <time.h>
#include <stdio.h>
#include <stdlib.h>

/*
    Name : linux-sync
    Author : Noe Flatreaud
    Course : Operating Systems Course
    Description : Write a program in Linux OS which, by using the means of synchronization, 
    two processes write and read from the same resource (ex. a file). See the following link as an example
    https://thysmichels.com/2012/01/31/process-synchronization-in-linux/
*/

void write_str(pid_t, char*, char*); // write string to file in "w" mode, wipes the old content
void append_str(pid_t, char*, char*); // write string to file in "a" mode, keeps the old content

char* to_string(char*); // reads the content of a file then returns it's string content

int main(int argc, char* argv[])
{
    // ./linux-sync <file>
    if(argc>1)
    {
        char* filename = argv[1];           // get filename
        char* str = to_string(filename);    // get content

        pid_t pid;                          // create new PID

        printf(" -- %s -- %s --\n\n", filename, str); // display content

        pid = fork();                       // fork the process into multiple branches (parent + childs)

        // forking the process
        //
        // A    << parent
        // ^
        // A*   << child
        //

        if(pid < 0)                         // should never happen, if so, then error
        {
           perror("Fork Error");
           exit(1);
        }
        else if(pid == 0)                   // if we are the first row, then we are child
        {
            // child
            printf("[%d] SYNC-LOG : my name is 'child' (%d)\n", pid, time(NULL));
            append_str(pid, filename, "my name is 'child'");
        }
        else                                // else we are parent
        {
            // parent
            wait(0);
            printf("[%d] SYNC-LOG : my name is 'parent' and I wait for child (%d)\n", pid, time(NULL));
            append_str(pid, filename, "my name is 'parent' and I wait for child");
        }

	free(str);
    }
    else
    {
        fprintf(stderr, "Too few arguments ! usage : ./linux-sync <filename>\n");
        exit(1);
    }

    return 0;
}

void write_str(pid_t pid, char* filename, char* buff)
{
    FILE* f = fopen(filename, "w");                         // fopen in w mode, wipes the old content
    fprintf(f, "[%d] %s (%d)\n", pid, buff, time(NULL));    // print into file using format.
    fclose(f);                                              // close file
}
void append_str(pid_t pid, char* filename, char* buff)
{
    FILE* f = fopen(filename, "a");                         // fopen in a mode, keeps the old content
    fprintf(f, "[%d] %s (%d)\n", pid, buff, time(NULL));    // print into file using format
    fclose(f);                                              // close file
}

// get string content from a filename
char* to_string(char* filename)
{
    char* str;
    size_t length;

    FILE* file;

    file = fopen(filename, "r");      // open file in r mode
    fseek(file, 0, SEEK_END);         // seek for end
    length = ftell(file);             // get length
    fseek(file, 0, SEEK_SET);         // returns back to first
    str = malloc(length + 1);         // malloc using length+1 (end with \0)
    if(str)
    {
        int i=0;
        while(!feof(file))              // while not end of file
        {
            str[i++] = fgetc(file);     // print file char into str
        }   
        str[i] = '\0';                  // end with \0
    }
    fclose(file);                       // close file

    return str;
}
