#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<string.h>
#include<ctype.h>
#include<dirent.h>
#include"grep.h"

//Initialise DLL
void init_ll(Nodeptr l){
    (l) = NULL;
}

Nodeptr appendAtTail_ll(Nodeptr tail, char *line, int lineNum, int patNum){

    //if this is 1st occurrence on current line (line) so create new node
    if(tail == NULL || tail->lineNum != lineNum){
       Nodeptr newTail = (Node*)malloc(sizeof(Node));
        strcpy(newTail->line, line); // copy line having pattern in line of Node structure
        newTail->lineNum = lineNum; //lineNum = line number
        newTail->next = NULL;
        newTail->PatHead = (PatNode*)malloc(sizeof(PatNode));
        newTail->PatTail = newTail->PatHead;
        newTail->PatTail->patNum = patNum; //patnum = count of pattern in given line
        newTail->PatTail->next = NULL;

        //
        if(tail)
            tail->next = newTail;
        return newTail;
    }
    //if line has 1 occurrence of the pattern add patNum to current tail's pattern list - to show number of ocurrances of pattern in given line
    else{
        Patptr newPat = (PatNode*)malloc(sizeof(PatNode));
        newPat->patNum = patNum;
        newPat->next = NULL;
        tail->PatTail->next = newPat;
        tail->PatTail = newPat;
        return tail;
    }
}

Nodeptr removeFromHead_ll(Nodeptr head, char *line, int *lineNum, Patptr *patNum){
    Node *next = head->next;

    strcpy(line, head->line);
    *lineNum = head->lineNum;
    *patNum = head->PatHead;
    head->PatHead = head->PatHead->next;
    free(head);
    if(next == NULL)
        return NULL;
    return next;
}

void print_ll(Nodeptr head){
    char pLine[MaxLineLen]; //store line having given pattern in char array/string
    int pLineNum = 0; //store line number of given pattern
    Patptr pPatNum; //point to node having the pattern - to show exactly whree node is in the given line

    while(head){
        head = removeFromHead_ll(head, pLine, &pLineNum, &pPatNum);
        printf("Line %d : ", pLineNum+1);
        //to print exact location of string in the line where the pattern is found
        while(pPatNum){
            PatNode *temp = pPatNum;
            printf("%d ", pPatNum->patNum);
            pPatNum = pPatNum->next;
            free(temp);
        }
        printf(" %s", pLine);
    }
}

void delete_ll(Nodeptr head){
    Nodeptr temp = (Node*)malloc(sizeof(Node));
    while(head != NULL){
        temp = head;
        free(temp->line);
        free(temp);
        head = (head)->next;
    }
}

//Grep Command Functions

int stricmp(char const *a, char const *b){
    for (;; a++, b++) {
        int d = tolower((unsigned char)*a) - tolower((unsigned char)*b);
        if (d != 0 || !*a)
            return d;
    }
}
void printLines(Node *head) {
    char pLine[MaxLineLen];
    int pLineNum = 0;
    PatNode *pWordNums;

    while(head){
        head = removeFromHead_ll(head, pLine, &pLineNum, &pWordNums);
        printf(" %s",pLine);
    }

}

int isFile(char* filename){
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    if(fp == NULL)
        return 0;

    else
        return 1;
}

void print_usage(){
    printf("Usage: [OPTION] PATTERN [FILE]\nSearch for PATTERN in each FILE.\nExample: grep -i 'hello world' menu.h main.c\n");
    printf("Functionalities/Options I have Implemented along with their Usage are:\n");
    printf("-d: Default grep command without any options\n");
    printf("-c : This prints only a count of the lines that match a pattern\n");
    printf("-h : Display the matched lines, but do not display the filenames.\n");
    printf("-i : Ignores, case for matching\n");
    printf("-l : Displays list of a filenames only.\n");
    printf("-n : Display the matched lines and their line numbers.\n");
    printf("-v : This prints out all the lines that do not matches the pattern\n");
    printf("-w : Match whole word\n");
    printf("-o : Print only the matched parts of a matching line, with each such part on a separate output line.\n");

}

void d_grep(char *filename, char *pattern){
    Node *head = NULL;
    Node *tail = NULL;
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("GREP: Unable to open file: %s\n", filename);
    }
    else{
        char line[MaxLineLen];
        char fullLine[MaxLineLen];
        char *delim = " .,?!;:()\"\n";
        int lineNum = 0;

        while (fgets(line, MaxLineLen, fp) != NULL){
            int wordNum = 0;
            char *ptr;
            strcpy(fullLine,line);
            ptr = strtok(line, delim);

            while (ptr){
                if (strstr(ptr, pattern) != NULL){
                    tail = appendAtTail_ll(tail, fullLine, lineNum, wordNum);
                    if (!head)
                        head = tail;
                    }
                    ptr = strtok(NULL, delim);
                    wordNum++;
            }
                lineNum++;
        }
        char pLine[MaxLineLen];
        int pLineNum = 0;
        PatNode *pWordNums;

        while(head){
            head = removeFromHead_ll(head, pLine, &pLineNum, &pWordNums);
            printf("%s: %s", filename, pLine);
        }
        fclose(fp);
    }
}

void longest_line_pattern(char *filename, char *pattern){
    Node *head = NULL;
    Node *tail = NULL;
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("GREP: Unable to open file: %s\n", filename);
    }
    else{
        char line[MaxLineLen];
        char fullLine[MaxLineLen];
        char *delim = " .,?!;:()\"\n";
        int lineNum = 0;
        int longestLineNum = 0;
        char longestLinePattern[MaxLineLen];

        while (fgets(line, MaxLineLen, fp) != NULL){
            int wordNum = 0;
            int lineCount = strlen(line);
            char *ptr;
            strcpy(fullLine,line);
            ptr = strtok(line, delim);
            if (lineCount > longestLineNum){
                        longestLineNum = lineCount;
                        strcpy(longestLinePattern,fullLine);
                    }


            while (ptr){
                if (strstr(ptr, pattern) != NULL){
                    tail = appendAtTail_ll(tail, fullLine, lineNum, wordNum);
                    if (!head)
                        head = tail;
                }
                ptr = strtok(NULL, delim);
                wordNum++;
            }
                lineNum++;
        }
        printf("\nLongest Line having '%s' in %s is: %sIt has %d characters", pattern, filename, longestLinePattern, longestLineNum);
        fclose(fp);
    }
}

void h_grep(char *filename, char *pattern){
    Node *head = NULL;
    Node *tail = NULL;
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("GREP: Unable to open file: %s\n", filename);
    }
    else{
        char line[MaxLineLen];
        char fullLine[MaxLineLen];
        char *delim = " .,?!;:()\"\n";
        int lineNum = 0;

        while (fgets(line, MaxLineLen, fp) != NULL){

            int wordNum = 0;
            char *ptr;
            strcpy(fullLine,line);
            ptr = strtok(line, delim);

            while (ptr){
                if (strcmp(ptr,pattern) == 0){
                    tail = appendAtTail_ll(tail, fullLine, lineNum, wordNum);
                    if (!head)
                        head = tail;
                    }
                    ptr = strtok(NULL, delim);
                    wordNum++;
            }
                lineNum++;
        }

        char pLine[MaxLineLen];
        int pLineNum = 0;
        PatNode *pWordNums;

        while(head){
            head = removeFromHead_ll(head, pLine, &pLineNum, &pWordNums);
            printf(" %s",pLine);
        }

        delete_ll(head);
        fclose(fp);
    }
}

void c_grep(char *filename, char *pattern){
    Node *head = NULL;
    Node *tail = NULL;
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("GREP: Unable to open file: %s\n", filename);
    }
     else{
        char line[MaxLineLen];
        char fullLine[MaxLineLen];
        char *delim = " .,?!;:()\"\n";
        int lineNum = 0;
        int pat_occur = 0, ans = 0, pat_line_occur = 0;
        while (fgets(line, MaxLineLen, fp) != NULL){
            ans = 0;
            int wordNum = 0;
            char *ptr;
            strcpy(fullLine,line);
            ptr = strtok(line, delim);

            while (ptr){
                if (strcmp(ptr,pattern) == 0){
                    tail = appendAtTail_ll(tail, fullLine, lineNum, wordNum);
                    if (!head){
                        head = tail;
                    }
                    ans = 1;
                    pat_occur++;
                }
                ptr = strtok(NULL, delim);
                wordNum++;
            }
            if(ans)
                pat_line_occur++;
            lineNum++;
        }
        if(head == NULL){
            printf("Total %d occurrences of %s found in %s\n", pat_occur, pattern, filename);
            printf("Total %d line occurrences of %s found in %s\n", pat_line_occur, pattern, filename);
            fclose(fp);
            return;
        }
        printf("Total %d occurrences of %s found in %s\n", pat_occur, pattern, filename);
        printf("Total %d line occurrences of %s found in %s\n", pat_line_occur, pattern, filename);
        fclose(fp);
    }
}

void v_grep(char *filename, char *pattern){
    Node *head = NULL;
    Node *tail = NULL;
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("GREP: Unable to open file: %s\n", filename);
    }
    else{
        char line[MaxLineLen];
        char fullLine[MaxLineLen];
        char *delim = " .,?!;:()\"\n";
        int lineNum = 0;
        int ans = 0;
        while (fgets(line, MaxLineLen, fp) != NULL){
            ans = 0;
            int wordNum = 0;
            char *ptr;
            strcpy(fullLine,line);
            ptr = strtok(line, delim);

            while (ptr){
                if (strcmp(ptr,pattern) == 0){
                    ans = 1;
                }
                ptr = strtok(NULL, delim);
                wordNum++;
            }
            if(ans == 0){
                tail = appendAtTail_ll(tail, fullLine, lineNum, wordNum);
                if (!head)
                    head = tail;
            }

            lineNum++;
        }

        char pLine[MaxLineLen];
        int pLineNum = 0;
        PatNode *pWordNums;

        while(head){
            head = removeFromHead_ll(head, pLine, &pLineNum, &pWordNums);
            printf("%s: Line %d: %s",filename, pLineNum, pLine);
        }
        fclose(fp);
    }
}

void i_grep(char *filename, char *pattern){
    Node *head = NULL;
    Node *tail = NULL;
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("GREP: Unable to open file: %s\n", filename);
    }
    else{
        char line[MaxLineLen];
        char fullLine[MaxLineLen];
        char *delim = " .,?!;:()\"\n";
        int lineNum = 0;

        while (fgets(line, MaxLineLen, fp) != NULL){

            int wordNum = 0;
            char *ptr;
            strcpy(fullLine,line);
            ptr = strtok(line, delim);

            while (ptr){
                if (stricmp(ptr,pattern) == 0){
                    tail = appendAtTail_ll(tail, fullLine, lineNum, wordNum);
                    if (!head)
                        head = tail;
                    }
                    ptr = strtok(NULL, delim);
                    wordNum++;
            }
                lineNum++;
        }
        char pLine[MaxLineLen];
        int pLineNum = 0;
        PatNode *pWordNums;

        while(head){
            head = removeFromHead_ll(head, pLine, &pLineNum, &pWordNums);
            printf("%s: %s",filename, pLine);
        }
        delete_ll(head);
        fclose(fp);
    }
}

int l_grep(char *filename, char *pattern){
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    int ans = 0;
    if (fp == NULL){
        return 0;
    }
    else{
        char line[MaxLineLen];
        char fullLine[MaxLineLen];
        char *delim = " .,?!;:()\"\n";
        int lineNum = 0;

        while (fgets(line, MaxLineLen, fp) != NULL){
            ans = 0;
            int wordNum = 0;
            char *ptr;
            strcpy(fullLine,line);
            ptr = strtok(line, delim);

            while (ptr){
                if (strcmp(ptr,pattern) == 0){
                    ans = 1;
                    return ans;
                }
                ptr = strtok(NULL, delim);
                wordNum++;
            }
            lineNum++;
        }
        fclose(fp);
    }
    return ans;
}



void w_grep(char *filename, char *pattern){
    Node *head = NULL;
    Node *tail = NULL;
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("GREP: Unable to open file: %s\n", filename);
    }
    else{
        char line[MaxLineLen];
        char fullLine[MaxLineLen];
        char *delim = " .,?!;:()\"\n";
        int lineNum = 0;

        while (fgets(line, MaxLineLen, fp) != NULL){

            int wordNum = 0;
            char *ptr;
            strcpy(fullLine,line);
            ptr = strtok(line, delim);

            while (ptr){
                if (strcmp(ptr,pattern) == 0){
                    tail = appendAtTail_ll(tail, fullLine, lineNum, wordNum);
                    if (!head)
                        head = tail;
                    }
                    ptr = strtok(NULL, delim);
                    wordNum++;
            }
                lineNum++;
        }
        char pLine[MaxLineLen];
        int pLineNum = 0;
        PatNode *pWordNums;

        while(head){
            head = removeFromHead_ll(head, pLine, &pLineNum, &pWordNums);
            printf("%s: Line %d: %s",filename, pLineNum, pLine);
        }
        delete_ll(head);
        fclose(fp);
    }
}

void o_grep(char *filename, char *pattern){
    Node *head = NULL;
    Node *tail = NULL;
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("GREP: Unable to open file: %s\n", filename);
    }
    else{
        char line[MaxLineLen];
        char fullLine[MaxLineLen];
        char *delim = " .,?!;:()\"\n";
        int lineNum = 0;

        while (fgets(line, MaxLineLen, fp) != NULL){

            int wordNum = 0;
            char *ptr;
            strcpy(fullLine,line);
            ptr = strtok(line, delim);

            while (ptr){
                if (strstr(ptr, pattern) != NULL){
                    tail = appendAtTail_ll(tail, fullLine, lineNum, wordNum);
                    if (!head)
                        head = tail;
                    }
                    ptr = strtok(NULL, delim);
                    wordNum++;
            }
                lineNum++;
        }
        char pLine[MaxLineLen];
        int pLineNum = 0;
        PatNode *pWordNums;

        while(head){
            head = removeFromHead_ll(head, pLine, &pLineNum, &pWordNums);
            printf("%s: %s\n",filename, pattern);
        }
        fclose(fp);
    }
}

void n_grep(char *filename, char *pattern){
    Node *head = NULL;
    Node *tail = NULL;
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("GREP: Unable to open file: %s\n", filename);
    }
    else{
        char line[MaxLineLen];
        char fullLine[MaxLineLen];
        char *delim = " .,?!;:()\"\n";
        int lineNum = 0;

        while (fgets(line, MaxLineLen, fp) != NULL){

            int wordNum = 0;
            char *ptr;
            strcpy(fullLine,line);
            ptr = strtok(line, delim);

            while (ptr){
                if (strstr(ptr, pattern) != NULL){
                    tail = appendAtTail_ll(tail, fullLine, lineNum, wordNum);
                    if (!head)
                        head = tail;
                    }
                    ptr = strtok(NULL, delim);
                    wordNum++;
            }
                lineNum++;
        }
        char pLine[MaxLineLen];
        int pLineNum = 0;
        PatNode *pWordNums;

        while(head){
            head = removeFromHead_ll(head, pLine, &pLineNum, &pWordNums);
            printf("%s: Line %d:  %s",filename, pLineNum, pLine);
        }
        fclose(fp);
    }
}

void x_grep(char *filename, char *pattern){
    Node *head = NULL;
    Node *tail = NULL;
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("GREP: Unable to open file: %s\n", filename);
    }
    else{
        char line[MaxLineLen];
        char fullLine[MaxLineLen];
        char *delim = " .,?!;:()\"\n";
        int lineNum = 0;

        while (fgets(line, MaxLineLen, fp) != NULL){

            int wordNum = 0;
            char *ptr;
            strcpy(fullLine,line);
            ptr = strtok(line, delim);

            while (ptr){
                if (strcmp(ptr, pattern) == 0){
                    tail = appendAtTail_ll(tail, fullLine, lineNum, wordNum);
                    if (!head)
                        head = tail;
                    }
                    ptr = strtok(NULL, delim);
                    wordNum++;
            }
                lineNum++;
        }
        char pLine[MaxLineLen];
        int pLineNum = 0;
        PatNode *pWordNums;

        while(head){
            head = removeFromHead_ll(head, pLine, &pLineNum, &pWordNums);
            printf("%s: Line %d, Index %d: %s",filename, pLineNum, pWordNums->patNum, pLine);
        }
        fclose(fp);
    }
}

