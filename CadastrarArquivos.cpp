#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

int verifica(int, int*, int);

void main()
{
	FILE* arquivo;
	int aleatorios[50];
	int aux;
	float consumo;
	char nome[50];

	for (int i = 0; i < 50; i++)
	{
		do 
		{
			aux = 1000 + (rand() % 9999);

		} while (verifica(aux, aleatorios, i) != 0 || aux <= 1000 || aux >= 9999);

			aleatorios[i] = aux;

		//printf("%2i. %d\n", i, aux);
	}


	arquivo = fopen("base.txt", "a");

	aux = 0;

	for (int i = 0; i < 50; i++)
	{
		fprintf(arquivo, "%d\n", aleatorios[aux]);

		printf("Carro: ");
		fgets(nome, 50, stdin);
		fprintf(arquivo, "%s", nome);

		printf("Montadora: ");
		fgets(nome, 50, stdin);
		fprintf(arquivo, "%s", nome);

		printf("Consumo: ");
		scanf("%f", &consumo);
		fprintf(arquivo, "%f\n", consumo);

		printf("Peso: ");
		scanf("%f", &consumo);
		fprintf(arquivo, "%f\n", consumo);

		getchar();
	}
	system("pause");
}

int verifica(int x, int ale[], int tam)
{
	for (int i = 0; i <= tam; i++)
	{
		if (x == ale[i])
			return 1;
	}
	return 0;
}