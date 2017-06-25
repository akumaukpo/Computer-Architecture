#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <bitset>

#include "calc.h"
#include "inputoutput.h"

uint8_t skip = 0; // Initialize skip bit 

calc::calc(void) {
    this->reg0 = 0;
    this->reg1 = 0;
    this->reg2 = 0;
    this->reg3 = 0;

 //  this->o0 = this->o1 = this->o2 = this->o3 = 0;  

}
calc::~calc(void) {}

void calc::load(uint8_t d, uint8_t imm) {
    
    switch(d) { // Load immediate into register, reset overflow flag
        case 0: reg0 = imm; 
        case 1: reg1 = imm; 
        case 2: reg2 = imm; 
        case 3: reg3 = imm; 
    }
}

void calc::add(uint8_t d, uint8_t s, uint8_t t, bool add){
    uint8_t result;
    uint8_t carry;

    //load the source reg 
    // can change "s" to "source" instead

    switch(s) {
        case 0: s = reg0;
              break;
        case 1: s = reg1;
            break;
        case 2: s = reg2;
            break;
        case 3: s = reg3;
            break;
    }

    //load target register for ADD ins
    //can change "t" to "target" instead

    if (add){
        switch(t) {
            case 0: t = reg0;
                    break;
            case 1: t = reg1;
                    break;
            case 2: t = reg2;
                    break;
            case 3: t = reg3;

    }
    //print "+"
    cout << bitset<8>(s) << "+" << bitset<8>(t) << endl;

}
//else case will be subtraction 

else {
    //load target register again

    switch(t) {
            case 0:
                t = reg0;
                break;
            case 1:
                t = reg1;
                break;
            case 2:
                t = reg2;
                break;
            case 3:
                t = reg3;
}

//print "-"
cout << bitset<8>(s) << "+" << bitset<8>(t) << endl;
//signed
t = ((t-1) ^ 0xFF);
}

//Calculate , carry, add

//code 



//half adder logic from https://www.cs.umd.edu/class/sum2003/cmsc311/Notes/Comb/adder.html
//bit slicing technique adapted from http://www.academia.edu/9257354/SIMPLE_8-BIT_CALCULATOR_DESIGN_-_BIT_SLICING_TECHNIQUE_AND_FPGA_PROTOTYPING 
//ISA design implementation from MIPS32 Architecture for programmers



switch(d){
    case 0: reg0 = result; break;
    case 1: reg1 = result; break;
    case 2: reg2 = result; break;
    case 3: reg3 = result;
}
return;
}

void calc::sub(uint8_t s, uint8_t t){
    add(d, s, t, false);
}

// Skip one instrution
void calc::skip_1(uint8_t s, uint8_t t) {
    switch(s) {
        case 0: s = reg0; break;
        case 1: s = reg1; break;
        case 2: s = reg2; break;
        case 3: s = reg3;
    }
    switch(t) {
        case 0: t = reg0; break;
        case 1: t = reg1; break;
        case 2: t = reg2; break;
        case 3: t = reg3;
    }
    if (s == t) skip = 1; //if source equals target, skip
}

//skip 2 ins, same as above but skip = 2

void calc::skip_1(uint8_t s, uint8_t t) {
    switch(s) {
        case 0: s = reg0; break;
        case 1: s = reg1; break;
        case 2: s = reg2; break;
        case 3: s = reg3;
    }
    switch(t) {
        case 0: t = reg0; break;
        case 1: t = reg1; break;
        case 2: t = reg2; break;
        case 3: t = reg3;
    }
    if (s == t) skip = 2; //if source equals target, skip
}

//Print register value

void calc::print(uint8_t s) {
    switch(s) {
        case 0:
            cout << bitset<8>(reg0) << endl;
            break;
        case 1:
             cout << bitset<8>(reg1) << endl;
            break;
        case 2:
             cout << bitset<8>(reg2) << endl;
            break;
        case 3:
             cout << bitset<8>(reg3) << endl;
    }
}

//READ BINARY FILE

//using code structure from open source https://www.codingunit.com/c-tutorial-binary-file-io

void calc::read_binary_file(const char* filename) {
    FILE*   fh   = s_fopen(filename,"r");
    uint8_t word = 0;

    //if instructions are present 
    while ( s_fread(&word, sizeof(uint8_t), 1UL, fh) ) {
 
 //reset skip
        if (skip != 0) { 
            skip--;
            continue;
        }

//LOAD Rs, Imm

    if (word>>6 == 0b00){
        load((word >> 4) & 0b11, word & 0xF);
    }
//ADD inst, Rd, Rs, Rt
    else if (word >> 6 == 0b01) { 
        add(word & 0b11, (word >> 4) & 0b11, (word >> 2) & 0b11, true);
//SUB ins, Rd, Rs, Rt
    } else if (word >> 6 == 0b10) {   
        sub(word & 0b11, (word >> 4) & 0b11, (word >> 2) & 0b11);  
    }
//Skip
else if (word >> 6 == 0b11) {  
    if ((word & 0x01) == 0) {           
            if (((word >> 1) & 0x01) == 0) {        
                skip_1((word >> 4) & 0b11, (word >> 2) & 0b11);
                } else if (((word >> 1) & 0x01) == 1) { 
                    skip_2((word >> 4) & 0b11, (word >> 2) & 0b11);
                }
//Print Rs
    }
else if ((word & 0x01) == 1) {
        print((word >>4) & 0b11);
}

 }
    }
fclose(fh);

    }



