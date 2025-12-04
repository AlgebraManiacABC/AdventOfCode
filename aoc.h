/********************************************************************************
 * aoc.h
 *
 * Utility functions for Advent of Code
 ********************************************************************************/

#ifndef ADVENTOFCODE_AOC_H
#define ADVENTOFCODE_AOC_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

typedef struct input_s
{
    size_t len;
    char * txt;

}   input_t;

input_t * openInput();

void forEachLine(const input_t * input, void (*func)(char*, size_t));
void enumerateForEachLine(const input_t * input, void (*func)(size_t, char*, size_t));

void forEachChar(const input_t * input, void (*func)(char));
void enumerateForEachChar(const input_t * input, void (*func)(size_t, char));

void print1dFloatArrayAsTable(const float *arr, size_t rows, size_t cols);
void print1dIntArrayAsTable(const int *arr, size_t rows, size_t cols);

#endif