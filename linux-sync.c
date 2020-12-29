#include <time.h>
#include <stdio.h>
#include <stdlib.h>


void write_str(pid_t, char*, char*);
void append_str(pid_t, char*, char*);

char* to_string();

int main(int argc, char* argv[])
{
    if(argc>1)
    {
        char* filename = argv[1];
        char* str = to_string(filename);
        
        pid_t pid;

        printf(" -- %s -- %s --\n\n", filename, str); // display content

        pid = fork();

        if(pid < 0)
        {
           perror("Fork Error");
           exit(1); 
        }
        else if(pid == 0)
        {
            // child
            printf("[%d] SYNC-LOG : my name is 'child' (%d)\n", pid, time(NULL));
            append_str(pid, filename, "my name is 'child'");
        }
        else
        {
            // parent
            wait(0);
            printf("[%d] SYNC-LOG : my name is 'parent' and I wait for child (%d)\n", pid, time(NULL));
            append_str(pid, filename, "my name is 'parent' and I wait for child");
        }
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
    FILE* f = fopen(filename, "w");
    fprintf(f, "[%d] %s (%d)\n", pid, buff, time(NULL));
    fclose(f);
}
void append_str(pid_t pid, char* filename, char* buff)
{
    FILE* f = fopen(filename, "a");
    fprintf(f, "[%d] %s (%d)\n", pid, buff, time(NULL));
    fclose(f);
}

char* to_string(char* filename)
{
    char* str;
    size_t length;

    FILE* file;

    file = fopen(filename, "r");
    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);    
    str = malloc(length + 1);
    if(str)
    {
        int i=0;
        while(!feof(file))
        {
            str[i++] = fgetc(file);
        }
        str[i] = '\0';
    }
    fclose(file);

    return str;
}