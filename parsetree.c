/*
Group - 37
Divyanshu Singh     2017B4A70547P
Lakshya Agarwal     2017B4A70630P
Naman Chokhani      2017B3A70726P
Tanya Garg          2018A7PS0215P
*/

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
    int depth;
    char lexeme[id_len];
    int rule;
}tree;

tree* newTreeNode(const char *str, bool is_terminal, int line_no, int depth)
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
    root->depth = depth;
    strcpy(root->lexeme, "-");
    return root;
}

bool createParseTree(tree *root, tokenStream **s, grammar G)
{
    if(!(*s))//Reached end of token stream
        return true;
    if(!root)
        root = newTreeNode(G.rules[0]->str, false, 0, 0);

    if(root->is_terminal)
    {
        if(!strcmp(root->str, "e"))//e means epsilon, although now we have removed epsilon from the grammar
        {
            strcpy(root->lexeme, "epsilon");
            return true;
        }
        else if( ((*s)->token == key) && !strcmp((*s)->lexeme, root->str) )//keyword
        {
            //printf("\033[0;32m");
            //printf("Terminal %s matched with token %s el1\n", root->str, (*s)->lexeme);
            //printf("\033[0m");
            strcpy(root->lexeme, (*s)->lexeme);
            *s = (*s)->next;
            return true;
        }
        else if((*s)->token == num && !strcmp(root->str, "digit"))//integer const
        {
            //printf("\033[0;32m");
            //printf("Terminal %s matched with token %s el2\n", root->str, (*s)->lexeme);
            //printf("\033[0m");
            strcpy(root->lexeme, (*s)->lexeme);
            *s = (*s)->next;
            //printf("***%s***\n", (*s)->lexeme);
            return true;
        }
        else if((*s)->token == id && !strcmp(root->str, "id"))//identifier
        {
            //printf("\033[0;32m");
            //printf("Terminal %s matched with token %s el3\n", root->str, (*s)->lexeme);
            //printf("\033[0m");
            strcpy(root->lexeme, (*s)->lexeme);
            *s = (*s)->next;
            return true;
        }
        else
        {
            //printf("\033[0;31m");
            //printf("Terminal %s didn't match token %s\n", root->str, (*s)->lexeme);
            //printf("\033[0m");
            return false;
        }
    }
    //Now we have to expand the non-terminal inside root and make its children
    int i, j;
    node *temp;
    tokenStream *tmpstream;
    bool flag = false;
    for(i=0 ; i<G.size && !flag ; i++)//flag = true means we found the correct rule
    {
        if(strcmp(root->str, G.rules[i]->str))//If doesn't match
            continue;
        root->nofc = 0;
        root->rule = i+1;
        for(temp = G.rules[i]->next; temp ; temp = temp->next)//No of children required
            root->nofc++;
        root->children = (tree**)malloc(sizeof(tree*)*root->nofc);
        //printf("Rule %d matched with %s. %d children needed\n", i+1, root->str, root->nofc);
        temp = G.rules[i]->next;
        tmpstream = *s;
        for(j=0 ; j<root->nofc; j++)
        {
            root->children[j] = newTreeNode(temp->str, temp->terminal, (*s)->line_no, root->depth+1);
            if(!strcmp(root->children[j]->str, "e"))
            {
                //printf("\033[0;32m");
                //printf("Replaced %s with epsilon\n", G.rules[i]->str);
                //printf("\033[0m");
            }
            flag = createParseTree(root->children[j], s, G);
            //printf("|||%s||| %d\n", (*s)->lexeme, flag);
            temp = temp->next;
            if(!flag)//If mismatch free all the memory allocated.
            {
                *s = tmpstream;
                for(j; j>=0;j--)
                    free(root->children[j]);
                free(root->children);
                root->nofc = 0;
                break;
            }
        }
    }
    //if(!flag)
        //printf("No rule matched for %s\n", root->str);
    //if(flag && (*s))
       // printf("Returning from %s with tokenstream at %s line %d\n", root->str, (*s)->lexeme, (*s)->line_no);
    return flag;
}

void printParseTree(tree *root)
{
    if(!root)
        return;
    if(root->is_terminal)
        printf("%20s | %8s | %20s | %7d | %4s | %5d\n", root->str, "Yes", root->lexeme, root->line_no, "-", root->depth);
    else
        printf("%20s | %8s | %20s | %7s | %4d | %5d\n", root->str, "No", root->lexeme, "-", root->rule, root->depth);
    for(int i = 0; i<root->nofc; i++)
        printParseTree(root->children[i]);
}

/*int main(int argc, char *argv[])
{
    if(argc<2)
    {
        printf("\033[0;31m");
        printf("Please provide sourcecode as a Command Line argument!\n");
        printf("\033[0m");
        return 1;
    }
    if(argc>2)
    {
        printf("\033[0;31m");
        printf("Too many arguments!\n");
        printf("\033[0m");
        return 1;
    }
    tokenStream *s = tokeniseSourcecode(argv[1]);
    if(!s)
        return 1;
    grammar G = readGrammar("grammar.txt");
    tree *root = newTreeNode(G.rules[0]->str, false, 0 ,0);
    bool flag = createParseTree(root, &s, G);
    if(!flag)
    {
        printf("\033[0;31m");
        printf("Something went horribly wrong while creating the parse tree!\n");
        printf("\033[0m");
        return 1;
    }
    printf("\033[0;32m");
    printf("Parse tree is created successfully!\n\n");
    printf("\033[0m");

    printf("Printing the parse tree\n\n");
    printf("%20s | %8s | %20s | %7s | %4s | %5s\n", "Symbol", "Terminal", "Lexeme", "Line No", "Rule", "Depth");
    printParseTree(root);
}*/