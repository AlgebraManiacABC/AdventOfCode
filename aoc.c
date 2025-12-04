/********************************************************************************
 * aoc.c
 * 
 ********************************************************************************/
#include "aoc.h"
#include <string.h>

void setLineCount(input_t * input)
{
    input->row_count = 0;
    for (size_t i = 0; i < input->len; i++)
    {
        if (input->txt[i] == '\n') input->row_count++;
    }
    input->row_count++;
}

void setRows(input_t * input)
{
    if (input->col_count == 0)
    {
        fprintf(stderr, "Could not set rows, since col_count == 0\n");
        return;
    }

    input->rows = malloc(input->row_count * sizeof(char *));
    if (!input->rows) goto err_mem_rows;
    char * current = input->txt;
    for (size_t i = 0; i < input->row_count; i++)
    {
        input->rows[i] = calloc(input->col_count + 1, sizeof(char));
        if (!input->rows[i]) goto err_mem_rowsi;
        memcpy(input->rows[i], current, input->col_count);
        current += input->col_count + 1;
    }
    return;
err_mem_rows:
    fprintf(stderr, "Not enough memory to allocate input->rows!\n");
    input->col_count = 0;
    return;
err_mem_rowsi:
    fprintf(stderr, "Not enough memory to allocate input->rows[i]!\n");
    input->col_count = 0;
}

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

    setLineCount(input);
    if (input->row_count >= 2)
    {
        char * temp = strchr(input->txt,'\n');
        char * temp2 = strchr(temp+1,'\n');
        if (temp2 - temp == temp - input->txt + 1)
        {
            input->col_count = temp2 - temp - 1;
        }
        else
        {
            input->col_count = 0;
        }
    }
    else
    {
        input->col_count = 0;
    }

    setRows(input);
    input->cols = NULL;

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

size_t getLineCount(input_t * input)
{
    return input->row_count;
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

void enumerateForEachCharInTable(const input_t * input, void (*func)(size_t, size_t, char))
{
    for (size_t i=0; i<input->row_count; i++)
    {
        for (size_t j=0; j<input->col_count; j++)
        {
            func(i,j,input->rows[i][j]);
        }
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

int countAdjacentChar(const input_t * input, size_t row, size_t col, char c)
{
    if (input->col_count == 0)
    {
        fprintf(stderr,"Couldn't get adjacent chars due to column width being 0.\n");
        return -1;
    }
    if (col >= input->col_count || row >= input->row_count)
    {
        fprintf(stderr,"Couldn't get adjacent chars: index OOB.\n");
        return -1;
    }
    int count = 0;
    // Count left
    if (col > 0)
    {
        // Top-left
        if (row > 0 && input->rows[row-1][col-1] == c) count++;
        // Left
        if (input->rows[row][col-1] == c) count++;
        // Bottom-left
        if (row < input->row_count - 1 && input->rows[row+1][col-1] == c) count++;
    }
    // Top
    if (row > 0 && input->rows[row-1][col] == c) count++;
    // Bottom
    if (row < input->row_count - 1 && input->rows[row+1][col] == c) count++;
    if (col < input->col_count - 1)
    {
        // Top-right
        if (row > 0 && input->rows[row-1][col+1] == c) count++;
        // Right
        if (input->rows[row][col+1] == c) count++;
        // Bottom-right
        if (row < input->row_count - 1 && input->rows[row+1][col+1] == c) count++;
    }
    return count;
}