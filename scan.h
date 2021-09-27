/* Definitions the scanner shares with the parser.
   Michael L. Scott, 2008-2021.
*/

enum token {t_read, t_write, t_id, t_literal, t_gets, t_add, 
t_sub, t_mul, t_div, t_lparen, t_rparen, t_eof, t_if, t_fi, t_do, t_od,t_check, 
t_dequals, t_nequal, t_lt, t_gt, t_le, t_ge, t_lexerror};

#define MAX_TOKEN_LEN 200
extern char token_image[MAX_TOKEN_LEN];
extern enum token scan(int &location, std::string &name);




