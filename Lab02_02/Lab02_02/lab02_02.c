#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef WIN32
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")
#else
#include <netdb.h>
#include <sys/socket.h>
#endif

int validate_ip_address(char* ip_address)
/* Description: validates if the give string is a valid IP address; the regular expression is as follows
 *      "\d{1,3}.\d{1,3}.\d{1,3}.\d{1,3}" and verifies the values too
 * return  1 on success, otherwise 0
 */
{
    int    num_len = 0;
    char*  num_start = NULL;
    int    num_value = 0;
    size_t ip_length = 0;
    size_t i = 0;
    int    count_of_nums = 0;
    int    count_of_points = 0;

    num_start = ip_address;
    ip_length = strlen(ip_address);

    for (i=0; i<=ip_length; i++)
    {
        if (isdigit(ip_address[i]))
        {
            // we have a number here
            if (0 == num_len)
            {
                count_of_nums += 1;
            }

            num_len += 1;
            if (3 < num_len)
            {
                // the number already contains 4 characters, what is not good
                return 0;
            }
        }
        else if ('.' == ip_address[i] ||
                 '\0' == ip_address[i])
        {
            count_of_points += 1;

            // validate the number
            if (0 == num_len)
            {
                // the number is empty
                return 0;
            }
            
            num_value = atoi(num_start);    // converting the string what represents an integer into int
            if (255 < num_value)
            {
                // the number can not be part of an IP address
                return 0;
            }

            // the number is OK, we want to start the new number
            num_start = &ip_address[i+1];
            num_len = 0;
        }
        else
        {
            return 0;
        }
    }

    if (count_of_points != 4 || count_of_nums != 4)
    {
        return 0;
    }

    return 1;
}

int host_lookup(char* ip_address, char* hostname)
/* Description: this function resolves the hostnames after the ip address
 * returns  0 if unsuccesful
 *          1 if succesful
 */
{
    struct hostent *hp;

    ///
    /// The Windows and unix implementations are different, so we need ifdefs to make difference between them
    ///

#ifndef WIN32
    // for linux and maybe for other unices???
    in_addr_t *addr;
#else
    // for windows
    WSADATA startupData;
    struct in_addr addr = { 0 };
#endif

#ifndef WIN32
    // for linux & unices
    addr = inet_addr(ip_address);   // making an address from string
    hp = gethostbyaddr( addr, sizeof(struct in_addr), AF_INET); // getting the hostname
#else
    // for windows
    WSAStartup(MAKEWORD(2, 2), &startupData);   // initializing sockets library
    addr.s_addr = inet_addr(ip_address);
    hp = gethostbyaddr( (char*)&addr, 4, AF_INET);   // getting the hostname
    WSACleanup();   // uninitialize socket library
#endif

    if (NULL == hp)
    {
        // host not found
        return 0;
    }

    strcpy(hostname, hp->h_name);

    return 1;
}

int main(int argc, char** argv)
{
    char ip_address[16];    // XXX.XXX.XXX.XXX\0 == 16 characters maximum
    char hostname[64];
    int res = 0;

    memset(ip_address, 0, sizeof(ip_address));  // preinit the ip_address
    memset(hostname, 0, sizeof(hostname));      // preinit the hostname

    printf("Provide an IP address: ");
    res = scanf("%16s", ip_address);    // scans maximum 16 characters into the ip_address local variable
    if (0 == res)
    {
        // no valid string was provided on the standard input
        printf("Not a valid IP address\n");
        goto cleanup;
    }

    if (0 == validate_ip_address(ip_address))   // validating the ip address string
    {
        printf("The provided string is not an IP address\n");
        goto cleanup;
    }

    if (0 == host_lookup(ip_address, hostname))
    {
        printf("Host not found!\n");
    }
    else
    {
        printf("Found hostname: %s\n", hostname);
    }

cleanup:
    return 0;
}