#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#define line_size 100//Max size of a line in grammar.txt
#define id_len 25//max size of identifier
typedef struct edon
{
    char str[id_len];
    bool terminal;
    struct edon* next;
}node;
typedef struct
{
    node **rules;
    int size;
}grammar;

bool isTerminal(node *temp, grammar G)
{
    int i;
    for(i=0;i<G.size;i++)
        if(!strcmp(temp->str, G.rules[i]->str))//If it matches an term on the LHS, then it's a non-terminal
            return false;
    return true;
}

void printGrammar(grammar G)
{
    int i;
    for(i=0;i<G.size;i++)
    {
        printf("%s %d -> ", G.rules[i]->str, G.rules[i]->terminal);
        node *temp = G.rules[i]->next;
        while(temp)
        {
            printf("%s(%d) ", temp->str, temp->terminal);
            //printf("%s ", temp->str);//Uncomment above and comment this to check terminal/non-terminal values
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
        token = strtok(strtok(buffer, "\n"), " ");//Remove \n from buffer
        G.rules[count] = (node*)malloc(sizeof(node));
        temp = G.rules[count];
        temp->next = NULL;
        temp->terminal = false;//We already know these are non-terminals as they appear on LHS
        strcpy(temp->str, token);
        while(token = strtok(NULL, " "))
        {
            new_node = (node*)malloc(sizeof(node));
            new_node->next = NULL;
            strcpy(new_node->str, token);
            temp->next = new_node;
            temp = new_node;
        }
        count++;
    }
    fclose(fp);
    //Now we need to set the terminal field for all nodes except those present in the array
    for(int i = 0; i<G.size; i++)
    {
        temp = G.rules[i]->next;
        while(temp)
        {
            temp->terminal = isTerminal(temp, G);
            temp = temp->next;
        }
    }
    return G;
}

/*int main()
{
    grammar G = readGrammar("grammar.txt");
    if(G.size == 0)
        return 1;
    printGrammar(G);
    return 0;
}*/