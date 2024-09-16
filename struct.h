// Copyright Caruntu Dana-Maria 311CAa 2023-2024

#ifndef STRUCTURE
#define STRUCTURE

typedef struct{
	int R;
	int G;
	int B;
} rgb; //imaginea propriu-zisa

typedef struct {
	int x1, y1, x2, y2;
	int row, col;
	int maxx;
	int ascii;
	int load_error;
	int nr_img;
	int charlie;
	char magic_word[5];
	int **matrix; //imagini grayscale
	rgb **photo; //imagini rgb
} image; //informatiile imaginii

#endif
