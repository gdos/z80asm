//-----------------------------------------------------------------------------
// z80asm Lemon grammar
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

%include {
#include "lemon.h"
#include <iostream>
}  
   
%token_type 	{int}
%token_prefix	TK_
%extra_argument	{ Parser* parser }

// operator precedence
%left PLUS MINUS.   
%left DIVIDE TIMES.  

%syntax_error {  
  std::cout << "Syntax error!" << std::endl;  
}   
   
program ::= expr(A).   { std::cout << "Result=" << A << std::endl; }  
   
expr(A) ::= expr(B) MINUS  expr(C).   { A = B - C; }  
expr(A) ::= expr(B) PLUS  expr(C).   { A = B + C; }  
expr(A) ::= expr(B) TIMES  expr(C).   { A = B * C; }  
expr(A) ::= expr(B) DIVIDE expr(C).  { 

         if(C != 0){
           A = B / C;
          }else{
           std::cout << "divide by zero" << std::endl;
           }
}  /* end of DIVIDE */

expr(A) ::= INTEGER(B). { A = B; }
