This code was worked on by:
Andrew Thankson
Rebero Prince

How to run the program: 
the folder contains two sample text files..... test1.txt and test2.txt... these  can be parsed as argments to run the program. To run the program, 
            run: 
                make 
                make run ARGS="test1.txt"   
             //   or  if you want to test test2.txt
                make run ARGS="test2.txt"  

   to clean the file run:
                make clean 


Extra credit:

1. We  translated the program in c 
2. Made makefile to compile  the program. 

Sample syntax Tree  output-- 

test1.txt :
 (program [(read "n")(:= "cp"( (num "2")do ))(do [(check ( (id "n")n> (num "0") ))(:= "found"( (num "0")cf1 ))(:= "cf1"( (num "2")cf1s ))(:= "cf1s"( (* (id "cf1")(id "cf1"))do ))(do [(check ( (id "cf1s")cf1s>= (id "cp") ))(:= "cf2"( (num "2")pr ))(:= "pr"( (* (id "cf1")(id "cf2"))do ))(do [(check ( (id "pr")pr>= (id "cp") ))(if (( (id "pr")pr== (id "cp") ))[(:= "found"( (num "1")fi ))])(:= "cf2"( (+ (id "cf2")(num "1"))pr ))(:= "pr"( (* (id "cf1")(id "cf2"))od ))])(:= "cf1"( (+ (id "cf1")(num "1"))cf1s ))(:= "cf1s"( (* (id "cf1")(id "cf1"))od ))])(if (( (id "found")found== (num "0") ))[(write ( (id "cp")n ))(:= "n"( (- (id "n")(num "1"))fi ))])(:= "cp"( (+ (id "cp")(num "1"))od ))])])
test2.txt :
(program [(read "n")(:= "cp"( (num "2")if ))(if (( (id "n")n> (num "0") ))[(read "n")])])

bugs:
when the input is entered through terminal, the program works smoothly no error. However, when we move the code into a file, there is one error " unexpected charcter" after it 
the code has passed $$.  

 

