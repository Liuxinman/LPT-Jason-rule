# LPT-Johnson-rule
This is a course project. It is created by Xinman Liu.

C language is used to implment longest processing time(LPT) rule and Johnson's rule.

- To compile the program, use command line:

      gcc -Wall -std=c99 -o a3 a3.c
    
- To generate 400 000 instances txt files, use command line:

      ./a3 -r
    
- To test whether ratio is calculated correctly or not, use command line:

      ./a3 -i <instancem_n_t1_t2_i.txt
    
    (or any other valid txt file name; and please delete all comment lines in the instances!)
    
    (it will print out ratio for this instance to stdout)
    
- To get result.txt, first, make a new directory "ratio" on the Desktop. Then, run shell script using this command line:

        sh run.sh
        
    or you can use:
    
        chmod 700 run.sh
        
        ./run.sh
