/* Simple ad-hoc scanner for the calculator language.
   Dies on lexical errors.
   Michael L. Scott, 2008-2021.
*/

// #include "stdio.h"
#include <iostream>
// #include "stdlib.h"
#include "string.h"
// #include "ctype.h"
#include <fstream>
#include "scan.h"

char token_image[MAX_TOKEN_LEN];

token scan(int &location, std::string &name)
{
    // static int c = ' ';
    char c;
    /* next available char; extra (int) width accommodates EOF */
    int i = 0; /* index into token_image */

std::ifstream input_file(name);
 input_file.seekg(location);
c = input_file.get();  location =  input_file.tellg( );


    // /* skip white space */
    while (isspace(c))
    {  
        c = input_file.tellg();
    }
   
    if (c == EOF){
         location =  input_file.tellg( );
              return t_eof;
    }
   
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
         c = input_file.get();  location =  input_file.tellg( );  
        } while (isalpha(c) || isdigit(c) || c == '_');
        token_image[i] = '\0';

        location =  input_file.tellg( );
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
         c = input_file.get();  location =  input_file.tellg( );
        } while (isdigit(c));
        token_image[i] = '\0';
          location =  input_file.tellg( );
        return t_literal;
    }
    else
        switch (c)
        {
        case ':':
         c = input_file.get();  location =  input_file.tellg( );  
            if (c != '=')
            {

                // fprintf(stderr,
                //         "expected '=' after ':', got '%c' (0x%x)\n",
                //         c, c);

                            std::cerr << "Unexpected charcater"<<c<<" (0x <<c";
                // exit(1);
            }
            else
            {
             c = input_file.get();  location =  input_file.tellg( );   
                
                return t_gets;
            }
            break;
        case '=':
         c = input_file.get();  location =  input_file.tellg( );  


            if (c == '=')
            {
             c = input_file.get();  location =  input_file.tellg( );   
                 location =  input_file.tellg( );
                return t_dequals;
            }
            
            else
            {
            //    fprintf(stderr,
            //             "expected '=' after '=', got '%c' (0x%x)\n",
            //             c, c);
                            std::cerr << "Unexpected charcater"<<c<<" (0x <<c";
                // exit(1);
            }
            break;
  case '$':
         c = input_file.get();  location =  input_file.tellg( );   

            if (c == '$')
            {
             c = input_file.get();  location =  input_file.tellg( );   
                return t_eof;
            }
         case '>':
        c = input_file.get();  location =  input_file.tellg( );   


            if (c == '=')
            {
             c = input_file.get();  location =  input_file.tellg( );  
                return t_ge;
            }

            
  if(c == ' ')
            {
               
                return t_gt;
            }
            
            else
            {
            //    fprintf(stderr,
            //             "expected '=' after '=', got '%c' (0x%x)\n",
            //             c, c);
                            std::cerr << "Unexpected charcater"<<c<<" (0x <<c";
                // exit(1);
            }
            break;
 case '<':
         c = input_file.get();  location =  input_file.tellg( );  


            if (c == '=')
            {
             c = input_file.get();  location =  input_file.tellg( );   
                return t_ge;
            }

            
  if(c == ' ')
            {
               
                return t_lt;
            }
              if(c == '>')
            {
            c = input_file.get();  location =  input_file.tellg( );   
                return t_nequal;
            }

             if(c == '=')
            {
            c = input_file.get();  location =  input_file.tellg( );   
                return t_le;
            }
            
            else
            {
            //    fprintf(stderr,
            //             "expected '=' after '=', got '%c' (0x%x)\n",
            //             c, c);
                            std::cerr << "Unexpected charcater"<<c<<" (0x <<c";
                exit(1);
            }
            break;

            //  case '<':
            //c = input_file.get();  location =  input_file.tellg( );


            // if (c == '=')
            // {
            //  c = input_file.get();  location =  input_file.tellg( );  location =  input_file.tellg( );  location =  input_file.tellg( );
            //     return t_gets;
            // }

            // if (c == ' ')
            // {
            //  c = input_file.get();  location =  input_file.tellg( );  location =  input_file.tellg( );  location =  input_file.tellg( );
            //     return t_le;
            // }
            


        
        case '+':
             c = input_file.get();  location =  input_file.tellg( );   
            return t_add;
        case '-':
             c = input_file.get();  location =  input_file.tellg( );  
            return t_sub;
        case '*':
             c = input_file.get();  location =  input_file.tellg( );   
            return t_mul;
        case '/':
             c = input_file.get();  location =  input_file.tellg( ); 
            return t_div;
         
                return t_eof;
           
            return t_add;
        case '(':
             c = input_file.get();  location =  input_file.tellg( );   
            return t_lparen;
        case ')':
             c = input_file.get();  location =  input_file.tellg( );  
            return t_rparen;
        
        
        default:
            // return t_lexerror;
         //   fprintf(stderr, "unexpected character '%c' (0x%x)\n", c, c);
            std::cerr << "Unexpected charcater"<<c<<" (0x <<c";
            // exit(1);//handle lexical errors here
        }
}
