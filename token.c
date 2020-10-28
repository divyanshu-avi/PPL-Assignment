#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include<ctype.h>
#define id_len 25
#define line_size 200
#define no_of_key 34//Number of keywords
#define key_size 11//max size of a keyword

typedef struct tok
{
    enum { key, num, id}token;
    char lexeme[id_len];
    int line_no;
    struct tok* next;
}tokenStream;

char** populateKeywords(char *src)
{
    char **keywords = (char**)malloc(sizeof(char*)*no_of_key);
    char buffer[key_size];
    FILE *fp = fopen(src, "r");
    if(!fp)
    {
        printf("Failed to open %s. Please check if the file exists.\n", src);
        return keywords;
    }
    for(int i = 0;i<no_of_key; i++)
    {
        fgets(buffer, key_size, fp);
        keywords[i] = (char*)malloc(sizeof(char)*no_of_key);
        strcpy(keywords[i], strtok(buffer, "\n"));
    }
    return keywords;
}

void setTokens(tokenStream *s, char **keywords)
{
    int i;
    while(s)
    {
        for(i=0;i<no_of_key;i++)
            if(!strcmp(s->lexeme, keywords[i]))
                break;
        if(i<no_of_key)
            s->token = key;
        else if(isdigit(s->lexeme[0]))
            s->token = num;
        else
            s->token = id;
        s = s->next;
    }
}

tokenStream* tokeniseSourcecode(const char *src)
{
    char buffer[line_size];
    char *word;
    tokenStream *head = NULL, *tail = NULL, *new_node = NULL;
    int lnum = 1;

    FILE *fp = fopen(src, "r");
    if(!fp)
    {
        printf("Failed to open sourcecode.txt. Please check if the file exists.\n");
        return head;
    }
    while(fgets(buffer, line_size, fp))
    {
        word = strtok(buffer, " \t\n");
        //printf("%s|", word);
        new_node = (tokenStream*)malloc(sizeof(tokenStream));
        new_node->next = NULL;
        strcpy(new_node->lexeme, word);
        new_node->line_no = lnum;
        if(!head)
        {
            head = new_node;
            tail = new_node;
        }
        else
        {
            tail->next = new_node;
            tail = new_node;
        }
        while(word = strtok(NULL, " \t\n"))
        {
            //printf("%s|", word);
            new_node = (tokenStream*)malloc(sizeof(tokenStream));
            new_node->next = NULL;
            strcpy(new_node->lexeme, word);
            new_node->line_no = lnum;
            tail->next = new_node;
            tail = new_node;
        }
        lnum++;
    }
    printf("Total %d lines read.\n", lnum-1);
    char **keywords = populateKeywords("keywords_terminal.txt");
    setTokens(head, keywords);
    return head;
}

void printTokenStream(tokenStream *s)
{
    while(s)
    {
        printf("%d-%s-%d\n", s->line_no, s->lexeme, s->token);
        s = s->next;
    }
}

/*int main()
{
    tokenStream *s = tokeniseSourcecode("testcases/t1.txt");
    printTokenStream(s);
    return 0;
}*/