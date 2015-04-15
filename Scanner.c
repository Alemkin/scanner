/**
* Alexander Lemkin
* Systems Software Fall 2014
* Homework 2 Scanner
* Scanner Implementation Assignment
*/
#include <stdio.h>
#include <stdlib.h>
#include<string.h>

#define IDENSIZE 11
#define NUMSIZE 5
FILE *inFile;
FILE *cleanOut;
FILE *lexTable;
FILE *lexList;
char ch; /*Main char pointer for the file*/
char theTemp; /*Used for one forward checking of chars*/
int breakCheck, check;
char st[IDENSIZE+1]; /*char array used to check identifiers*/
/*Array holding the reserved symbols*/
char *res_stuff[] = {"null", "asdwad", "cxvvase", "+", "-", "*", "/", "odd",
    "=", "<>", "<", "<=", ">", ">=", "(", ")", ",", ";", ".", ":=", "begin",
    "end", "if", "then", "while", "do", "call", "const", "var", "procedure",
    "write", "read", "else"};

int main() {
    int i;
    /*Input and output files*/
    inFile = fopen("input.txt", "r");
    cleanOut = fopen("cleaninput.txt", "w");
    lexTable = fopen("lexemetable.txt", "w");
    lexList = fopen("lexemelist.txt", "w");
    printCleanOut();
    fclose(inFile);
    fclose(cleanOut);
    inFile = fopen("cleaninput.txt", "r");

    /*Initializing the lexemetable.txt file*/
    fprintf(lexTable,"lexeme  token type\n");

    ch = fgetc(inFile);
    /*Main while loop taking care of the checking*/
    while(!feof(inFile)) {
        breakCheck = 0; /*Used to check if a continue is needed*/
        memset(&st[0], 0, sizeof(st)); /*Resets st*/
        i = 0;
        while(isspace(ch)) {
            ch = fgetc(inFile);
        }
        if (isdigit(ch)) { /*If next space is a digit*/
            while (isdigit(ch)) {
                if (i<NUMSIZE) {
                    st[i] = ch;
                    i++;
                }
                else {
                    printf("Number too long.\n");
                    exit(0);
                }
                ch = fgetc(inFile);
                /*If char in this sequence is not another digit*/
                if (!isspace(ch) && !isdigit(ch)) {
                /*Fail is it is a character*/
                if(isalpha(ch)) {
                    printf("Variable does not start with a letter.\n");
                    exit(0);
                }
                /*However print it and set ch to the tempChar if semicolon, or invalidate the file*/
                if (ch == ';') {
                    theTemp = fgetc(inFile);
                    if(!isspace(theTemp)) {
                        printf("Invalid Symbols.\n");
                        exit(0);
                    }
                    fprintf(lexTable, "%s\t3\n", st);
                    fprintf(lexList, "3 %s ", st);
                    fprintf(lexTable, ";\t18\n");
                    fprintf(lexList, "18 ", st);
                    ch = theTemp;
                    breakCheck = 1;
                    break;
                }
                    printf("Invalid Symbols.\n");
                    exit(0);
                }
            }
            if(breakCheck == 1) {
                continue;
            }
            fprintf(lexTable, "%s\t3\n", st);
            fprintf(lexList, "3 %s ", st);
        }
        else if (isalpha(ch)) { /*Now if it starts as character*/
            i = 0;
            memset(&st[0], 0, sizeof(st));
            while (isalpha(ch) || isdigit(ch)) {
                if (i < IDENSIZE) {
                    st[i] = ch;
                    i++;
                }
                else {
                    printf("Name too long.\n");
                    exit(0);
                }
                ch = fgetc(inFile);
                /*If a next space is not expected*/
                if(!isspace(ch) && !isalpha(ch) && !isdigit(ch)) {
                        /*If it is any of these, make sure the next char is a space*/
                    if (ch == ';' || ch == ',' || ch == '.') {
                        theTemp = fgetc(inFile);
                        if (!isspace(theTemp)) {
                            printf("Invalid Symbols.\n");
                            exit(0);
                        }
                        /*If cool, print the string and the next char, then set ch to the tempChar*/
                        check = checkCharNum();
                        if (check != 0) {
                            fprintf(lexTable, "%s\t%d\n", st, check);
                            fprintf(lexList, "%d ", check);
                        }
                        else {
                            fprintf(lexTable, "%s\t2\n", st);
                            fprintf(lexList, "2 %s ", st);
                        }

                        if(ch ==';'){
                            fprintf(lexTable, ";\t18\n");
                            fprintf(lexList, "18 ");
                        }
                        else if (ch =='.') {
                            fprintf(lexTable, ".\t19\n");
                            fprintf(lexList, "19 ");
                        }
                        else {
                            fprintf(lexTable, ",\t17\n");
                            fprintf(lexList, "17 ");
                        }

                        ch = theTemp;
                        breakCheck = 1;
                        break;
                    }
                }
            }
            if (breakCheck == 1) {
                continue;
            }
            check = checkCharNum();
            if (check != 0) {
                fprintf(lexTable, "%s\t%d\n", st, check);
                fprintf(lexList, "%d ", check);
            }
            else {
                fprintf(lexTable, "%s\t2\n", st);
                fprintf(lexList, "2 %s ", st);
            }

        }
        else { /*If not a char or digit or space, then check each string against the array of idents*/
            i = 0;
            memset(&st[0], 0, sizeof(st));
            st[i] = ch;
            i++;
            ch = fgetc(inFile);
            while(1) {
                if (isspace(ch) || feof(inFile)) {
                    break;
                }
                st[i] = ch;
                i++;
                ch = fgetc(inFile);
            }
            check = checkCharNum();
            if(check != 0) {
                fprintf(lexTable, "%s\t%d\n", st, check);
                fprintf(lexList, "%d ", check);
            }
            else {
                if (feof(inFile)){
                    break;
                }
                printf("Invalid Symbols.\n");
                exit(0);
            }
        }
        ch = fgetc(inFile);
    }

    fclose(inFile);
    fclose(lexTable);
    fclose(lexList);

}

/**Checks the string against the ones in the array to find the
identifying number*/
int checkCharNum() {
    int ret = 0;
    int i = 0;
    for (i = 0; i < 33; i++) {
        if(strcmp(st, res_stuff[i]) == 0) {
            ret = i+1;
            return ret;
        }
    }
    return ret;
}

/**
*Prints the cleaninput.txt file, clearing comments and /t and /n
*/
printCleanOut() {
    ch = fgetc(inFile);
    char prevChar;
    char tempChar;

    while(!feof(inFile)) {
        /*Skips comments*/
        while (ch == '/') {
            if (ch == '/') {
                tempChar = fgetc(inFile);
                if(tempChar == '*') {
                    prevChar = ch;
                    ch = fgetc(inFile);
                    while (ch != '/' && prevChar != '*') {
                        if(feof(inFile)) {
                            break;
                        }
                        prevChar = ch;
                        ch = fgetc(inFile);

                    }
                    ch = fgetc(inFile);
                }
                else{
                    fprintf(cleanOut, "%c%c", ch, tempChar);
                    ch = fgetc(inFile);
                }
            }
        }
            /*Skips newlines and tabs*/
            while(ch == '\\') {
                ch = fgetc(inFile);
                ch = fgetc(inFile);
            }

            fprintf(cleanOut, "%c", ch);
            ch = fgetc(inFile);

    }
}
