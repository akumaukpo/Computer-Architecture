#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>

#include "inputoutput.h"

//reads and writes / error detection

inline void error(const char* err_msg) {
    perror(err_msg);
    exit(errno);
}

FILE* s_fopen(const char* filepath, const char* filemode) {
    FILE* fh = fopen(filepath,filemode);
    if (!fh)
        error("fopen");
    return fh;
}

inline size_t s_fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream) 
{
    size_t dex = fwrite(ptr,size,nmemb,stream);
    if (dex == 0) {
        if ( ferror(stream) ) 
            error("fwrite"); 
    }
    return dex;
}

size_t s_fread(void* ptr, size_t size, size_t nmemb, FILE* stream) 
{
    size_t n_elt = fread(ptr,size,nmemb,stream);
    if (n_elt == 0) { 
        if ( ferror(stream) )  
            fatal("fread");
    }
    return n_elt;
}

inline void 
usage(const char* progname)
{
    fprintf(stderr,"USAGE:\t '%s' filename\n", progname);
    exit(1);
}











//calculate register 

inline uint8_t CalcReg(char *token){
    uint8_t register = (uint8_t) atoi(token);
    register &= 0b11;
    return register;
}

//immediate field
inline uint8_t CalcImm(char *token){
    uint8_t val;
    if (*token == '-') {
        val = (uint8_t) atoi(token+1);
        val -= 1;     
        val ^= 0xF;   
        val |= 0b1000;
        val &= 0x0F;
        return val;
}
    else {
        val = (uint8_t) atoi(token);
        val &= 0b0111;
        return val;
    }
}

inline uint8_t CalcBit(char *token){
    uint8_t register = (uint8_t) atoi(token);
    register &= 0x01;
    return register;
}

//read from assembly -> translate to binary -> process
//last line in assembly left blank for test cases and readability 

void read_assembly_file(const char* read_filename, const char* write_filename){
    FILE* read_fh = s_fopen(read_filename, "r");
    FILE* write_fh = s_fopen(write_filename, "w");

    char *token;
    char *operation = NULL;
    uint8_t instruction;
    int i = 0;
    char character = 0;
    char *line = (char*) calloc(MAX_INSTRUCTION_LENGHT, sizeof(char));

while ( s_fread(&character, sizeof(char), 1UL, read_fh) ) {
        if (character == '\n'){ //parse till end 

        token = strtok(line, " , $#");
        operation = token;

        //BINARY TRANSLATION

    if (strcmp(operation, "LOAD") == 0)            
         instruction = 0b00000000;
         else if (strcmp(operation, "ADD") == 0)   
         instruction = 0b01000000;
         else if (strcmp(operation, "SUB") == 0)   
         instruction = 0b10000000;
         else if (strcmp(operation, "SKIP") == 0)  
         instruction = 0b11000000;
         else if (strcmp(operation, "PRINT") == 0) 
         instruction = 0b11000001; 

         token = strtok(NULL, ",$#");

//READ NEW INSTRUCTION LINE, DECODE OPERATIONS 
         for (i = 0; token != NULL; i++){
             //IF LOAD INSTRUCTION, PARSE FIRST REG AND IMM FIELD
             if (strcmp(operation, "LOAD")== 0){
             if (i == 0) instruction |= CalcReg(token) << 4;
             else if (i == 1) instruction |= CalcImm(token) << 0;
         }
//IF ADD OR SUB , READ SOURCE REGISTERS 
         else if (strcmp(operation, "ADD")== 0) || (strcmp(operation, "SUB")== 0){
             if (i == 0) instruction |= CalcReg(token) << 0;
                    else if (i == 1) instruction |= CalcReg(token) << 4;
                    else if (i == 2) instruction |= CalcReg(token) << 2;
         }
//IF SKIP, READ REGISTERS TO BE COMPARED 
        else if (strcmp(operation, "SKIP") == 0) {
                    if (i == 0) instruction |= CalcReg(token) << 4;
                    else if (i == 1) instruction |= CalcReg(token) << 2;
                    else if (i == 2) instruction |= CalcReg(token) << 1;
        }
//PRINT
        else if (strcmp(operation, "PRINT") == 0) {
                    if (i == 0) instruction |= CalcReg(token) << 4;
    
         }

         token = strtok(NULL, " ,$#");


        }

        //Write to Binary 

        s_fwrite(&instruction, sizeof(uint8_t), 1UL, write_fh);
        //clear line
        line[0] = '\0';
        i = 0;
        continue;

        }
        line[i] = character;
        line[++i] = '\0';

}

fclose(read_fh);
fclose(write_fh);

}


