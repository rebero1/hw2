/* Complete recursive descent parser for the calculator language.
   Builds on figure 2.16 in the text.  Prints a trace of productions
   predicted and tokens matched.  Does no error recovery: prints
   "syntax error" and dies on invalid input.
   Michael L. Scott, 2008-2021.
*/

//change - puts, putchar, fputs
#include <iostream>
#include "stdio.h"
#include "stdlib.h"

#include "scan.h"

const char *names[] = {"read", "write", "id", "literal", "gets", "add",
                       "sub", "mul", "div", "lparen", "rparen", "eof", "if",
                       "fi", "do", "od", "check", "dequals", "nequal", "lt", "gt", "le", "ge"};

static token upcoming_token;

void error()
{
    std::cerr<<"syntax error\n";
    exit(1);
}

void match(token expected)
{
    if (upcoming_token == expected)
    {
        // printf("matched %s", names[upcoming_token]);
        std::cout<<"matched "<<names[upcoming_token];
        if (upcoming_token == t_id || upcoming_token == t_literal)
            std::cout<<": "<<token_image;
            // printf(": %s", token_image);
        std::cout.put('\n');
        
        upcoming_token = scan();
    }
    else
        error();
}

void program();
void stmt_list();
void stmt();
void relation();
void expr();
void term();
void term_tail();
void factor();
void expr_tail();
void factor_tail();
void add_op();
void mul_op();
void rel_op();

void program()
{
    switch (upcoming_token)
    {
    case t_id:
    case t_read:
    case t_write:
    case t_if:
    case t_do:
    case t_check:
    case t_eof:
        // puts("predict program --> stmt_list eof\n");
        std::cout<<"predict program --> stmt_list eof\n";
        stmt_list();
        match(t_eof);
        break;
    default:
        error();
    }
}

void stmt_list()
{
    switch (upcoming_token)
    {
    case t_id:
    case t_read:
    case t_write:
    case t_if:
    case t_do:
    case t_check:
        std::cout<<"predict stmt_list --> stmt stmt_list\n";
        stmt();
        stmt_list();
        break;
    case t_eof:
    case t_fi:
    case t_od:
        std::cout<<"predict stmt_list --> epsilon\n";
        break; /* epsilon production */
    default:
        error();
    }
}

void stmt()
{
    switch (upcoming_token)
    {
    case t_id:
        std::cout<<"predict stmt --> id gets relation\n";
        match(t_id);
        match(t_gets);
        // expr();
        relation();
        break;
    case t_read:
        std::cout<<"predict stmt --> read id\n";
        match(t_read);
        match(t_id);
        break;
    case t_write:
        std::cout<<"predict stmt --> write relation\n";
        match(t_write);
        // expr();
        relation();
        break;
    case t_if:
        std::cout<<"predict stmt --> if relation stmt_list fi\n";
        match(t_if);
        relation();
        stmt_list();
        match(t_fi);
        break;
    case t_do:
        std::cout<<"predict stmt --> do stmt_list od\n";
        match(t_do);
        stmt_list();
        match(t_od);
        break;
    case t_check:
        std::cout<<"predict stmt --> check relation\n";
        match(t_check);
        relation();  
        break;
    default:
        error();
    }
}

void relation(){//R	→ 	E ET
    switch (upcoming_token)
    {
        case t_id:
        case t_lparen:
        case t_literal:
            std::cout<<"predict relation -> expr expr_tail\n";
            expr();
            expr_tail();
            break;
        default:
            error();
    }
}

void expr()
{
    switch (upcoming_token)
    {
    case t_id:
    case t_literal:
    case t_lparen:
        std::cout<<"predict expr --> term term_tail\n";
        term();
        term_tail();
        break;
    default:
        error();
    }
}

void term()
{
    switch (upcoming_token)
    {
    case t_id:
    case t_literal:
    case t_lparen:
        std::cout<<"predict term --> factor factor_tail\n";
        factor();
        factor_tail();
        break;
    default:
        error();
    }
}

void term_tail()
{
    switch (upcoming_token)
    {
    case t_add:
    case t_sub:
        std::cout<<"predict term_tail --> add_op term term_tail\n";
        add_op();
        term();
        term_tail();
        break;
    case t_rparen:
    case t_id:
    case t_read:
    case t_write:
    case t_eof:
    case t_if:
    case t_fi:
    case t_do:
    case t_od:
    case t_check:
    case t_dequals:
    case t_nequal:
    case t_lt:
    case t_gt:
    case t_le:
    case t_ge:
        std::cout<<"predict term_tail --> epsilon\n";
        break; /* epsilon production */
    default:
        error();
    }
}

void factor()
{
    switch (upcoming_token)
    {
    case t_literal:
        std::cout<<"predict factor --> literal\n";
        match(t_literal);
        break;
    case t_id:
        std::cout<<"predict factor --> id\n";
        match(t_id);
        break;
    case t_lparen:
        std::cout<<"predict factor --> lparen relation rparen\n";
        match(t_lparen);
        relation();
        match(t_rparen);
        break;
    default:
        error();
    }
}

void factor_tail()
{
    switch (upcoming_token)
    {
    case t_mul:
    case t_div:
        std::cout<<"predict factor_tail --> mul_op factor factor_tail\n";
        mul_op();
        factor();
        factor_tail();
        break;
    case t_add:
    case t_sub:
    case t_rparen:
    case t_id:
    case t_read:
    case t_write:
    case t_eof:
    case t_if:
    case t_fi:
    case t_do:
    case t_od:
    case t_check:
    case t_dequals:
    case t_nequal:
    case t_lt:
    case t_gt:
    case t_le:
    case t_ge:
        std::cout<<"predict factor_tail --> epsilon\n";
        break; /* epsilon production */
    default:
        error();
    }
}

void add_op()
{
    switch (upcoming_token)
    {
    case t_add:
        std::cout<<"predict add_op --> add\n";
        match(t_add);
        break;
    case t_sub:
        std::cout<<"predict add_op --> sub\n";
        match(t_sub);
        break;
    default:
        error();
    }
}

void mul_op()
{
    switch (upcoming_token)
    {
    case t_mul:
        std::cout<<"predict mul_op --> mul\n";
        match(t_mul);
        break;
    case t_div:
        std::cout<<"predict mul_op --> div\n";
        match(t_div);
        break;
    default:
        error();
    }
}

void rel_op()
{
    switch (upcoming_token)
    {
    case t_dequals:
        std::cout<<"predict rel_op --> dequals\n";
        match(t_dequals);
        break;
    case t_nequal:
        std::cout<<"predict rel_op --> t_nequal\n";
        match(t_nequal);
        break;
    case t_lt:
        std::cout<<"predict rel_op --> t_lt\n";
        match(t_lt);
        break;
    case t_gt:
        std::cout<<"predict rel_op --> t_gt\n";
        match(t_gt);
        break;
    case t_le:
        std::cout<<"predict rel_op --> t_le\n";
        match(t_le);
        break;
    case t_ge:
        std::cout<<"predict rel_op --> t_ge\n";
        match(t_ge);
        break;
    default:
        error();
    }

}

void expr_tail()
{
    switch (upcoming_token)
    {
    case t_dequals:
    case t_nequal:
    case t_lt:
    case t_gt:
    case t_le:
    case t_ge:
        std::cout<<"predict expr_tail --> rel_op expr\n";
        rel_op();
        expr();
        break;
    case t_eof:
    case t_id:
    case t_read: 
    case t_write:
    case t_if:
    case t_fi:
    case t_do:
    case t_od:
    case t_check:
    case t_rparen:
        std::cout<<"predict expr_tail --> epsilon\n";
        break;
    
    default:
        error();
    }

}

int main()
{
    upcoming_token = scan();
    program();
    return 0;
}
