// Copyright Caruntu Dana-Maria 311CAa 2023-2024
#ifndef COMMANDS
#define COMMANDS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "alloc.h"

void SAVE(image * img, char img_name[]);
void file_read_matrix(int ***v, int n, int m, FILE *input);
void read_matrix(image *img, FILE *input);
void file_read_matrix_binary(int ***v, int n, int m, FILE *input);
void read_matrix_binary(image *img, FILE *input);
void read_rgb(image *img, FILE *input);
void read_rgb_binary(image *img, FILE *input);
void LOAD(image *img, char img_name[]);
void swap(int *a, int *b);
void SELECT(image *img, int x1, int x2, int y1, int y2);
void select_all(image *img);
void clamp(int *n, int min, int max);
void EDGE(image *img, int *apply);
void SHARPEN(image *img, int *apply);
void BLUR(image *img, int *apply);
void GAUSSIAN_BLUR(image *img, int *apply);
int verify(int n);
void verify_HISTOGRAM(image *img);
void HISTOGRAM(image *img, int x, int y);
void ROTATE_LEFT_SELECT(image *img);
void ROTATE_LEFT_SELECT_ALL(image *img);
void ROTATE(image *img, int angle);
void EXIT(image *img);
int sum(int x, int freq[]);
void EQUALIZE(image *img);
void write_values(image *img, FILE *output, int i, int j);
void CROP(image *img);
void get_enter(void);
void APPLY_FILTER(image *img, char line[]);
void get_line(char line[], int *nr);
int get_coordinates(char command[], int *x1, int *y1, int *x2,
					int *y2, int *nr);
void print_histogram(int *histogram, int y, int j);
#endif
