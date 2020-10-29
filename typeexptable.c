#include"token.c"

enum type{ integer, real, boolean, rectarr, jaggedarr};

typedef struct tet
{
    struct tet *next;
    char var[id_len];
    enum type data_type;
    bool is_static;
}tablenode;

tablenode* newTableNode(const char *var, enum type dtype, bool is_static)
{
    tablenode* head = (tablenode*)malloc(sizeof(tablenode));
    strcpy(head->var, var);
    head->data_type = dtype;
    head->is_static = is_static;
    head->next = NULL;
    return head;
}

tablenode* createParseTable(tokenStream *s)
{
    tokenStream *temp;
    enum type dtype;
    tablenode *head=NULL, *trav = NULL;
    bool is_static;
    while(s)
    {
        if(s->token != id)
        {
            s = s->next;
            continue;
        }
        temp = s;
        while( temp && strcmp(temp->lexeme, ";") && strcmp(temp->lexeme, "=") && strcmp(temp->lexeme, ":") )
            temp = temp->next;
        if(!temp || !strcmp(temp->lexeme, "="))
            break;
        if(!strcmp(temp->lexeme, ";"))
        {
            s = temp->next;
            continue;
        }
        temp = temp->next;
        if(!strcmp(temp->lexeme, "integer"))
            dtype = integer;
        else if(!strcmp(temp->lexeme, "real"))
            dtype = real;
        else if(!strcmp(temp->lexeme, "boolean"))
            dtype = boolean;
        else if(!strcmp(temp->lexeme, "array"))
            dtype = rectarr;
        else if(!strcmp(temp->lexeme, "jagged"))
            dtype = jaggedarr;
        else
        {
            printf("Something went wrong in parse table creation %s\n", temp->lexeme);
            return NULL;
        }
        if(dtype == rectarr)
        {
            if(temp->next->next->token == id)
                is_static = false;
            else
                is_static = true;
        }
        for(s;s->next!=temp; s=s->next)
        {
            if(!head)
            {
                head = newTableNode(s->lexeme, dtype, is_static);
                trav = head;
            }
            else
            {
                trav->next = newTableNode(s->lexeme, dtype, is_static);
                trav = trav->next;
            }
            
        }
        s = temp->next;
    }
    return head;
}

void printParseTable(tablenode *head)
{
    char *type;
    char *sord;
    while(head)
    {
        switch(head->data_type)
        {
            case integer : type = "integer"; break;
            case real : type = "real"; break;
            case boolean : type = "boolean"; break;
            case rectarr : type = "array"; break;
            case jaggedarr : type = "jagged array"; break;
        }
        if(head->is_static)
            sord = "static";
        else
            sord = "dynamic";
        if(head->data_type!=rectarr)
            sord = "not_applicable";
        
        printf("%20s | %12s | %14s\n", head->var, type, sord);
        head = head->next;
    }
}

int main()
{
    tokenStream *s = tokeniseSourcecode("t4.txt");
    tablenode *head = createParseTable(s);
    printParseTable(head);
    return 0;
}