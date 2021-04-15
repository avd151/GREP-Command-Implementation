/*

File Name:
Author Name: Apurva Vijay Deshpande

*/

//Problem : GREP COMMAND IMPLEMENTATIPN

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include"grep.h"



int cflag = 0, hflag = 0, iflag = 0, lflag = 0, nflag = 0, vflag = 0, fflag = 0, wflag = 0, oflag = 0, dflag = 0, eflag = 0, xflag = 0;

int main(int argc, char*argv[]){
    int opt;
    int i_f = 0, i_p = 0, len_f = 0, len_p = 0;
    char* patterns[10];
    char* filenames[10];
    if(strcmp(argv[2], "--help") == 0){
            print_usage();
    }
    else if (argc < 3){
        printf("GREP: Improper number of arguments\nType grep--help for usage instructions\n");
        return 1;
    }
    else{
        while((opt = getopt(argc, argv, "dchilnvwox?d:e"))!= -1){
            switch(opt){
            case 'd':
                dflag = 1;
                break;
            case 'c':
                cflag = 1;
                break;
            case 'h':
                hflag = 1;
                break;
            case 'i':
                iflag = 1;
                break;
            case 'l':
                lflag = 1;
                break;
            case 'n':
                nflag = 1;
                break;
            case 'v':
                vflag = 1;
                break;
            case 'w':
                wflag = 1;
                break;
            case 'o':
                oflag = 1;
                break;
            case 'e':
                eflag = 1;
                break;
            case 'x':
                xflag = 1;
                break;
            case '?':
                printf("Unknown Command: grep\nType grep--help for usage instructions\n\n");
                cflag++;
                break;
            default:
                printf("Unknown option\nType grep--help for usage instructions\n ");
            }
        }
        //Extract Files and Patterns from Command Line Arguments
        for(; optind < argc; optind++){
            char* argument = argv[optind];
            if(isFile(argument)){
                filenames[i_f] = argument;
                i_f++;
            }
            else{
                patterns[i_p] = argument;
                i_p++;
            }
        }
        //Length of Pattern and File arrays
        len_f = i_f;
        len_p = i_p;

        //-d = default grep command
        if(dflag != 0){
            i_p = i_f = 0;
            printf("\n-d : Default Grep Command\n");
            while(i_p < len_p){
                i_f = 0;
                while(i_f < len_f){
                    printf("\nOccurrence of '%s' in file: %s -\n", patterns[i_p], filenames[i_f]);
                    d_grep(filenames[i_f], patterns[i_p]);
                    i_f++;
                }
                i_p++;
                printf("\n");
            }
        }

        // -i = Ignore Case for Matching
        if(iflag != 0){
            i_p = i_f = 0;
            printf("\n-i: Ignore Case for Matching\n");
            while(i_p < len_p){
                i_f = 0;
                while(i_f < len_f){
                    printf("\nOccurrence of '%s'(case insensitive) in file: %s -\n", patterns[i_p], filenames[i_f]);
                    i_grep(filenames[i_f], patterns[i_p]);
                    i_f++;
                }
                i_p++;
            }
        }

        //-c = Count lines that Match Pattern
        if(cflag != 0){
            i_p = i_f = 0;
            printf("\nCount lines that Match Pattern\n");
            while(i_f < len_f){
                i_p = 0;
                while(i_p < len_p){
                    c_grep(filenames[i_f], patterns[i_p]);
                    i_p++;
                }
                i_f++;
            }
        }

        //-h = Display the matched lines, but do not display the filenames.
        if(hflag != 0){
            i_p = i_f = 0;
            printf("\nDisplay the matched lines, but do not display the filenames.\n");
            while(i_p < len_p){
                i_f = 0;
                printf("\nLines in %s having '%s' are- \n", filenames[i_f], patterns[i_p]);
                while(i_f < len_f){
                    h_grep(filenames[i_f], patterns[i_p]);
                    i_f++;
                }
                i_p++;
            }
        }

        //-l = Display the matched lines, but do not display the filenames.
        if(lflag != 0){
            i_p = i_f = 0;
            printf("\nDisplay the matched lines, but do not display the filenames.\n");
            while(i_p < len_p){
                i_f = 0;
                while(i_f < len_f){
                    printf("\nMatched Lines in %s having occurrence of '%s' are:\n", filenames[i_f], patterns[i_p]);
                    if(l_grep(filenames[i_f], patterns[i_p]))
                        printf("%s\n", filenames[i_f]);
                    i_f++;
                }
                i_p++;
            }
        }

        // -v = prints out all the lines that do not matches the pattern
        if(vflag != 0){
            i_p = i_f = 0;
            printf("This prints out all the lines that do not matches the pattern\n\n");
            while(i_p < len_p){
                i_f = 0;
                while(i_f < len_f){
                    printf("\nLines in %s which do not have occurrence of '%s' are- \n", filenames[i_f], patterns[i_p]);
                    v_grep(filenames[i_f], patterns[i_p]);
                    i_f++;
                }
                i_p++;
            }
        }

         // -n = Match whole/ exact word
        if(wflag != 0){
            i_p = i_f = 0;
            printf("\nMatch whole word\n");
            while(i_p < len_p){
                i_f = 0;
                while(i_f < len_f){
                    printf("\n Lines in %s having occurrence of exact word: '%s' are-\n", filenames[i_f], patterns[i_p]);
                    w_grep(filenames[i_f], patterns[i_p]);
                    i_f++;
                }
                i_p++;
            }
        }

        // -n = Display the matched lines and their line numbers.
        if(nflag != 0){
            i_p = i_f = 0;
            printf("\nDisplay the matched lines with their line numbers.\n");
            while(i_p < len_p){
                i_f = 0;
                while(i_f < len_f){
                    printf("\nLines in %s having occurrence of exact word: '%s' are:\n", filenames[i_f], patterns[i_p]);
                    n_grep(filenames[i_f], patterns[i_p]);
                    i_f++;
                }
                i_p++;
            }
        }

        // -o = Print only the matched parts of a matching line, with each such part on a separate output line.
        if(oflag != 0){
            i_p = i_f = 0;
            printf("\nPrint only the matched parts of a matching line, with each such part on a separate output line.\n");
            while(i_p < len_p){
                i_f = 0;
                while(i_f < len_f){
                    printf("\nMatched parts of %s having '%s'  are: \n", filenames[i_f], patterns[i_p]);
                    o_grep(filenames[i_f], patterns[i_p]);
                    i_f++;
                }
                i_p++;
            }
        }

        // -x : Print Exact location of Pattern found in the file(s)
        if(xflag != 0){
            i_p = i_f = 0;
            printf("\nPrint Exact location of Pattern found in the file(s)\n");
            while(i_p < len_p){
                i_f = 0;
                while(i_f < len_f){
                    printf("\nPrint Exact location of '%s' found in %s are:\n", filenames[i_f], patterns[i_p]);
                    x_grep(filenames[i_f], patterns[i_p]);
                    i_f++;
                }
                i_p++;
            }
        }
        /*
        //-e = Evaluate Regular Expression
        if(eflag != 0){
            i_p = i_f = 0;
            printf("\nEvaluate Regular Expression\n\n");
            while(i_p < len_p){
                i_f = 0;
                while(i_f < len_f){
                   check_regex(filenames[i_f], patterns[i_p]);
                    i_f++;
                }
                i_p++;
            }
        }
        */
        return 0;
    }

}
