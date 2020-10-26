#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "grammar.c"
#include "token.c"

typedef struct tnode
{
    struct tnode *left, *right, *parent, **children;
    char str[id_len];
    int line_no;
    bool is_terminal;
    int nofc;
}tree;

tree* newTreeNode(const char *str, bool is_terminal, int line_no)
{
    tree *root = (tree*)malloc(sizeof(tree));
    root->left = NULL;
    root->right = NULL;
    root->parent = NULL;
    root->children = NULL;
    strcpy(root->str, str);//copy start symbol into root
    root->is_terminal = is_terminal;
    root->line_no = line_no;
    root->nofc = 0;
    return root;
}

bool createParseTree(tree *root, tokenStream **s, grammar G)
{
    if(!s)//Reached end of token stream
        return true;
    if(!root)
        root = newTreeNode(G.rules[0]->str, false, 0);

    if(root->is_terminal)
    {
        if(!strcmp(root->str, "e"))//e means epsilon
        {
            printf("\033[0;32m");
            printf("Used epsilon\n", root->str, (*s)->lexeme);
            printf("\033[0m");
            return true;
        }
        else if( ((*s)->token == key) && !strcmp((*s)->lexeme, root->str) )//keyword
        {
            printf("\033[0;32m");
            printf("Terminal %s matched with token %s el1\n", root->str, (*s)->lexeme);
            printf("\033[0m");
            (*s) = (*s)->next;
            return true;
        }
        else if((*s)->token == num && !strcmp(root->str, "digit"))//integer const
        {
            printf("\033[0;32m");
            printf("Terminal %s matched with token %s el2\n", root->str, (*s)->lexeme);
            printf("\033[0m");
            (*s) = (*s)->next;
            return true;
        }
        else if((*s)->token == id && !strcmp(root->str, "id"))//identifier
        {
            printf("\033[0;32m");
            printf("Terminal %s matched with token %s el3\n", root->str, (*s)->lexeme);
            printf("\033[0m");
            (*s) = (*s)->next;
            return true;
        }
        else
        {
            printf("\033[0;31m");
            printf("Terminal %s didn't match token %s\n", root->str, (*s)->lexeme);
            printf("\033[0m");
            return false;
        }
    }
    //Now we have to expand the non-terminal inside root and make its children
    int i, j, size;
    node *temp;
    tokenStream *tmpstream;
    bool flag = false;
    for(i=0 ; i<G.size && !flag ; i++)//flag = true means we found the correct rule
    {
        if(strcmp(root->str, G.rules[i]->str))//If doesn't match
            continue;
        root->nofc = 0;
        for(temp = G.rules[i]->next; temp ; temp = temp->next)//No of children required
            root->nofc++;
        root->children = (tree**)malloc(sizeof(tree*)*root->nofc);
        printf("Rule %d matched with %s. %d children needed\n", i, root->str, root->nofc);
        temp = G.rules[i]->next;
        tmpstream = *s;
        for(j=0 ; j<root->nofc ; j++)
        {
            root->children[j] = newTreeNode(temp->str, temp->terminal, (*s)->line_no);
            flag = createParseTree(root->children[j], s, G);
            temp = temp->next;
            if(!flag)//If mismatch free all the memory allocated.
            {
                s = &tmpstream;
                for(j; j>=0;j--)
                    free(root->children[j]);
                free(root->children);
                root->nofc = 0;
                break;
            }
        }
    }
    if(!flag)
        printf("No rule matched for %s\n", root->str);
    return flag;
}

void printParseTree(tree *root)
{
    if(!root)
        return;
    printf("%s\n", root->str);
    for(int i = 0; i<root->nofc; i++)
        printParseTree(root->children[i]);
}

int main()
{
    tokenStream *s = tokeniseSourcecode("sourcecode.txt");
    grammar G = readGrammar("grammar.txt");
    tree *root = newTreeNode(G.rules[0]->str, false, 0);
    bool flag = createParseTree(root, &s, G);
    if(!flag)
        printf("Something went horribly wrong!\n");
    printParseTree(root);
}