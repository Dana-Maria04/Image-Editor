// Copyright Caruntu Dana-Maria 311CAa 2023-2024
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "alloc.h"

int **alloc_matrix(int n, int m)
{
	// functie pentru alocarea dinamica a matricii
	int i;
	// alocarea matricii
	int **a = (int **)malloc(n * sizeof(int *));
	if (!a) {
		// realizarea defensiva a alocarii
		printf("first malloc failed!\n");
		return NULL;
	}
	// alocare pe linii
	for (i = 0; i < n; i++) {
		a[i] = (int *)malloc(m * sizeof(int));
		if (!a[i]) {
			printf("allocating every array failed\n");
			while (--i >= 0)
				free(a[i]);
			free(a);
			return NULL;
		}
	}
	return a;
}

void free_matrix(int **a, int n)
{
	// functie de eliberare a matricii
	for (int i = 0; i < n; i++)
		free(a[i]);
	free(a);
}

rgb **alloc_rgb(int n, int m)
{
	// functie pentru alocarea dinamica a matricii
	int i;
	// alocarea matricii
	rgb **a = (rgb **)malloc(n * sizeof(rgb *));
	if (!a) {
		// realizarea defensiva a alocarii
		printf("first malloc failed!\n");
		return NULL;
	}
	// alocare pe linii
	for (i = 0; i < n; i++) {
		a[i] = (rgb *)malloc(m * sizeof(rgb));
		if (!a[i]) {
			printf("allocating every array failed\n");
			while (--i >= 0)
				free(a[i]);
			free(a);
			return NULL;
		}
	}
	return a;
}

void free_rgb(rgb **a, int n)
{
	// functie de eliberare a matricii
	for (int i = 0; i < n; i++)
		free(a[i]);
	free(a);
}
