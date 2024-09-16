// Copyright Caruntu Dana-Maria 311CAa 2023-2024
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "alloc.h"

void write_values(image *img, FILE *output, int i, int j)
{
	//scriu valorile in fisier
	if (fwrite(&img->photo[i][j].R, sizeof(char), 1, output) != 1 ||
		fwrite(&img->photo[i][j].G, sizeof(char), 1, output) != 1 ||
		fwrite(&img->photo[i][j].B, sizeof(char), 1, output) != 1)
		printf("Error writing this file!\n");
}

void SAVE(image *img, char img_name[])
{
	if (img->nr_img == 0) {
		printf("No image loaded\n"); return;
	}
	if (img->ascii == 1) {
		FILE *output = fopen(img_name, "wt");
		if (!output) {
			printf("Error opening this file.\n");
			return;
		}
		if (strcmp(img->magic_word, "P5") == 0 ||
			strcmp(img->magic_word, "P2") == 0)
			fprintf(output, "P2\n");  // grayscale in ascii
		if (strcmp(img->magic_word, "P6") == 0 ||
			strcmp(img->magic_word, "P3") == 0) {
			fprintf(output, "P3\n");  // rgb in ascii
		}
		//citesc valorile in fisier
		fprintf(output, "%d %d\n%d\n", img->col, img->row, img->maxx);
		if (strcmp(img->magic_word, "P5") == 0 ||
			strcmp(img->magic_word, "P2") == 0) {
			for (int i = 0; i < img->row; i++) {
				for (int j = 0; j < img->col; j++)
					fprintf(output, "%d ", img->matrix[i][j]);
				fprintf(output, "\n");
			}
		}
		if (strcmp(img->magic_word, "P6") == 0 ||
			strcmp(img->magic_word, "P3") == 0) {
			for (int i = 0; i < img->row; i++) {
				for (int j = 0; j < img->col; j++) {
					fprintf(output, "%d ", img->photo[i][j].R);
					fprintf(output, "%d ", img->photo[i][j].G);
					fprintf(output, "%d ", img->photo[i][j].B);
				}
				fprintf(output, "\n");
			}
		}
		printf("Saved %s\n", img_name);
		fclose(output); return;
	}
	FILE *output = fopen(img_name, "wb");
	if (!output) {
		printf("Error opening this file.\n");
		return;
	}
	//citesc valorile in fisier
	if (strcmp(img->magic_word, "P5") == 0 ||
		strcmp(img->magic_word, "P2") == 0)
		fprintf(output, "P5\n");  // grayscale in binar
	if (strcmp(img->magic_word, "P6") == 0 ||
		strcmp(img->magic_word, "P3") == 0)
		fprintf(output, "P6\n");// rgb in binar
	fprintf(output, "%d %d\n%d\n", img->col, img->row, img->maxx);
	if (strcmp(img->magic_word, "P5") == 0 ||
		strcmp(img->magic_word, "P2") == 0)
		for (int i = 0; i < img->row; i++)  // grayscale
			for (int j = 0; j < img->col; j++)
				fwrite(&img->matrix[i][j], sizeof(char), 1, output);
	if (strcmp(img->magic_word, "P6") == 0 ||
		strcmp(img->magic_word, "P3") == 0) {
		if (img && img->photo && output) {
			for (int i = 0; i < img->row; i++) {
				for (int j = 0; j < img->col; j++) {
					// Verificarea limitelor pentru img->photo
					if (i >= 0 && i < img->row && j >= 0 && j < img->col) {
						/* Scrierea datelor in fisier si
						verificarea rezultatului*/
						write_values(&(*img), output, i, j);
					} else {
						printf("Invalid index!\n");// Dacaa indexul este invalid
					}
				}
			}
		} else {
			printf("NULL Pointer\n");
		}
	}
	printf("Saved %s\n", img_name); fclose(output);
}

void read_matrix(image *img, FILE *input)
{
	//citire din fisier
	for (int i = 0; i < img->row; i++) {
		for (int j = 0; j < img->col; j++)
			fscanf(input, "%d", &img->matrix[i][j]);
	}
}

void read_matrix_binary(image *img, FILE *input)
{
	//citire din fisier binar
	fseek(input, 1, SEEK_CUR);
	unsigned char hue;
	for (int i = 0; i < img->row; i++) {
		for (int j = 0; j < img->col; j++) {
			fread(&hue, sizeof(char), 1, input);
			img->matrix[i][j] = (int)hue;
			// imi transform toate caracterele in valori pt nuantele de gri
		}
	}
}

void read_rgb(image *img, FILE *input)
{
	for (int i = 0; i < img->row; i++) {
		for (int j = 0; j < img->col; j++) {
			fscanf(input, "%d", &img->photo[i][j].R);
			fscanf(input, "%d", &img->photo[i][j].G);
			fscanf(input, "%d", &img->photo[i][j].B);
		}
	}
}

void read_rgb_binary(image *img, FILE *input)
{
	fscanf(input, "%d%d", &img->col, &img->row);
	fscanf(input, "%d", &img->maxx);
	img->photo = alloc_rgb(img->row, img->col);
	fseek(input, 1, SEEK_CUR);
	unsigned char hue;
	for (int i = 0; i < img->row; i++) {
		for (int j = 0; j < img->col; j++) {
			fread(&hue, sizeof(char), 1, input);
			img->photo[i][j].R = (int)hue;  // imi transform toate
			// caracterele in valori pt nuantele de gri
			fread(&hue, sizeof(char), 1, input);
			img->photo[i][j].G = (int)hue;
			fread(&hue, sizeof(char), 1, input);
			img->photo[i][j].B = (int)hue;
		}
	}
}

void LOAD(image *img, char img_name[])
{
	//daca exista o imagine inainte, ii dealoc resursele
	if (img->matrix) {
		free_matrix(img->matrix, img->row);
		img->nr_img = 0;
		img->matrix = NULL;
	}
	if (img->photo) {
		free_rgb(img->photo, img->row);
		img->nr_img = 0;
		img->photo = NULL;
	}
	FILE *input = fopen(img_name, "r");
	if (!input) {
		printf("Failed to load %s\n", img_name);
		return;
	}
	img->matrix = NULL;
	img->photo = NULL;
	char magic_word[5];
	fgets(magic_word, 5, input);
	magic_word[2] = '\0';  // pun terminatorul de sir dupa al 2-lea caracter
	// pentru a incepe comparatia
	// pentru imaginile de tip grayscale
	strcpy(img->magic_word, magic_word);
	if (!strcmp(magic_word, "P2")) {
		fscanf(input, "%d%d", &img->col, &img->row);
		fscanf(input, "%d", &img->maxx);
		img->matrix = alloc_matrix(img->row, img->col);
		read_matrix(img, input);  // ASCII
	}
	if (!strcmp(magic_word, "P5")) {
		fscanf(input, "%d%d", &img->col, &img->row);
		fscanf(input, "%d", &img->maxx);
		img->matrix = alloc_matrix(img->row, img->col);
		read_matrix_binary(img, input);  // binary
	}
	// pentru imaginile de tip rgb
	if (!strcmp(magic_word, "P3")) {
		fscanf(input, "%d%d", &img->col, &img->row);
		fscanf(input, "%d", &img->maxx);
		img->photo = alloc_rgb(img->row, img->col);
		read_rgb(img, input);  // ASCII
	}
	if (!strcmp(magic_word, "P6")) {
		fscanf(input, "%d%d", &img->col, &img->row);
		fscanf(input, "%d", &img->maxx);
		img->photo = alloc_rgb(img->row, img->col);
		fseek(input, 1, SEEK_CUR);
		unsigned char hue;
		for (int i = 0; i < img->row; i++) {
			for (int j = 0; j < img->col; j++) {
				fread(&hue, sizeof(char), 1, input);
				img->photo[i][j].R = (int)hue;  // imi transform
				// toate caracterele in valori pt nuantele de gri
				fread(&hue, sizeof(char), 1, input);
				img->photo[i][j].G = (int)hue;
				fread(&hue, sizeof(char), 1, input);
				img->photo[i][j].B = (int)hue;
			}
		}
	}
	img->x1 = 0;
	img->y1 = 0;
	img->x2 = img->col;
	img->y2 = img->row;
	img->nr_img++;
	printf("Loaded %s\n", img_name);
	fclose(input);
}

void swap(int *a, int *b)
{
	int aux = *a;
	*a = *b;
	*b = aux;
}

void SELECT(image *img, int x1, int x2, int y1, int y2)
{
	if (img->nr_img == 0) {
		printf("No image loaded\n");
		return;
	}
	if (x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0 || x1 == x2 || y1 == y2 ||
		x1 > img->col || x2 > img->col || y1 > img->row || y2 > img->row) {
		printf("Invalid set of coordinates\n");
		return;
	}
	//tratez cazul in care coordonatele sunt citite invers
	if (x1 > x2)
		swap(&x1, &x2);
	if (y1 > y2)
		swap(&y1, &y2);
	//actualizez coordonatele
	img->x1 = x1;
	img->x2 = x2;
	img->y1 = y1;
	img->y2 = y2;
	printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
}

void select_all(image *img)
{
	if (img->nr_img == 0) {
		printf("No image loaded\n");
		return;
	}
	img->x1 = 0;
	img->y1 = 0;
	img->x2 = img->col;
	img->y2 = img->row;
	printf("Selected ALL\n");
}

void clamp(int *n, int min, int max)
{
	//functie pt a aplica limitele de 0-255
	if (*n < min)
		*n = min;
	if (*n > max)
		*n = max;
}

void EDGE(image *img, int *apply)
{
	if (img->nr_img == 0) {
		printf("No image loaded\n");
		img->load_error = 1;
		return;
	}
	if (strcmp(img->magic_word, "P2") == 0 ||
		strcmp(img->magic_word, "P5") == 0) {
		printf("Easy, Charlie Chaplin\n");
		img->charlie = 1;
		return;
	}
	rgb * *tmp = alloc_rgb(img->row, img->col);
	//folosesc o matrice auxiliara pentru aplicarea filtrului
	for (int i = 0; i < img->row; i++) {
		for (int j = 0; j < img->col; j++) {
			tmp[i][j].R = img->photo[i][j].R;
			tmp[i][j].G = img->photo[i][j].G;
			tmp[i][j].B = img->photo[i][j].B;
		}
	}
	int edge[3][3] = {{-1, -1, -1}, {-1, 8, -1,}, {-1, -1, -1}};
	for (int i = img->y1; i < img->y2; i++) {
		for (int j = img->x1; j < img->x2; j++) {
			if (!(i <= 0 || j <= 0 || j >= img->col - 1 ||
				  i >= img->row - 1)) {  //exclud marginile
				int sumR = 0, sumB = 0, sumG = 0;
				for (int m = 0; m < 3; ++m) {
					for (int n = 0; n < 3; ++n) {
						sumR += img->photo[i + m - 1][j + n - 1].R *
						edge[m][n];
						sumB += img->photo[i + m - 1][j + n - 1].B *
						edge[m][n];
						sumG += img->photo[i + m - 1][j + n - 1].G *
						edge[m][n];
					}
				}
				tmp[i][j].R = sumR;
				tmp[i][j].G = sumG;
				tmp[i][j].B = sumB;
				clamp(&tmp[i][j].R, 0, 255);
				clamp(&tmp[i][j].G, 0, 255);
				clamp(&tmp[i][j].B, 0, 255);
			}
		}
	}
	for (int i = img->y1; i < img->y2; i++)
		for (int j = img->x1; j < img->x2; j++) {
			img->photo[i][j].R = tmp[i][j].R;
			img->photo[i][j].G = tmp[i][j].G;
			img->photo[i][j].B = tmp[i][j].B;
	}
	//pointer care imi arata daca a fost aplicat sau nu
	*apply = 1;
	free_rgb(tmp, img->row);
	printf("APPLY EDGE done\n");
}

void SHARPEN(image *img, int *apply)
{
	if (img->nr_img == 0) {
		printf("No image loaded\n");
		img->load_error = 1;
		return;
	}
	if (strcmp(img->magic_word, "P2") == 0 ||
		strcmp(img->magic_word, "P5") == 0) {
		printf("Easy, Charlie Chaplin\n");
		img->charlie = 1;
		return;
	}
	//folosesc o matrice auxiliara pentru aplicarea filtrului
	rgb * *tmp = alloc_rgb(img->row, img->col);
	for (int i = 0; i < img->row; i++) {
		for (int j = 0; j < img->col; j++) {
			tmp[i][j].R = img->photo[i][j].R;
			tmp[i][j].G = img->photo[i][j].G;
			tmp[i][j].B = img->photo[i][j].B;
		}
	}
	int sharpen[3][3] = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
	for (int i = img->y1; i < img->y2; i++) {
		for (int j = img->x1; j < img->x2; j++) {
			if (!(i <= 0 || j <= 0 || j >= img->col - 1 ||
				  i >= img->row - 1)) {  // exclud marginile
				int sumR = 0, sumB = 0, sumG = 0;
				for (int m = 0; m < 3; ++m) {
					for (int n = 0; n < 3; ++n) {
						sumR += img->photo[i + m - 1][j + n - 1].R *
						sharpen[m][n];
						sumB += img->photo[i + m - 1][j + n - 1].B *
						sharpen[m][n];
						sumG += img->photo[i + m - 1][j + n - 1].G *
						sharpen[m][n];
					}
				}
				tmp[i][j].R = sumR;
				tmp[i][j].G = sumG;
				tmp[i][j].B = sumB;
				clamp(&tmp[i][j].R, 0, 255);
				clamp(&tmp[i][j].G, 0, 255);
				clamp(&tmp[i][j].B, 0, 255);
			}
		}
	}
	for (int i = img->y1; i < img->y2; i++)
		for (int j = img->x1; j < img->x2; j++) {
			img->photo[i][j].R = tmp[i][j].R;
			img->photo[i][j].G = tmp[i][j].G;
			img->photo[i][j].B = tmp[i][j].B;
	}
	*apply = 1;
	//pointer care imi arata daca a fost aplicat sau nu
	free_rgb(tmp, img->row);
	printf("APPLY SHARPEN done\n");
}

void BLUR(image *img, int *apply)
{
	double sumR, sumG, sumB;
	if (img->nr_img == 0) {
		printf("No image loaded\n");
		img->load_error = 1;
		return;
	}
	if (strcmp(img->magic_word, "P2") == 0 ||
		strcmp(img->magic_word, "P5") == 0) {
		printf("Easy, Charlie Chaplin\n");
		img->charlie = 1;
		return;
	}
	//folosesc o matrice auxiliara pentru aplicarea filtrului
	rgb * *tmp = alloc_rgb(img->row, img->col);
	for (int i = 0; i < img->row; i++) {
		for (int j = 0; j < img->col; j++) {
			tmp[i][j].R = img->photo[i][j].R;
			tmp[i][j].G = img->photo[i][j].G;
			tmp[i][j].B = img->photo[i][j].B;
		}
	}
	int blur[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
	for (int i = img->y1; i < img->y2; i++) {
		for (int j = img->x1; j < img->x2; j++) {
			if (!(i <= 0 || j <= 0 || j >= img->col - 1 ||
				  i >= img->row - 1)) { //exclud marginile
				sumR = 0., sumG = 0., sumB = 0.;
				for (int m = 0; m < 3; ++m) {
					for (int n = 0; n < 3; ++n) {
						sumR += img->photo[i + m - 1][j + n - 1].R * blur[m][n];
						sumB += img->photo[i + m - 1][j + n - 1].B * blur[m][n];
						sumG += img->photo[i + m - 1][j + n - 1].G * blur[m][n];
					}
				}
				tmp[i][j].R = round(sumR /  9.);
				tmp[i][j].G = round(sumG / 9.);
				tmp[i][j].B = round(sumB / 9.);
				clamp(&tmp[i][j].R, 0, 255);
				clamp(&tmp[i][j].G, 0, 255);
				clamp(&tmp[i][j].B, 0, 255);
			}
		}
	}
	for (int i = img->y1; i < img->y2; i++)
		for (int j = img->x1; j < img->x2; j++) {
			img->photo[i][j].R = tmp[i][j].R;
			img->photo[i][j].G = tmp[i][j].G;
			img->photo[i][j].B = tmp[i][j].B;
	}
	//pointer care imi arata daca a fost aplicat sau nu
	*apply = 1;
	free_rgb(tmp, img->row);
	printf("APPLY BLUR done\n");
}

void GAUSSIAN_BLUR(image *img, int *apply)
{
	double sumR = 0, sumG = 0, sumB = 0;
	if (img->nr_img == 0) {
		printf("No image loaded\n");
		img->load_error = 1;
		return;
	}
	if (strcmp(img->magic_word, "P2") == 0 ||
		strcmp(img->magic_word, "P5") == 0) {
		printf("Easy, Charlie Chaplin\n");
		img->charlie = 1;
		return;
	}
	//folosesc o matrice auxiliara pentru aplicarea filtrului
	rgb * *tmp = alloc_rgb(img->row, img->col);
	for (int i = 0; i < img->row; i++) {
		for (int j = 0; j < img->col; j++) {
			tmp[i][j].R = img->photo[i][j].R;
			tmp[i][j].G = img->photo[i][j].G;
			tmp[i][j].B = img->photo[i][j].B;
		}
	}
	int gaussian[3][3] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};
	for (int i = img->y1; i < img->y2; i++) {
		for (int j = img->x1; j < img->x2; j++) {
			if (!(i <= 0 || j <= 0 || j >= img->col - 1 ||
				  i >= img->row - 1)){  // exclud marginile
				sumR = 0., sumG = 0., sumB = 0.;
				for (int m = 0; m < 3; ++m) {
					for (int n = 0; n < 3; ++n) {
						sumR += img->photo[i + m - 1][j + n - 1].R *
						gaussian[m][n];
						sumB += img->photo[i + m - 1][j + n - 1].B *
						gaussian[m][n];
						sumG += img->photo[i + m - 1][j + n - 1].G *
						gaussian[m][n];
					}
				}
				tmp[i][j].R = round(sumR / 16.);
				tmp[i][j].G = round(sumG / 16.);
				tmp[i][j].B = round(sumB / 16.);
				clamp(&tmp[i][j].R, 0, 255);
				clamp(&tmp[i][j].G, 0, 255);
				clamp(&tmp[i][j].B, 0, 255);
			}
		}
	}
	for (int i = img->y1; i < img->y2; i++)
		for (int j = img->x1; j < img->x2; j++) {
			img->photo[i][j].R = tmp[i][j].R;
			img->photo[i][j].G = tmp[i][j].G;
			img->photo[i][j].B = tmp[i][j].B;
	}
	// pointer care imi arata daca a fost aplicat sau nu
	*apply = 1;
	free_rgb(tmp, img->row);
	printf("APPLY GAUSSIAN_BLUR done\n");
}

int verify(int n)
{
	// functie care verifica daca un nr este putere a lui 2
	if (n <= 0)
		return 0;  // Numerele negative sau zero nu sunt puteri ale lui 2
	for (; n > 1; n /= 2) {
		if (n % 2 != 0)
			return 0;  // Daca nu este divizibil cu 2, nu este putere a lui 2
	}
	return 1;  // Daca ajungem la 1, este o putere a lui 2
}

void verify_HISTOGRAM(image *img)
{
	if (img->nr_img == 0) {
		printf("No image loaded\n");
		return;
	}
	if (strcmp(img->magic_word, "P3") == 0 ||
		strcmp(img->magic_word, "P6") == 0) {
		printf("Black and white image needed\n");
		return;
	}
}

void print_histogram(int *histogram, int y, int j)
{
	//functie care imi afiseza histograma
	for (int i = 0; i < y; i++) {
		printf("%d | ", histogram[i]);
		for (j = 0; j < histogram[i]; j++)
			printf("*");
		printf("\n");
	}
}

void HISTOGRAM(image *img, int x, int y)
{
	if (img->nr_img == 0) {
		printf("No image loaded\n");
		return;
	}
	if (strcmp(img->magic_word, "P3") == 0 ||
		strcmp(img->magic_word, "P6") == 0) {
		printf("Black and white image needed\n");
		return;
	}
	if (verify(y) == 0) {
		printf("Invalid set of parameters\n");
		return;
	}
	if (y > 256 || y < 0) {
		printf("Invalid set of parameters\n");
		return;
	}
	int star_sum = 0;
	int maxx = 0;
	int *freq = calloc(256, sizeof(int));
	if (!freq) {
		printf("allocating array failed\n");
		return;
	}
	for (int i = 0; i < img->row; i++)
		for (int j = 0; j < img->col; j++)
			freq[img->matrix[i][j]]++; //realizez un vector de
	//frecventa pt a memora aparitiile fiecarui elem din matrice
	int *histogram = calloc(256, sizeof(int));
	int k = 256 / y;//nr grupuri
	int j;
	for (int i = 0; i <= 255; i++) {
	//aflu maximul de nr de stele
		star_sum = star_sum + freq[i];
		if ((i + 1) % k == 0) {
			if (star_sum > maxx)
				maxx = star_sum;
			star_sum = 0;
		}
	}
	j = 0;
	//construiesc histograma
	for (int i = 0; i <= 255; i++) {
		star_sum = star_sum + freq[i];
		if ((i + 1) % k == 0) {
			histogram[j] = (int)(star_sum * x / maxx);
			j++;
			star_sum = 0;
		}
	}
	print_histogram(histogram, y, j);
	free(freq);
}

void ROTATE_LEFT_SELECT(image *img)
{
	if (strcmp(img->magic_word, "P2") == 0 ||
		strcmp(img->magic_word, "P5") == 0) {
		// grayscale
		int newrow = img->y2 - img->y1;
		int newcol = img->x2 - img->x1;
		// iau o matrice auxiliara in care sa imi pun selectia
		int **selected = alloc_matrix(newrow, newcol);
		for (int i = 0; i < newrow; i++) {
			for (int j = 0; j < newcol; j++)
				selected[i][j] = img->matrix[i + img->y1][j + img->x1];
		}
		// facem o auxiliara rotated in care punem valorile din
		// selected dar rotite
		int **rotated = alloc_matrix(newrow, newcol);
		for (int i = 0; i < newrow; i++)
			for (int j = 0; j < newcol; j++)
				rotated[newcol - j - 1][i] = selected[i][j];
		// actualizam valorile in matricea originala
		for (int i = 0; i < newrow; i++)
			for (int j = 0; j < newcol; j++)
				img->matrix[i + img->y1][j + img->x1] = rotated[i][j];
		// eliberam selected si rotated
		free_matrix(selected, newrow);
		free_matrix(rotated, newrow);
	}
	if (strcmp(img->magic_word, "P3") == 0 ||
		strcmp(img->magic_word, "P6") == 0) {
		// rgb
		int newrow = img->y2 - img->y1;
		int newcol = img->x2 - img->x1;
		// iau o matrice auxiliara in care sa imi pun selectia
		rgb **selected = alloc_rgb(newrow, newcol);
		for (int i = 0; i < newrow; i++) {
			for (int j = 0; j < newcol; j++)
				selected[i][j] = img->photo[i + img->y1][j + img->x1];
		}
		// facem o auxiliara rotated in care punem valorile din
		// selected dar rotite
		rgb **rotated = alloc_rgb(newrow, newcol);
		for (int i = 0; i < newrow; i++)
			for (int j = 0; j < newcol; j++)
				rotated[newcol - j - 1][i] = selected[i][j];
		// actualizam valorile in matricea originala
		for (int i = 0; i < newrow; i++)
			for (int j = 0; j < newcol; j++)
				img->photo[i + img->y1][j + img->x1] = rotated[i][j];
		// eliberam selected si rotated
		free_rgb(selected, newrow);
		free_rgb(rotated, newrow);
	}
}

void ROTATE_LEFT_SELECT_ALL(image *img)
{
	if (strcmp(img->magic_word, "P2") == 0 ||
		strcmp(img->magic_word, "P5") == 0) {
		// grayscale
		int **tmp = alloc_matrix(img->col, img->row);
		for (int i = 0; i < img->row; i++) {
			for (int j = 0; j < img->col; j++)
				tmp[img->col - j - 1][i] = img->matrix[i][j];
		}
		free_matrix(img->matrix, img->row);
		img->matrix = alloc_matrix(img->col, img->row);
		for (int i = 0; i < img->col; i++)
			for (int j = 0; j < img->row; j++)
				img->matrix[i][j] = tmp[i][j];
		// free matricea auxiliara
		free_matrix(tmp, img->col);
		//	actulizam dimensiunile
		swap(&img->row, &img->col);
		// actualizam selectia
		img->x1 = 0;
		img->x2 = img->col;
		img->y1 = 0;
		img->y2 = img->row;
	}
	if (strcmp(img->magic_word, "P3") == 0 ||
		strcmp(img->magic_word, "P6") == 0) {
		// rgb
		rgb **tmp = alloc_rgb(img->col, img->row);
		for (int i = 0; i < img->row; i++)
			for (int j = 0; j < img->col; j++)
				tmp[img->col - j - 1][i] = img->photo[i][j];
		// scapam de imaginea veche
		free_rgb(img->photo, img->row);
		// alocam spatiu nou pentru imaginea noua
		img->photo = alloc_rgb(img->col, img->row);
		// actualizam noua imagine obtinuta
		for (int i = 0; i < img->col; i++)
			for (int j = 0; j < img->row; j++)
				img->photo[i][j] = tmp[i][j];
		// free matricea auxiliara
		free_rgb(tmp, img->col);
		//actulizam dimensiunile
		swap(&img->row, &img->col);
		// actualizam selectia
		img->x1 = 0;
		img->x2 = img->col;
		img->y1 = 0;
		img->y2 = img->row;
	}
}

void ROTATE(image *img, int angle)
{
	int angle_array[9] = {-360, -270, -180, -90, 0, 90,
						  180, 270, 360}, ok_angle = 0;
	if (img->nr_img == 0) {
		printf("No image loaded\n");
		return;
	}
	//parcurg vectorul care contine valorile valide ale unghiului
	for (int i = 0; i < 9; i++) {
		if (angle_array[i] == angle) {
			ok_angle = 1;
			break;
		}
	}
	if (ok_angle == 0) {
		printf("Unsupported rotation angle\n");
		return;
	}
	if (((img->x2 - img->x1) != (img->y2 - img->y1)) && img->x1 != 0 &&
		img->x2 != img->col && img->y1 != 0 && img->y2 != img->row) {
		printf("The selection must be square\n");
		return;
	}
	//daca mi-a gasit unghiul valid
	if (ok_angle == 1) {
		if (angle == 360 || angle == -360 || angle == 0)
			printf("Rotated %d\n", angle);
		if (angle == 90 || angle == -270) {
			if (img->x1 == 0 && img->x2 == img->col && img->y1 == 0 &&
				img->y2 == img->row) {
				//selectia totala
				ROTATE_LEFT_SELECT_ALL(img);
				ROTATE_LEFT_SELECT_ALL(img);
				ROTATE_LEFT_SELECT_ALL(img);
			} else {
				ROTATE_LEFT_SELECT(img);
				ROTATE_LEFT_SELECT(img);
				ROTATE_LEFT_SELECT(img);
			}
			printf("Rotated %d\n", angle);
		}
		if (angle == 180 || angle == -180) {
			if (img->x1 == 0 && img->x2 == img->col && img->y1 == 0 &&
				img->y2 == img->row) {//selectia totala
				ROTATE_LEFT_SELECT_ALL(img);
				ROTATE_LEFT_SELECT_ALL(img);
			} else {
				ROTATE_LEFT_SELECT(img);
				ROTATE_LEFT_SELECT(img);
			}
			printf("Rotated %d\n", angle);
		}
		if (angle == -90 || angle == 270) {
			if (img->x1 == 0 && img->x2 == img->col && img->y1 == 0 &&
				img->y2 == img->row)//selectia totala
				ROTATE_LEFT_SELECT_ALL(img);
			else
				ROTATE_LEFT_SELECT(img);
			printf("Rotated %d\n", angle);
		}
	}
}

void EXIT(image *img)
{
	if (img->nr_img == 0) {
		printf("No image loaded\n");
		free(img);
		return;
	}
	//daca matricea incarcata este grayscale o eliberez
	if (img->matrix)
		free_matrix(img->matrix, img->row);
	//daca imaginea incarcata este rgb , o eliberez
	if (img->photo)
		free_rgb(img->photo, img->row);
	// free la structura
	free(img);
}

int sum(int x, int freq[])
{
	int s = 0;
	for (int i = 0; i <= x; i++)
		s = s + freq[i];
	return s;
}

void EQUALIZE(image *img)
{
	if (img->nr_img == 0) {
		printf("No image loaded\n");
		return;
	}
	if (strcmp(img->magic_word, "P3") == 0 ||
		strcmp(img->magic_word, "P6") == 0) {
		printf("Black and white image needed\n");
		return;
	}
	int *freq = calloc(256, sizeof(int));
	if (!freq) {
		printf("calloc failed!\n");
		return;
	}
	//construim vectorul de frecventa
	for (int i = 0; i < img->row; i++) {
		for (int j = 0; j < img->col; j++)
			freq[img->matrix[i][j]]++;
	}
	//egalizam imaginea respectand formula
	for (int i = 0; i < img->row; i++) {
		for (int j = 0; j < img->col; j++) {
			int s = sum(img->matrix[i][j], freq);
			int area = img->row * img->col;
			img->matrix[i][j] = (int)round((double)255 * s / area);
		}
	}
	free(freq);
	printf("Equalize done\n");
}

void CROP(image *img)
{
	if (img->nr_img == 0) {
		printf("No image loaded\n");
		return;
	}
	if (strcmp(img->magic_word, "P5") == 0 ||
		strcmp(img->magic_word, "P2") == 0) {
		int newrow = img->y2 - img->y1;
		int newcol = img->x2 - img->x1;
		int **cropped = alloc_matrix(newrow, newcol);
		for (int i = img->y1; i < img->y2; i++) {
			for (int j = img->x1; j < img->x2; j++)
				cropped[i - img->y1][j - img->x1] = img->matrix[i][j];
			// redimensionarea matricii dorite
		}
		// eliberarea memoriei matricei vechi
		if (img->matrix)
			free_matrix(img->matrix, img->row);
		// actualizarea ei
		img->matrix = alloc_matrix(newrow, newcol);
		for (int i = 0; i < newrow; i++) {
			for (int j = 0; j < newcol; j++)
				img->matrix[i][j] = cropped[i][j];
		}
		// eliberare matrice aux
		if (cropped)
			free_matrix(cropped, newrow);
		// Actualizare dimensiuni
		img->row = newrow;
		img->col = newcol;
		img->x1 = 0;
		img->y1 = 0;
		img->x2 = newcol;
		img->y2 = newrow;
		img->nr_img = 1;
		printf("Image cropped\n");
	}
	if (strcmp(img->magic_word, "P6") == 0 ||
		strcmp(img->magic_word, "P3") == 0) {
		int newrow = img->y2 - img->y1;
		int newcol = img->x2 - img->x1;
		rgb **cropped = alloc_rgb(newrow, newcol);
		for (int i = img->y1; i < img->y2; i++) {
			for (int j = img->x1; j < img->x2; j++)
				cropped[i - img->y1][j - img->x1] = img->photo[i][j];
		// redimensionarea matricii dorite
		}
		// eliberarea memoriei matricei vechi
		free_rgb(img->photo, img->row);
		// actualizarea ei
		img->photo = alloc_rgb(newrow, newcol);
		for (int i = 0; i < newrow; i++) {
			for (int j = 0; j < newcol; j++)
				img->photo[i][j] = cropped[i][j];
		}
		// eliberare matrice aux
		free_rgb(cropped, newrow);
		//  Actualizare dimensiuni
		img->row = newrow;
		img->col = newcol;
		img->x1 = 0;
		img->y1 = 0;
		img->x2 = newcol;
		img->y2 = newrow;
		printf("Image cropped\n");
	}
}

void APPLY_FILTER(image *img, char line[])
{
	int apply = 0; //daca s-a putut realiza apply-ul
	if (img->nr_img == 1 && strcmp(line, "\n") == 0) {
		printf("Invalid command\n");
	} else {
		img->load_error = 0;
		if (strcmp(line, "EDGE") == 0)
			EDGE(img, &apply);
		if (strcmp(line, "SHARPEN") == 0)
			SHARPEN(img, &apply);
		if (strcmp(line, "BLUR") == 0)
			BLUR(img, &apply);
		if (strcmp(line, "GAUSSIAN_BLUR") == 0)
			GAUSSIAN_BLUR(img, &apply);
		if (apply == 0) {
			if (img->charlie == 0 && img->load_error == 0)
				printf("APPLY parameter invalid\n");
			img->charlie = 0;
		}
	}
}

void get_line(char line[], int *nr)
{
	//obtinem urmatorul cuvant de pe linie
	char w;
	do {
		scanf("%c", &w);
		if (w != ' ') {
			line[(*nr)] = w;
			(*nr)++;
		}
	} while (w != '\n');
}

void get_enter(void)
{
	char c;
	scanf("%c", &c);
	while (c != '\n')
		scanf("%c", &c);
}

int get_coordinates(char command[], int *x1, int *y1, int *x2,
					int *y2, int *nr)
{
	(*x1) = atoi(command);
	char line[1000];
	fgets(line, 1000, stdin);
	//verific daca am numarul corect de coordonate
	if (sscanf(line, "%d%d%d", &(*y1), &(*x2), &(*y2)) == 3) {
		(*nr) = 3;
		return 1;
	}
	printf("Invalid command\n");
	return 0;
}
