#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

void
list_parent()
{
    char* args[] = {"ls", "-l", "../", NULL};
        
    execvp("ls", args);
}

int
write_file(char* string)
{
    int fd;
    struct stat fs;
    
    fd = open("hello.txt", O_RDWR);
    if (fd < 0)
    {
        printf("Could not open the file\n");
        return -1;
    }
    
    if (0 != fstat(fd, &fs))
    {
        goto error;
    }
    
    printf("File size: %d\n", (int)fs.st_size);
    
    if (fs.st_size > 1024)
    {
        fprintf(stderr, "%s", string);
        goto error;
    }
    
error:
    close(fd);
}

int main(int argc, char** argv)
{
    int option;
    
    while ( (option = getopt(argc, argv, "lf:p")) != -1)
    {
        switch(option)
        {
            case '?':
            {
                printf("Usage: lab08_01 [option]\n");
                printf("OPTIONS\n");
                printf("\t-l List parent diretory\n");
                return -1;
            }
            
            case 'l':
            {
                list_parent();
                break;
            }
            case 'f':
            {
                write_file(optarg);
                break;
            }
            case 'p':
            {
                printf("uid: %d euid: %d\n", getuid(), geteuid());
                break;
            }
        }
    }
}