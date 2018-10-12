#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
	int chave;
	char nomeCarro[50];
	char montadora[50];
	float consumo;
	int peso;

} no;

void fimDeLinha(no*);

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
		getchar();
		fgets(aux.nomeCarro, 50, stdin);
		fgets(aux.montadora, 50, stdin);
		scanf("%f", &aux.consumo);
		scanf("%i", &aux.peso);

		fimDeLinha(&aux);

		fwrite(&aux, sizeof(no), 1, p);
	}

	system("pause");
}

void fimDeLinha(no* x)
{
	for (int i = 0; i < strlen(x->nomeCarro); i++)
		if (x->nomeCarro[i] == '\n')
			x->nomeCarro[i] = '\0';

	for (int i = 0; i < strlen(x->montadora); i++)
		if (x->montadora[i] == '\n')
			x->montadora[i] = '\0';
}
