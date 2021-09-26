/* Simple ad-hoc scanner for the calculator language.
   Dies on lexical errors.
   Michael L. Scott, 2008-2021.
*/

// #include "stdio.h"
#include <iostream>
// #include "stdlib.h"
#include "string.h"
// #include "ctype.h"

#include "scan.h"

char token_image[MAX_TOKEN_LEN];

token scan()
{
    // static int c = ' ';
    char c;
    /* next available char; extra (int) width accommodates EOF */
    int i = 0; /* index into token_image */

    // /* skip white space */
    // while (isspace(c))
    // {
    //     c = getchar();
    // }
    std::cin.get(c);
    if (c == EOF)
        return t_eof;
    if (isalpha(c))
    {
        do
        {
            token_image[i++] = c;
            if (i >= MAX_TOKEN_LEN)
            {
                std::cerr << "max token length exceeded\n";
                exit(1);
            }
            std::cin.get(c);
        } while (isalpha(c) || isdigit(c) || c == '_');
        token_image[i] = '\0';
        if (!strcmp(token_image, "read"))
            return t_read;
        else if (!strcmp(token_image, "write"))
            return t_write;
          else if (!strcmp(token_image, "if"))
            return t_if;
              else if (!strcmp(token_image, "fi"))
            return t_fi;
         else if (!strcmp(token_image, "check"))
            return t_check;
         else if (!strcmp(token_image, "do"))
            return t_do;
         else if (!strcmp(token_image, "od"))
            return t_od;
        //  else if (!strcmp(token_image, "eof"))
        //     return t_eof;
        else
            return t_id;
    }
    else if (isdigit(c))
    {
        do
        {
            token_image[i++] = c;
            std::cin.get(c);
        } while (isdigit(c));
        token_image[i] = '\0';
        return t_literal;
    }
    else
        switch (c)
        {
        case ':':
            std::cin.get(c);
            if (c != '=')
            {

                fprintf(stderr,
                        "expected '=' after ':', got '%c' (0x%x)\n",
                        c, c);
                exit(1);
            }
            else
            {
                std::cin.get(c);
                return t_gets;
            }
            break;
        case '=':
            std::cin.get(c);


            if (c == '=')
            {
                std::cin.get(c);
                return t_dequals;
            }
            
            else
            {
               fprintf(stderr,
                        "expected '=' after '=', got '%c' (0x%x)\n",
                        c, c);
                exit(1);
            }
            break;
  case '$':
            std::cin.get(c);


            if (c == '$')
            {
                std::cin.get(c);
                return t_eof;
            }
         case '>':
            std::cin.get(c);


            if (c == '=')
            {
                std::cin.get(c);
                return t_ge;
            }

            
  if(c == ' ')
            {
               
                return t_gt;
            }
            
            else
            {
               fprintf(stderr,
                        "expected '=' after '=', got '%c' (0x%x)\n",
                        c, c);
                exit(1);
            }
            break;
 case '<':
            std::cin.get(c);


            if (c == '=')
            {
                std::cin.get(c);
                return t_ge;
            }

            
  if(c == ' ')
            {
               
                return t_lt;
            }
              if(c == '>')
            {
               std::cin.get(c);
                return t_nequal;
            }

             if(c == '=')
            {
               std::cin.get(c);
                return t_le;
            }
            
            else
            {
               fprintf(stderr,
                        "expected '=' after '=', got '%c' (0x%x)\n",
                        c, c);
                exit(1);
            }
            break;

            //  case '<':
            // std::cin.get(c);


            // if (c == '=')
            // {
            //     std::cin.get(c);
            //     return t_gets;
            // }

            // if (c == ' ')
            // {
            //     std::cin.get(c);
            //     return t_le;
            // }
            


        
        case '+':
                std::cin.get(c);
            return t_add;
        case '-':
                std::cin.get(c);
            return t_sub;
        case '*':
                std::cin.get(c);
            return t_mul;
        case '/':
                std::cin.get(c);
            return t_div;
         
                return t_eof;
           
            return t_add;
        case '(':
                std::cin.get(c);
            return t_lparen;
        case ')':
                std::cin.get(c);
            return t_rparen;
        
        
        default:
            return t_lexerror;
            // fprintf(stderr, "unexpected character '%c' (0x%x)\n", c, c);
            // exit(1);//handle lexical errors here
        }
}
