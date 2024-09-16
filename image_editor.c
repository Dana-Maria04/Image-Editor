// Copyright Caruntu Dana-Maria 311CAa 2023-2024
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"

int main(void)
{
	char command[100], img_name[100];
	int x, y, z, x1, x2, y1, y2, nr;
	image *img = malloc(1 * sizeof(image));// aloc memorie pentru structura mea
	scanf("%s", command);
	img->nr_img = 0; img->ascii = 0;
	img->matrix = NULL; img->photo = NULL;//pornesc fara imagini
	//incep compararea cu comenzile cerute
	while (strcmp(command, "EXIT") != 0) {
		if (strcmp(command, "LOAD") == 0) {
			scanf("%s", img_name);//citesc numele fisierului
			LOAD(img, img_name);
		} else if (strcmp(command, "SELECT") == 0) {
			//citesc urmatorul cuvant
			scanf("%s", command);
			if (strcmp(command, "ALL") == 0) {
				select_all(img);
			} else {
				if (get_coordinates(command, &x1, &y1, &x2, &y2, &nr))
					SELECT(img, x1, x2, y1, y2);
			}
		} else if (strcmp(command, "EQUALIZE") == 0) {
			EQUALIZE(img);
		} else if (strcmp(command, "CROP") == 0) {
			CROP(img);
		} else if (strcmp(command, "APPLY") == 0) {
			nr = 0;
			char line[100] = {'\0'};
			//obtin urmatorul cuvant de dupa APPLY
			get_line(line, &nr);
			if (line[0] == '\0') {
				printf("Invalid command\n");
			} else {
				if (nr > 1)
					line[nr - 1] = '\0';
				if (img->nr_img == 0 && strcmp(line, "\n") == 0)
					printf("No image loaded\n");
				else
					APPLY_FILTER(img, line);
			}
		} else if (strcmp(command, "HISTOGRAM") == 0) {
			char line[1000];
			fgets(line, 1000, stdin);
			if (img->nr_img == 0) {
				printf("No image loaded\n");
			} else {
				if (sscanf(line, "%d%d%d", &x, &y, &z) == 2)
					HISTOGRAM(img, x, y);
				else
					printf("Invalid command\n");
			}
		} else if (strcmp(command, "ROTATE") == 0) {
			int angle;
			scanf("%d", &angle);
			ROTATE(img, angle);
		} else if (strcmp(command, "SAVE") == 0) {
			scanf("%s", img_name);
			char asc[10];
			scanf("%s", asc);
			//verificarea daca salvez sub format ascii
			if (strcmp(asc, "ascii") == 0) {
				img->ascii = 0;
			} else {
				img->ascii = 1;
				//lipesc la comanda stringul asc ca sa nu o pierd
				strcpy(command, asc);
			}
			SAVE(img, img_name);
		} else if (strcmp(command, "EXIT") != 0) {
			printf("Invalid command\n");
			//obtin urmatorul enter pentru a putea trece la urmatoarea comanda
			get_enter();
		}
		if (img->ascii == 0)
			scanf("%s", command);
		img->ascii = 0;
	}
	EXIT(img);
	return 0;
}
