#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>

typedef struct {
	int chave;
	char nomeCarro[50];
	char montadora[50];
	float consumo;
	int peso;

} no;

void main() 
{
	FILE *p;
	no aux;
	int tam = 50;

	p = fopen("database.dat", "wb");

	fwrite(&tam, sizeof(int), 1, p);

	for (int i = 0; i < tam; i++)
	{
		scanf("%i", &aux.chave);
		fgets(aux.nomeCarro, 50, stdin);
		fgets(aux.montadora, 50, stdin);
		scanf("%f", &aux.consumo);
		scanf("%i", &aux.peso);

		fwrite(&aux, sizeof(no), 1, p);
	}

	system("pause");
}