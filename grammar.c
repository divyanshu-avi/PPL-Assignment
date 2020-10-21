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
typedef struct
{
    node **rules;
    int size;
}grammar;

void printGrammar(grammar G)
{
    int i;
    for(i=0;i<G.size;i++)
    {
        printf("%s -> ", G.rules[i]->str);
        node *temp = G.rules[i]->next;
        while(temp)
        {
            printf("%s ", temp->str);
            temp = temp->next;
        }
        printf("\n");
    }
    printf("Grammar contains %d rules.\n", G.size);
    
}

grammar readGrammar(const char* source)
{
    char buffer[line_size];
    int count=0, lines=0;
    char *token;
    node *temp, *new_node;
    grammar G;
    G.size = 0;
    
    FILE *fp = fopen(source, "r");
    if(!fp)
    {
        printf("Failed to open grammar.txt. Please check if the file exists.\n");
        return G;
    }

    while(fgets(buffer, line_size, fp))
        lines++;
    fclose(fp);
    G.size = lines;
    G.rules = (node**)malloc(sizeof(node*)*G.size);
    
    fp = fopen("grammar.txt", "r");
    while(fgets(buffer, line_size, fp))
    {
        token = strtok(buffer, " ");
        count++;
        G.rules[count] = (node*)malloc(sizeof(node));
        temp = G.rules[count];
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
    fclose(fp);
    return G;
}

int main()
{
    grammar G = readGrammar("grammar.txt");
    if(G.size == 0)
        return 1;
    printGrammar(G);
    return 0;
}