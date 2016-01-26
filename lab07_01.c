#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

const uid_t my_user_uid = 1000;

void print_uids()
{
    uid_t ruid;
    uid_t euid;
    
    ruid = getuid();
    euid = geteuid();
    
    printf("ruid=%i euid=%i ", ruid, euid);
}

void print_gids()
{
    gid_t rgid;
    gid_t egid;
    
    rgid = getgid();
    egid = getegid();
    
    printf("rgid=%i egid=%i ", rgid, egid);
}

int try_priviledged_stuff()
{
    FILE* f;
    
    f = fopen("/etc/shadow", "r");
    if (NULL != f)
    {
        fclose(f);
        return 1;
    }
    
    return 0;
}

void print_ids()
{
    print_uids();
    print_gids();
    printf("priv=%i\n", try_priviledged_stuff());
}

int main()
{
    if (0 != geteuid())
    {
        printf("Please run me with root priviledges\n");
        return 1;
    }
    
    printf("# Running as root\n");
    print_ids();
    
    // dropping priviledges
    if(setegid(99)==-1){
        printf("ERROR set Group id(99) \n");
    }
    if(seteuid(my_user_uid) == -1){
        printf("ERROR set User id(my_user_uid) \n");
    }
    
    printf("# Running as unpriviledged process\n");
    print_ids();
    
    // gaining priviledges back
    if(seteuid(0)==-1){
        printf("ERROR at User id(0)\n");
    }
    if(setegid(0)==-1){
        printf("ERROR set Group id(0)\n");
    }
    
    printf("# Running as root again\n");
    print_ids();
    
    return 0;
}