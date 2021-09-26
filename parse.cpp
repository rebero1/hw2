/* Complete recursive descent parser for the calculator language.
   Builds on figure 2.16 in the text.  Prints a trace of productions
   predicted and tokens matched.  Does no error recovery: prints
   "syntax error" and dies on invalid input.
   Michael L. Scott, 2008-2021.
*/

//change - puts, putchar, fputs
#include <iostream>
#include <set>
#include <unordered_set>
#include "stdio.h"
#include "stdlib.h"

#include "scan.h"

const char *names[] = {"read", "write", "id", "literal", "gets", "add",
                       "sub", "mul", "div", "lparen", "rparen", "eof", "if",
                       "fi", "do", "od", "check", "dequals", "nequal", "lt", "gt", "le", "ge", "lexerror"};

static token upcoming_token;

//First and follow sets
//FIRST SETS
static std::set<token> first_P = {t_eof, t_id, t_read,t_write,t_if,t_do,t_check};
static std::set<token> first_SL = {t_id, t_read,t_write,t_if,t_do,t_check};
static std::set<token> first_S = {t_id, t_read,t_write,t_if,t_do,t_check};
static std::set<token> first_R = {t_id, t_lparen,t_literal};
static std::set<token> first_E = {t_id, t_lparen,t_literal};
static std::set<token> first_ET = {t_dequals, t_nequal,t_lt,t_gt,t_le,t_ge};
static std::set<token> first_T = {t_id, t_lparen,t_literal};
static std::set<token> first_TT = {t_add,t_sub};
static std::set<token> first_F = {t_id, t_lparen, t_literal};
static std::set<token> first_FT = {t_mul,t_div};
static std::set<token> first_ro = {t_dequals,t_nequal,t_lt,t_gt,t_le,t_ge};
static std::set<token> first_ao = {t_add, t_sub};
static std::set<token> first_mo = {t_mul,t_div};

//FOLLOW SETS
static std::set<token> follow_P = {};
static std::set<token> follow_SL = {t_eof, t_id, t_read,t_write,t_if,t_fi, t_do, t_od, t_check};
static std::set<token> follow_S = {t_eof, t_id, t_read,t_write,t_if,t_fi, t_do, t_od, t_check};
static std::set<token> follow_R = {t_eof, t_id, t_read,t_write,t_if,t_fi, t_do, t_od, t_check, t_rparen};
static std::set<token> follow_E = {t_eof, t_id, t_read,t_write,t_if,t_fi, t_do, t_od, t_check, t_rparen, t_dequals, t_nequal, t_lt,t_gt,t_ge,t_le};
static std::set<token> follow_ET = {t_eof, t_id, t_read,t_write,t_if,t_fi, t_do, t_od, t_check, t_rparen};
static std::set<token> follow_T = {t_eof, t_id, t_read,t_write,t_if,t_fi, t_do, t_od, t_check, t_rparen, t_dequals, t_nequal, t_lt,t_gt,t_ge,t_le,t_add,t_sub};
static std::set<token> follow_TT = {t_eof, t_id, t_read,t_write,t_if,t_fi, t_do, t_od, t_check, t_rparen, t_dequals, t_nequal, t_lt,t_gt,t_ge,t_le};
static std::set<token> follow_F = {t_eof, t_id, t_read,t_write,t_if,t_fi, t_do, t_od, t_check, t_rparen, t_dequals, t_nequal, t_lt,t_gt,t_ge,t_le,t_add,t_sub, t_mul, t_div};
static std::set<token> follow_FT = {t_eof, t_id, t_read,t_write,t_if,t_fi, t_do, t_od, t_check, t_rparen, t_dequals, t_nequal, t_lt,t_gt,t_ge,t_le,t_add,t_sub};
static std::set<token> follow_ro = {t_id, t_lparen,t_literal};
static std::set<token> follow_ao = {t_id, t_lparen,t_literal};
static std::set<token> follow_mo = {t_id, t_lparen,t_literal};



void error()
{
    std::cerr<<"syntax error\n";
    return;
    // exit(1);
}

int check_for_error(std::set<token> first_set, std::set<token> follow_set){
    // std::cout<<"upcoming tokkin"<<upcoming_token;
 if(upcoming_token == t_eof) return 1;
    
 if(first_set.find(upcoming_token) == first_set.end() && follow_set.find(upcoming_token) == follow_set.end()){//print error
     std::cerr<<"error: token \""<<names[upcoming_token]<<"\" not parseable\n";
     memmove(token_image, token_image+1,strlen(token_image +1)+1);

     if(upcoming_token != t_eof) upcoming_token = scan();
     
 }

 
 while(first_set.find(upcoming_token) == first_set.end() && follow_set.find(upcoming_token) == follow_set.end() && upcoming_token != t_eof){//delete tokens
    memmove(token_image, token_image+1,strlen(token_image +1)+1);
    if(upcoming_token != t_eof) upcoming_token = scan();
 }


 //case 0 - token in first //case 1 - token in follow_set  //case 3 - t_eof

 if(first_set.find(upcoming_token) != first_set.end()){
     return 0;
 }else if(follow_set.find(upcoming_token) != follow_set.end()){
     return 1;
 }

 return 1;



}

void match(token expected)
{

    if (upcoming_token == expected)
    {
        // printf("matched %s", names[upcoming_token]);
        std::cout<<"matched "<<names[upcoming_token];
        if (upcoming_token == t_id || upcoming_token == t_literal)
            std::cout<<": "<<token_image;
        std::cout.put('\n');
        
        upcoming_token = scan();
    }
    else{
        // token_image = expected + token_image;
        std::cerr<<"error: expected "<<names[expected]<<" got "<<names[upcoming_token]<<std::endl;
        if (expected == t_id || expected == t_literal)
            std::cout<<": "<<token_image;
        std::cout.put('\n');
        upcoming_token = scan();
    }
        // error();
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
    int error_code = check_for_error(first_P, follow_P);
    switch(error_code)
    {
        case 0: //case 0 - token in first
        case 3: //case 3 - t_eof
            break;
        case 1: //case 1 - token in follow_set
            return;
        default:
            error();    
    }
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
    int error_code = check_for_error(first_SL, follow_SL);
    switch(error_code)
    {
        case 0: //case 0 - token in first
        case 3: //case 3 - t_eof
            break;
        case 1: //case 1 - token in follow_set
            return;
        default:
            error();    
    }
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
    int error_code = check_for_error(first_S, follow_S);
    switch(error_code)
    {
        case 0: //case 0 - token in first
        case 3: //case 3 - t_eof
            break;
        case 1: //case 1 - token in follow_set
            return;
        default:
            error();    
    }
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
    int error_code = check_for_error(first_R, follow_R);
    switch(error_code)
    {
        case 0: //case 0 - token in first
        case 3: //case 3 - t_eof
            break;
        case 1: //case 1 - token in follow_set
            return;
        default:
            error();    
    }
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
    int error_code = check_for_error(first_E, follow_E);
    switch(error_code)
    {
        case 0: //case 0 - token in first
        case 3: //case 3 - t_eof
            break;
        case 1: //case 1 - token in follow_set
            return;
        default:
            error();    
    }
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
    int error_code = check_for_error(first_T, follow_T);
    switch(error_code)
    {
        case 0: //case 0 - token in first
        case 3: //case 3 - t_eof
            break;
        case 1: //case 1 - token in follow_set
            return;
        default:
            error();    
    }
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
    int error_code = check_for_error(first_TT, follow_TT);
    switch(error_code)
    {
        case 0: //case 0 - token in first
        case 3: //case 3 - t_eof
            break;
        case 1: //case 1 - token in follow_set
            return;
        default:
            error();    
    }
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
    int error_code = check_for_error(first_F, follow_F);
    switch(error_code)
    {
        case 0: //case 0 - token in first
        case 3: //case 3 - t_eof
            break;
        case 1: //case 1 - token in follow_set
            return;
        default:
            error();    
    }
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
    int error_code = check_for_error(first_FT, follow_FT);
    switch(error_code)
    {
        case 0: //case 0 - token in first
        case 3: //case 3 - t_eof
            break;
        case 1: //case 1 - token in follow_set
            return;
        default:
            error();    
    }
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
    int error_code = check_for_error(first_ao, follow_ao);
    switch(error_code)
    {
        case 0: //case 0 - token in first
        case 3: //case 3 - t_eof
            break;
        case 1: //case 1 - token in follow_set
            return;
        default:
            error();    
    }
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
    int error_code = check_for_error(first_mo, follow_mo);
    switch(error_code)
    {
        case 0: //case 0 - token in first
        case 3: //case 3 - t_eof
            break;
        case 1: //case 1 - token in follow_set
            return;
        default:
            error();    
    }
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
    int error_code = check_for_error(first_ro, follow_ro);
    switch(error_code)
    {
        case 0: //case 0 - token in first
        case 3: //case 3 - t_eof
            break;
        case 1: //case 1 - token in follow_set
            return;
        default:
            error();    
    }
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
    int error_code = check_for_error(first_ET, follow_ET);
    switch(error_code)
    {
        case 0: //case 0 - token in first
        case 3: //case 3 - t_eof
            break;
        case 1: //case 1 - token in follow_set
            return;
        default:
            error();    
    }
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
    // std::cout<<"upcoming token: "<<upcoming_token<<std::endl;
    program();
    return 0;
}
