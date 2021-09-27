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

std::string program();
std::string stmt_list();
std::string stmt();
std::string relation();
std::string expr();
std::string term();
std::string term_tail(std::string t);
std::string factor();
std::string expr_tail();
std::string factor_tail(std::string f);
std::string add_op();
std::string mul_op();
std::string rel_op();

std::string program()
{
    std::string syntax_tree = "";
    int error_code = check_for_error(first_P, follow_P);
    switch(error_code)
    {
        case 0: //case 0 - token in first
        case 3: //case 3 - t_eof
            break;
        case 1: //case 1 - token in follow_set
            return syntax_tree;
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
        syntax_tree+="(program [";
        // puts("predict program --> stmt_list eof\n");
        std::cout<<"predict program --> stmt_list eof\n";
        syntax_tree += stmt_list();
        match(t_eof);
        syntax_tree += "])";
        break;
    default:
        error();
    }
    return syntax_tree;
}

std::string stmt_list()
{
    std::string syntax_tree = "";
    std::string slist = "";
    int error_code = check_for_error(first_SL, follow_SL);
    switch(error_code)
    {
        case 0: //case 0 - token in first
        case 3: //case 3 - t_eof
            break;
        case 1: //case 1 - token in follow_set
            return syntax_tree;
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
        syntax_tree+=stmt();
        slist+=stmt_list();
        syntax_tree+=slist;
        break;
    case t_eof:
    case t_fi:
    case t_od:
        std::cout<<"predict stmt_list --> epsilon\n";
        break; /* epsilon production */
    default:
        error();
    }

    return syntax_tree;
}

std::string stmt()
{
    std::string syntax_tree = "";
    int error_code = check_for_error(first_S, follow_S);
    switch(error_code)
    {
        case 0: //case 0 - token in first
        case 3: //case 3 - t_eof
            break;
        case 1: //case 1 - token in follow_set
            return syntax_tree;
        default:
            error();    
    }
    switch (upcoming_token)
    {
    case t_id:
        std::cout<<"predict stmt --> id gets relation\n";
        syntax_tree+="(:= \"";
        syntax_tree+=token_image;
        syntax_tree += "\"";
        match(t_id);
        match(t_gets);        
        syntax_tree+=relation();
        syntax_tree+=")";
        break;
    case t_read:
        std::cout<<"predict stmt --> read id\n";
        syntax_tree+="(read ";
        syntax_tree+="\"";
        match(t_read);
        syntax_tree+=token_image;
        match(t_id);
        syntax_tree += "\")";
        break;
    case t_write:
        std::cout<<"predict stmt --> write relation\n";
        syntax_tree+="(write ";
        match(t_write); 
        syntax_tree+=relation();
        syntax_tree+=")";
        break;
    case t_if:
        std::cout<<"predict stmt --> if relation stmt_list fi\n";
        syntax_tree+="(if (";
        match(t_if);      
        syntax_tree+=relation();
        syntax_tree+=")[";
        syntax_tree+=stmt_list();
        syntax_tree+="])";
        match(t_fi);
        break;
    case t_do:
        std::cout<<"predict stmt --> do stmt_list od\n";
        match(t_do);
        syntax_tree+="(do [";
        syntax_tree+=stmt_list();
        match(t_od);
        syntax_tree+="])";
        break;
    case t_check:
        std::cout<<"predict stmt --> check relation\n";
        syntax_tree+="(check ";
        match(t_check);
        syntax_tree+=relation();  
        syntax_tree+=")";
        break;
    default:
        error();
    }

    return syntax_tree;
}

std::string relation(){//R	→ 	E ET
    std::string syntax_tree = "";
    std::string exprr = "";
    int error_code = check_for_error(first_R, follow_R);
    switch(error_code)
    {
        case 0: //case 0 - token in first
        case 3: //case 3 - t_eof
            break;
        case 1: //case 1 - token in follow_set
            return syntax_tree;
        default:
            error();    
    }
    switch (upcoming_token)
    {
       
        case t_id:
        case t_lparen:
        case t_literal:
            std::cout<<"predict relation -> expr expr_tail\n";
            syntax_tree +="( ";
            syntax_tree+= expr();
            syntax_tree+=token_image;
            syntax_tree+=expr_tail();
            syntax_tree +=" )";
            break;
        default:
            error();
    }

    return syntax_tree;
}

std::string expr()
{
    std::string syntax_tree = "";
    std::string t = "";
    std::string tt = "";
    int error_code = check_for_error(first_E, follow_E);
    switch(error_code)
    {
        case 0: //case 0 - token in first
        case 3: //case 3 - t_eof
            break;
        case 1: //case 1 - token in follow_set
            return syntax_tree;
        default:
            error();    
    }
    switch (upcoming_token)
    {
    case t_id:
    case t_literal:
    case t_lparen:
        std::cout<<"predict expr --> term term_tail\n";
        t+=term();
        tt+=term_tail(t);
        syntax_tree+=tt;
        if (t.compare(tt) != 0) syntax_tree += ")";
        //syntax_tree+=")";
        break;
    default:
        error();
    }

    return syntax_tree;
}

std::string term()
{
    std::string syntax_tree = "";
    std::string f = "";
    std::string ft = "";
    int error_code = check_for_error(first_T, follow_T);
    switch(error_code)
    {
        case 0: //case 0 - token in first
        case 3: //case 3 - t_eof
            break;
        case 1: //case 1 - token in follow_set
            return syntax_tree;
        default:
            error();    
    }
    switch (upcoming_token)
    {
    case t_id:
    case t_literal:
    case t_lparen:
        std::cout<<"predict term --> factor factor_tail\n";
        f+=factor();
        ft+=factor_tail(f);
        syntax_tree+=ft;
        if (f.compare(ft) != 0) syntax_tree += ")";
        //syntax_tree+=")";
        break;
    default:
        error();
    }

    return syntax_tree;
}

std::string term_tail(std::string t)
{
    std::string syntax_tree = "";
    std::string term_l = "";
    std::string sb = "";
    int error_code = check_for_error(first_TT, follow_TT);
    switch(error_code)
    {
        case 0: //case 0 - token in first
        case 3: //case 3 - t_eof
            break;
        case 1: //case 1 - token in follow_set
            return t;
        default:
            error();    
    }
    switch (upcoming_token)
    {
    case t_add:
    case t_sub:
        std::cout<<"predict term_tail --> add_op term term_tail\n";
        syntax_tree+="(";
        // syntax_tree+=add_op();
        // syntax_tree+= t + " ";
        // term_l+=term();
        sb = add_op()+t+term();

        // std::string sb = ( + t term)

        syntax_tree+=term_tail(sb);
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
        syntax_tree+=t;
        break; /* epsilon production */
    default:
        syntax_tree+=t;
        error();
    }

    return syntax_tree;
}

std::string factor()
{
    std::string syntax_tree = "";
    int error_code = check_for_error(first_F, follow_F);
    switch(error_code)
    {
        case 0: //case 0 - token in first
        case 3: //case 3 - t_eof
            break;
        case 1: //case 1 - token in follow_set
            return syntax_tree;
        default:
            error();    
    }
    switch (upcoming_token)
    {
    case t_literal:
        std::cout<<"predict factor --> literal\n";
        syntax_tree+="(num \"";
        syntax_tree+= token_image;
        std::cout<<"here"<<token_image<<std::endl;
        syntax_tree+="\")";
        match(t_literal);
        break;
    case t_id:
        std::cout<<"predict factor --> id\n";
        syntax_tree+="(id \"";
        syntax_tree+= token_image;
        syntax_tree+="\")";
        match(t_id);
        std::cout<<"here"<<token_image<<std::endl;
        break;
    case t_lparen:
        std::cout<<"predict factor --> lparen relation rparen\n";
        match(t_lparen);
        syntax_tree+="(";
        syntax_tree+=relation();
        match(t_rparen);
        syntax_tree+=")";
        break;
    default:
        error();
    }

    return syntax_tree;
}

std::string factor_tail(std::string f)
{
    std::string syntax_tree="";
    std::string ftor = "";
    std::string ftail = "";
    std::string st = "";
    int error_code = check_for_error(first_FT, follow_FT);
    switch(error_code)
    {
        case 0: //case 0 - token in first
        case 3: //case 3 - t_eof
            break;
        case 1: //case 1 - token in follow_set
            return f;
        default:
            error();    
    }
    switch (upcoming_token)
    {
    case t_mul:
    case t_div:
        std::cout<<"predict factor_tail --> mul_op factor factor_tail\n";
        syntax_tree+="(";
        // syntax_tree+=mul_op();
        st = mul_op()+f+factor();
        std::cout<<"st"<<st<<std::endl;
        // syntax_tree+= f + " ";
        // ftor+=factor();
        // ftail+=factor_tail(ftor);
        syntax_tree+=factor_tail(st);
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
        syntax_tree+= f;
        break; /* epsilon production */
    default:
        syntax_tree+= f;
        error();
    }

    return syntax_tree;
}

std::string add_op()
{
    std::string syntax_tree = "";
    int error_code = check_for_error(first_ao, follow_ao);
    switch(error_code)
    {
        case 0: //case 0 - token in first
        case 3: //case 3 - t_eof
            break;
        case 1: //case 1 - token in follow_set
            return syntax_tree;
        default:
            error();    
    }
    switch (upcoming_token)
    {
    case t_add:
        std::cout<<"predict add_op --> add\n";
        match(t_add);
        syntax_tree+="+ ";
        break;
    case t_sub:
        std::cout<<"predict add_op --> sub\n";
        match(t_sub);
        syntax_tree+="- ";
        break;
    default:
        error();
    }
    return syntax_tree;
}

std::string mul_op()
{
    std::string syntax_tree="";
    int error_code = check_for_error(first_mo, follow_mo);
    switch(error_code)
    {
        case 0: //case 0 - token in first
        case 3: //case 3 - t_eof
            break;
        case 1: //case 1 - token in follow_set
            return syntax_tree;
        default:
            error();    
    }
    switch (upcoming_token)
    {
    case t_mul:
        std::cout<<"predict mul_op --> mul\n";
        match(t_mul);
        syntax_tree+="* ";
        break;
    case t_div:
        std::cout<<"predict mul_op --> div\n";
        match(t_div);
        syntax_tree+="/ ";
        break;
    default:
        error();
    }

    return syntax_tree;
}

std::string rel_op()
{
    std::string syntax_tree="";
    int error_code = check_for_error(first_ro, follow_ro);
    switch(error_code)
    {
        case 0: //case 0 - token in first
        case 3: //case 3 - t_eof
            break;
        case 1: //case 1 - token in follow_set
            return syntax_tree;
        default:
            error();    
    }
    switch (upcoming_token)
    {
    case t_dequals:
        std::cout<<"predict rel_op --> dequals\n";
        syntax_tree+="== ";
        match(t_dequals);
        
        break;
    case t_nequal:
        std::cout<<"predict rel_op --> t_nequal\n";
        match(t_nequal);
        syntax_tree+="<> ";
        break;
    case t_lt:
        std::cout<<"predict rel_op --> t_lt\n";
        match(t_lt);
        syntax_tree+="< ";
        break;
    case t_gt:
        std::cout<<"predict rel_op --> t_gt\n";
        match(t_gt);
        syntax_tree+="> ";
        break;
    case t_le:
        std::cout<<"predict rel_op --> t_le\n";
        match(t_le);
        syntax_tree+="<= ";
        break;
    case t_ge:
        std::cout<<"predict rel_op --> t_ge\n";
        match(t_ge);
        syntax_tree+=">= ";
        break;
    default:
        error();
    }

    return syntax_tree;

}

std::string expr_tail()
{
    std::string syntax_tree = "";
    int error_code = check_for_error(first_ET, follow_ET);
    switch(error_code)
    {
        case 0: //case 0 - token in first
        case 3: //case 3 - t_eof
            break;
        case 1: //case 1 - token in follow_set
            return syntax_tree;
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
        syntax_tree+=rel_op();
        syntax_tree+=expr();
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

    return syntax_tree;

}

int main()
{
    upcoming_token = scan();
    // std::cout<<"upcoming token: "<<upcoming_token<<std::endl;
    std::string syntax_tree = program();
    std::cout<<"SYNTAX TREE:"<<syntax_tree<<std::endl;
    return 0;
}
