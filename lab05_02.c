#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HTML_ENTRY(s) (s),(sizeof(s)-1)

typedef struct _char_to_html_entry
{
    char c;
    char* h;
    size_t s;
} char_to_html_entry;

static char_to_html_entry char_to_html[] = {
    {'"',  HTML_ENTRY("&quot;")},
    {'\'', HTML_ENTRY("&apos;")},
    {'&',  HTML_ENTRY("&amp;")},
    {'<',  HTML_ENTRY("&lt;")},
    {'>',  HTML_ENTRY("&gt;")},
    {' ',  HTML_ENTRY("&nbsp;")},
};
#define CHAR_TO_HTML_NUM sizeof(char_to_html)/sizeof(char_to_html[0])

int isMetaChar(char c)
{
    int i=0;
    
    for (i=0; i<CHAR_TO_HTML_NUM; i++)
    {
        if (char_to_html[i].c == c)
        {
            return i;
        }
    }
    
    return -1;
}

//
// Return value: the size of the generated string
//
size_t html_escape(char* toescape, char* escaped)
{
    size_t ret = 0;
    int n;
    
    while (*toescape)
    {
        n = isMetaChar(*toescape);
        if (-1 == n)
        {
            *escaped = *toescape;
            escaped++;
            toescape++;
            ret++;
        }
        else
        {
            memcpy(escaped, char_to_html[n].h, char_to_html[n].s);
            escaped += char_to_html[n].s;
            toescape++;
            ret += char_to_html[n].s;
        }
    }
    
    *escaped = '\0';
    
    return ret;
}

int main()
{
    size_t n;
    char buff[100];
    char encoded[100];
    
    printf("String to escape:\n");
    scanf("%99[^\n]s", buff);
    
    n = html_escape(buff, encoded);
    printf("Html escaped version:\n%s", encoded);
}