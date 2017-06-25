#ifndef INPUT_OUTPUT_H
#define INPUT_OUTPUT_H

const int MAX_INSTRUCTION_LENGHT = 150;


FILE* s_fopen(const char* path, const char* mode);

inline size_t s_fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream);

size_t s_fread(void* ptr, size_t size, size_t nmemb, FILE* stream);



inline uint8_t CalcReg(char *token);

inline uint8_t CalcImm(char *token);

inline uint8_t CalcBit(char *token);

//benchmark assembly code , should end in blank line on purpose

void read_assembly_file(const char* read_filename, const char* write_filename);


#endif 