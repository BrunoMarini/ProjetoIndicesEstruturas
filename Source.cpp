#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 10
#define DIM 16

typedef struct {
	char var_nome[10];
	int tipo;//0:int, 1:float, 2:char
	int valor;
}variavel;

typedef struct no {
	variavel info;
	no* link;
}no;

void criar_vazia(no* pri[]);
int inserir(np *pri[],variavel V);
no* buscar(no* pri[],char nome[]);
int hash(char nome[]);

int main()
{
	no* pri[MAX], *ende;
	variavel V;
	char nome[10];

	criar_vazia(pri); //ler info para V

	inserir(pri,V);

	ende = buscar(pri,nome);


}

void criar_vazia(no * pri)
{
	for (int i = 0; i < MAX; i++) pri[i] = NULL;
}

int inserir(np *pri[],variavel V)
{
	no* novo = (no*)malloc(sizeof(no));
	int p;
	if (!novo) return 0;
	novo->info = V;
	p = hash(V.var_nome);
	pri[p] = novo;
	return 1;

}

no* buscar(no* pri[], char nome[])
{
	no* address;
	int p = hash(nome);
	address = pri[p];
	while (address) if (strcmp(address->info.var_nome, nome) == 0) return address;
	address = address->link;
	return NULL;
}
int hash(char nome[])
{
	int j = 0, soma = 0;
	while(nome[i] != '\0') soma += nome[i];
	return (soma % MAX);
}
