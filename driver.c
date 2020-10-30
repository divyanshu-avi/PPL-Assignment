#include"parsetree.c"
#include"typeexptable.c"

int main(int argc, char *argv[])
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
    tokenStream *s = tokeniseSourcecode(argv[1]), *temp;
    tree *root = NULL;
    tablenode *head = NULL;
    if(!s)
        return 1;
    grammar G = readGrammar("grammar.txt");
    int choice;
    do
    {
        printf("Option 0: Exit\n");
        printf("Option 1: Create parse tree\n");
        printf("Option 2: Construct type expression table\n");
        printf("Option 3: Print parse tree\n");
        printf("Option 4: Print type expression table\n");
        printf("Please enter your choice\n");
        scanf("%d", &choice);
        switch(choice)
        {
            case 0 :    break;
            case 1 :    root = newTreeNode(G.rules[0]->str, false, 0 ,0);
                        temp = s;
                        bool flag = createParseTree(root, &temp, G);
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
                        break;
            case 2 :    head = createParseTable(s);
                        if(!head)
                        {
                            printf("\033[0;31m");
                            printf("Something went horribly wrong while creating the parse table!\n");
                            printf("\033[0m");
                            return 1;
                        }
                        printf("\033[0;32m");
                        printf("Parse table is created successfully!\n\n");
                        printf("\033[0m");
                        errornode *errorHead = genarateErrors(s, head);
                        if(!errorHead)
                        {
                            printf("\033[0;31m");
                            printf("Something went horribly wrong while finding errors!\n");
                            printf("\033[0m");
                            return 1;
                        }
                        printf("\nPrinting errors\n\n");
                        printf("%4s | %14s | %8s | %20s | %13s | %20s | %13s\n", "Line", "Statement Type", "Operator", "Left Operand", "Type", "Right Operand", "Type");
                        printErrors(errorHead);
                        printf("%4s | %14s | %8s | %20s | %13s | %20s | %13s\n", "Line", "Statement Type", "Operator", "Left Operand", "Type", "Right Operand", "Type");
                        break;
            case 3 :    if(!root)
                        {
                            root = newTreeNode(G.rules[0]->str, false, 0 ,0);
                            temp = s;
                            bool flag = createParseTree(root, &temp, G);
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
                        }
                        printf("Printing the parse tree\n\n");
                        printf("%20s | %8s | %20s | %7s | %4s | %5s\n", "Symbol", "Terminal", "Lexeme", "Line No", "Rule", "Depth\n");
                        printParseTree(root);
                        printf("\n%20s | %8s | %20s | %7s | %4s | %5s\n", "Symbol", "Terminal", "Lexeme", "Line No", "Rule", "Depth");
                        break;
            case 4 :    if(!head)
                        {
                            head = createParseTable(s);
                            if(!head)
                            {
                                printf("\033[0;31m");
                                printf("Something went horribly wrong while creating the parse table!\n");
                                printf("\033[0m");
                                return 1;
                            }
                            printf("\033[0;32m");
                            printf("Parse table is created successfully!\n\n");
                            printf("\033[0m");
                        }
                        printf("printing the type expression table\n\n");
                        printf("%20s | %12s | %14s\n", "Variable", "Type", "Static/Dynamic\n");
                        printParseTable(head);
                        break;
            default :   printf("\033[0;31m");
                        printf("Please enter a valid option\n");
                        printf("\033[0m");
                        break;
        }

    } while (choice);
    return 0;
}