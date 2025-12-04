/********************************************************************************
 * aoc.c
 * 
 ********************************************************************************/
#include "aoc.h"
#include <string.h>

input_t * openInput()
{
    input_t * input = malloc(sizeof(struct input_s));
    if (!input) goto err_alloc;
    // Assume input is in ./input.txt
    FILE * in = fopen("./input.txt","r");
    if (!in) {free(input); goto err_file;}
    fseek(in,0,SEEK_END);
    input->len = ftell(in); // Overshoots the size in cases of \r\n
    rewind(in);
    input->txt = calloc(input->len+1, sizeof(char));
    if (!input->txt) {free(input); fclose(in); goto err_alloc;}
    // Now copy bytes one by one
    input->len = fread(input->txt,sizeof(char),input->len,in);
    fclose(in);
    if (strlen(input->txt) > input->len) input->txt[input->len] = '\0';
    // Now len == real len
    // Shrink the memory back down
    input->txt = realloc(input->txt, sizeof(char) * (input->len+1));

    return input;

err_alloc:
    fprintf(stderr, "Couldn't allocate memory!\n");
    return NULL;

err_file:
    fprintf(stderr, "Error reading input.txt\n");
    return NULL;

err_size:
    fprintf(stderr, "File size issue (%lld != %lld)!\n",strlen(input->txt),input->len);
    return NULL;
}

void forEachLine(const input_t * input, void (*func)(char*, size_t))
{
    char * scanner = input->txt;
    while (scanner != (char*)1)
    {
        char * next_newline = strchr(scanner,'\n');
        if (!next_newline) next_newline = input->txt + input->len; // OOBA
        char str[next_newline-scanner+1];
        sscanf(scanner,"%[^\n]",str);
        func(str,next_newline-scanner);
        scanner = strchr(scanner, '\n') + 1;
    }
}

void enumerateForEachLine(const input_t * input, void (*func)(size_t, char*, size_t))
{
    char * scanner = input->txt;
    size_t i = 0;
    while (scanner != (char*)1)
    {
        char * next_newline = strchr(scanner,'\n');
        if (!next_newline) next_newline = input->txt + input->len; // OOBA
        char str[next_newline-scanner+1];
        sscanf(scanner,"%[^\n]",str);
        str[next_newline-scanner] = '\0';
        func(i++, str, next_newline-scanner);
        scanner = strchr(scanner, '\n') + 1;
    }
    // char * scanner = input->txt;
    // for (size_t i=0; scanner < input->txt + input->len; i++)
    // {
    //     char * next_n = strchr(scanner, '\n');
    //     if (next_n) *next_n = '\0';
    //     else next_n = input->txt + input->len;
    //     char * str = strdup(scanner);
    //     func(i, str, strlen(str));
    //     free(str);
    //     scanner = next_n + 1;
    // }
}

void forEachChar(const input_t * input, void (*func)(char))
{
    // For each char of input.txt:
    for (size_t i=0; i<input->len; i++)
    {
        func(input->txt[i]);
    }
}

void enumerateForEachChar(const input_t * input, void (*func)(size_t, char))
{
    for (size_t i=0; i<input->len; i++)
    {
        func(i, input->txt[i]);
    }
}

void print1dFloatArrayAsTable(const float * arr, size_t rows, size_t cols)
{
    if(!arr) return;

    for(size_t r=0; r<rows; r++)
    {
        for(size_t c=0; c<cols; c++)
        {
            printf("%5.2ff%s",*(arr+c+(cols*r)),(c==cols-1?"\n":", "));
        }
    }
}

void print1dIntArrayAsTable(const int *arr, size_t rows, size_t cols)
{
    if(!arr) return;

    for(size_t r=0; r<rows; r++)
    {
        for(size_t c=0; c<cols; c++)
        {
            printf("%df%s",*(arr+c+(cols*r)),(c==cols-1?"\n":", "));
        }
    }
}