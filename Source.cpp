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

void criar_vazia(no* pri);
int inserir(int pri,no tab[],int* disp, variavel V);
no* buscar(no* pri[],char nome[]);


int main()
{
	no* pri[MAX], ende;
	no tab[DIM];
	variavel V;
	char nome[10];
	int disp;

	criar_vazia(pri); //ler info para V

	int t = inserir(pri,V);

	ende = buscar(pri,nome);


}

void criar_vazia(no * pri)
{
	for (int i = 0; i < MAX; i++) pri[i] = NULL;
}

int inserir(int pri, no tab[], int* disp, variavel V)
{
	no* novo = (no*)malloc(sizeof(no));
	int p;
	if (!novo) return 0;
	novo = *disp;
	*disp = tab[*disp].link;
	tab[novo].info = V //novo->info = V
	p = Hash(var_nome);
	tab[novo].link = pri[p];
	pri[p] = novo;
	return 1;

}

no* buscar(no* pri[], char nome[])
{
	no* x;
	int p = Hash(nome);
	x = pri[p];
	while (x) if (strcmp(x->info.var_nome, nome) == 0) return x;
	x = x->link;
	return NULL;
}