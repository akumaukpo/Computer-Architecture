//Akuma Akuma-Ukpo

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <errno.h>
#include <string>
using namespace std;
int regfile [4] = {0,0,0,0};
//Returns 0 for addition, 1 for sub, 2 for load, 3 for skip1, 4 for skip2, 5 for print
int decode(string inst){
    if (inst[0]=='0' && inst[1]=='0') return 0;                                     //00xxxxxx=add
    if (inst[0]=='0' && inst[1]=='1') return 1;                                     //01xxxxxx=sub
    if (inst[0]=='1' && inst[1]=='0') return 2;                                     //10xxxxxx=ld
    if (inst[0]=='1' && inst[1]=='1' && inst[2]=='1' && inst[3]=='0') return 3;     //1110xxxx=skip1
    if (inst[0]=='1' && inst[1]=='1' && inst[2]=='1' && inst[3]=='1')return 4;      //1111xxxx=skip2
    if (inst[0]=='1' && inst[1]=='1'&& inst[2]=='0') return 5;                      //110xxxxx=print
}
//add
void add(int rd, int rs1, int rs2){
    regfile[rd] = regfile[rs1] + regfile[rs2];
}
//sub
void sub(int rd, int rs1, int rs2){
    regfile[rd] = regfile[rs1] - regfile[rs2];
}
//ld
void ld(int r, int val){
    regfile[r] = val;
}
//skip1
int skip1(int r1, int r2){
    if (regfile[r1]==regfile[r2]){
        return 1;
    }
    return 0;
}
//skip2
int skip2(int r1, int r2){
    if (regfile[r1]==regfile[r2]){
        return 2;
    }
    return 0;
}
//print
int print(int r){
    cout <<"R"<<r<<"= "<< regfile[r] << endl;
}
//decode register
int decode_reg(string reg){
    if (reg == "00") return 0;
    if (reg == "01") return 1;
    if (reg == "10") return 2;
    if (reg == "11") return 3;
}
//converts 2's complements to integers
int binary2int(string num_str){
    int return_num = 0;
    for(int i = 1 ; i < 4 ; ++i){
        int bit =((int) num_str[i] - 48);
        return_num = (return_num << 1);
        if (num_str[0] == '0')
             return_num += bit;
        else 
            if (bit == 0)
                return_num += 1 ;
    }
    if (num_str[0] == '0')
        return return_num;
    else
        return (return_num + 1)* (-1);
}
int main(){
    ifstream instructions_file ("test_case.txt");
    //check if instructions_file has been read successfully
    if(instructions_file){
      string tmp;
      if(instructions_file >> tmp){
        std::cout << "successful read \n";
        
      }
      else{
        std::cout << "fail";
      }
    }

    //error message if file cannot be opened , function should be defined here
    
    string instruction;
    int skip = 0;
    int rs1, rs2, r, rd, r1, r2, val = 0;
    string rs1_str, rs2_str, r_str, rd_str, r1_str, r2_str, val_str = "";
    while (instructions_file >> instruction)
    {
        while (skip-- != 0 && instructions_file >> instruction);
        skip = 0;
        switch (decode(instruction)){
                case 0:         //add
                        rs1_str = instruction.substr(6, 2);
                        rs2_str = instruction.substr(4, 2);
                        rd_str = instruction.substr(2, 2);
                        rs1 = decode(rs1_str);
                        rs2 = decode(rs2_str);
                        rd = decode(rd_str);

                        add(rd, rs1, rs2);
                    break;
                case 1:         //sub
                        rs1_str = instruction.substr(6, 2);
                        rs2_str = instruction.substr(4, 2);
                        rd_str = instruction.substr(2, 2);
                        rs1 = decode(rs1_str);
                        rs2 = decode(rs2_str);
                        rd = decode(rd_str);

                        sub(rd, rs1, rs2);
                        
                    break;
                case 2:         //ld
                        r_str = instruction.substr(2, 2);
                        val_str = instruction.substr(4, 4);
                        r = decode(r_str);
                        val = binary2int(val_str);

                        ld(r,val);
                    break;
                case 3:         //skip1
                        r1_str = instruction.substr(6, 2);
                        r2_str = instruction.substr(4, 2);
                        r1 = decode(r1_str);
                        r2 = decode(r2_str);
                        
                        skip = skip1(r1,r2);
                    break;
                case 4:         //skip2
                        r1_str = instruction.substr(6, 2);
                        r2_str = instruction.substr(4, 2);
                        r1 = decode(r1_str);
                        r2 = decode(r2_str);
                        
                        skip = skip2(r1,r2);
                    break;
                case 5:         //print
                         r_str = instruction.substr(3, 2);
                        r = decode(r_str);
                        
                        print(r);
                        break;
                default: cout << "ERROR" << endl;
        }
    }
    return 0;
}