#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#define id_len 25
#define line_size 100

typedef struct tok
{
    enum { key, op, punc, id, num}token;
    char lexeme[id_len];
    int line_no;
    struct tok* next;
}tokenStream;

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
        word = strtok(strtok(buffer, "\n"), " ");//Removing \n from buffer
        printf("%s|", word);
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
        while(word = strtok(NULL, " "))
        {
            printf("%s|", word);
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
    return head;
}

void printTokenStream(tokenStream *s)
{
    while(s)
    {
        printf("%d-%s\n", s->line_no, s->lexeme);
        s = s->next;
    }
}

int main()
{
    tokenStream *s = tokeniseSourcecode("sourcecode.txt");
    printTokenStream(s);
    return 0;
}