// Copyright Caruntu Dana-Maria 311CAa 2023-2024
#ifndef ALLOC
#define ALLOC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

int **alloc_matrix(int n, int m);
void free_matrix(int **a, int n);
rgb **alloc_rgb(int n, int m);
void free_rgb(rgb **a, int n);

#endif
