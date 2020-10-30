//#include"token.c"
enum type{ integer, real, boolean, rectarr, jaggedarr};

typedef struct tet
{
    struct tet *next;
    char var[id_len];
    enum type data_type;
    bool is_static;
}tablenode;

typedef struct err
{
    struct err *next;
    char stype[12];//Declaration or Assignment
    char operator[4];
    char lhs[id_len];
    char ltype[15];
    char rhs[id_len];
    char rtype[15];
    int depth;
    int line_no;
    char message[31];
}errornode;

tablenode* newTableNode(const char *var, enum type dtype, bool is_static)
{
    tablenode* head = (tablenode*)malloc(sizeof(tablenode));
    strcpy(head->var, var);
    head->data_type = dtype;
    head->is_static = is_static;
    head->next = NULL;
    return head;
}
errornode* newErrorNode(const char *stype, const char *operator, const char *lhs, const char *ltype, const char *rhs, const char *rtype, int depth, const char *message, int line_no)
{
    errornode *head = (errornode*)malloc(sizeof(errornode));
    strcpy(head->stype, stype);
    strcpy(head->operator, operator);
    strcpy(head->lhs, lhs);
    strcpy(head->ltype, ltype);
    strcpy(head->rhs, rhs);
    strcpy(head->rtype, rtype);
    strcpy(head->message, message);
    head->depth = depth;
    head->line_no = line_no;
    head->next = NULL;
    return head;
}

bool isaop(const char *op)//Arithmetic operator?
{
    if(!strcmp(op, "+") || !strcmp(op, "-") || !strcmp(op, "*") || !strcmp(op, "/"))
        return true;
    return false;
}

bool isbop(const char *op)//Boolean operator?
{
    if(!strcmp(op, "&&&") || !strcmp(op, "|||"))
        return true;
    return false;
}

char *findtype(char *lex, tablenode *head)
{
    enum type dtype;
    while(head)
    {
        if(!strcmp(lex, head->var))
        {
            dtype = head->data_type;
            break;
        }
        head = head->next;
    }
    if(!head)
    {
        printf("\033[0;31m");        
        printf("%s not found in the type exp table\n", lex);
        printf("\033[0m");
        return NULL;
    }
    if(dtype == integer)
        return "integer";
    else if(dtype == real)
        return "real";
    else if(dtype == boolean)
        return "boolean";
    else if(dtype == rectarr)
        return "array";
    else if(dtype == jaggedarr)
        return "jagged array";
    else return NULL;
}

errornode* genarateErrors(tokenStream *s, tablenode *tetable)
{
    errornode *head = NULL, *trav;
    char stype[12], operator[4], lhs[id_len], ltype[13], rhs[id_len], rtype[13], message[30];
    tokenStream *l=s, *r;
    int depth = 0;
    bool error = false;
    while(s)
    {
        if(s->token != key || (!isaop(s->lexeme) && !isbop(s->lexeme) && strcmp(s->lexeme, "=")) )
        {
            l = s;
            s = s->next;
            continue;
        }
        strcpy(operator, s->lexeme);
        r = s->next;
        if(l->token == id)
        {
            strcpy(ltype, findtype(l->lexeme, tetable));
            strcpy(lhs, l->lexeme);
        }
        else if(l->token == num)
        {
            strcpy(ltype, "integer");
            strcpy(lhs, l->lexeme);
        }
        else
        {
            l = s;
            s = s->next;
            continue;
        }        
        if(r->token == id)
        {
            strcpy(rtype, findtype(r->lexeme, tetable));
            strcpy(rhs, r->lexeme);
        }
        else if(r->token == num)
        {
            strcpy(rtype, "integer");
            strcpy(rhs, r->lexeme);
        }
        else
        {
            l = s;
            s = s->next;
            continue;
        }
        if(strcmp(ltype, rtype))
        {
            error = true;
            //printf("-%s-\n-%s-\n", ltype, rtype);
        }
        else if(isbop(operator) && (strcmp(ltype,"boolean") || strcmp(rtype, "boolean")))
            error = true;
        else if(isaop(operator) && (!strcmp(ltype,"boolean") || !strcmp(rtype, "boolean")) )
            error = true;
        if(error)
        {
            error = false;
            if(!head)
            {
                head = newErrorNode("Assignment", operator, lhs, ltype, rhs, rtype, 0, "Dummy message", s->line_no);
                trav = head;
            }
            else
            {
                trav->next = newErrorNode("Assignment", operator, lhs, ltype, rhs, rtype, 0, "Dummy message", s->line_no);
                trav = trav->next;
            }
        }
        l = s;
        s = s->next;
    }
    return head;
}

void printErrors(errornode *head)
{
    while(head)
    {
        printf("%4d | %14s | %8s | %20s | %13s | %20s | %13s\n", head->line_no, head->stype, head->operator, head->lhs, head->ltype, head->rhs, head->rtype);
        head = head->next;
    }
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

/*int main()
{
    tokenStream *s = tokeniseSourcecode("t2.txt");
    tablenode *head = createParseTable(s);
    printParseTable(head);
    errornode *errorHead = genarateErrors(s, head);
    if(!errorHead)
    {
        printf("NULL returned\n");
        return 1;
    }
    printf("\nPrinting errors\n\n");
    printf("%4s | %14s | %8s | %20s | %12s | %20s | %12s\n", "Line", "Statement Type", "Operator", "Left Operand", "Type", "Right Operand", "Type");
    printErrors(errorHead);
    printf("%4s | %14s | %8s | %20s | %12s | %20s | %12s\n", "Line", "Statement Type", "Operator", "Left Operand", "Type", "Right Operand", "Type");
    return 0;
}*/