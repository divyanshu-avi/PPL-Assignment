#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define line_size 60//Max size of a line in grammar.txt
#define id_len 25//max size of identifier
typedef struct edon
{
    char str[id_len];
    struct edon* next;
}node;

int main()
{
    FILE *grammar = fopen("grammar.txt", "r");
    if(!grammar)
    {
        printf("Failed to open grammar.txt. Please check if the file exists.\n");
        return 1;
    }
    char buffer[line_size];
    int count=0, lines=0;
    char *token;
    node *temp, *new_node;

    while(fgets(buffer, line_size, grammar))
        lines++;
    fclose(grammar);
    node **garr = (node**)malloc(sizeof(node*)*lines);
    
    grammar = fopen("grammar.txt", "r");
    while(fgets(buffer, line_size, grammar))
    {
        token = strtok(buffer, " ");
        count++;
        garr[count] = (node*)malloc(sizeof(node));
        temp = garr[count];
        temp->next = NULL;
        strcpy(temp->str, token);
        while(token = strtok(NULL, " "))
        {
            new_node = (node*)malloc(sizeof(node));
            new_node->next = NULL;
            strcpy(new_node->str, token);
            temp->next = new_node;
            temp = new_node;
        }
    }

    fclose(grammar);
    return 0;
}