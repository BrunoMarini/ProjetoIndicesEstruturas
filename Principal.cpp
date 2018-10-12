#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>

enum menuPrincipal { consultarRegistros = 1, alterarRegistro, apagarRegistro, inserirRegistro, impressao, sair };
enum menuImpressao { chaveOrdenada = 1, nomeOrdenado, categoriaChaveOrdenado, categoriaNomeOrdenado, imprimirArquivo };

typedef struct {
	int chave;
	char nomeCarro[50];
	char montadora[50];
	float consumo;
	int peso;

} no;

no* salvarNovoArquivo(no, no*, int*);
no* adicionarRegistros(no*, int*);
no* carregarBaseDados(int*, no*);
void impressoes(no*);

int verificaChave(int, no*, int);

void main()
{
	menuPrincipal opcao;
	no aux;
	no *cadastros = NULL;
	int qtdCadastros;

	cadastros = carregarBaseDados(&qtdCadastros, cadastros);

	printf("-------------------- Menu --------------------\n");
	printf(" 1. Consultar um registro;\n");
	printf(" 2. ALterar/Atualizar campo(s) de um registro;\n");
	printf(" 3. Eliminar um registro do arquivo;\n");
	printf(" 4. Inserir um registro no arquivo;\n");
	printf(" 5. Impressao;\n");
	printf(" 6. Sair;\n\n");
	printf(" Opcao desejada: ");
	scanf("%d", &opcao);

	switch (opcao)
	{
	case consultarRegistros:
		break;
	case alterarRegistro:
		break;
	case apagarRegistro:
		break;
	case inserirRegistro:
		cadastros = adicionarRegistros(cadastros, &qtdCadastros);
		break;
	case impressao:
		impressoes(cadastros);
		break;
	case sair:
		break;
	default:
		break;
	}



	system("pause");
}

no* carregarBaseDados(int *qtd, no *cadastros)
{
	no aux;
	int i, qtdCadastros;

	FILE *arquivo = fopen("database.dat", "rb");

	fread(qtd, sizeof(int), 1, arquivo);

	qtdCadastros = *qtd;

	cadastros = (no*)malloc(qtdCadastros * sizeof(no));

	for (i = 0; i < qtdCadastros; i++)
	{
		fread(&aux, sizeof(no), 1, arquivo);
		cadastros[i] = aux;
	}
	fclose(arquivo);

	return(cadastros);
}

no* adicionarRegistros(no* cadastros, int *qtd)
{
	//FILE *arq = fopen("carros.dat", "a+b");
	no aux;
	int i, aleatorio, op;

	system("cls");

	do
	{
		printf("----- Adicionar Registro ----\n\n");

		getchar();

		printf("Digite o nome do carro: ");
		fgets(aux.nomeCarro, 50, stdin);

		printf("Digite a montadora: ");
		fgets(aux.montadora, 50, stdin);

		printf("Digite o consumo do veiculo: ");
		scanf("%f", &aux.consumo);

		printf("Digite o peso do veiculo: ");
		scanf("%i", &aux.peso);

		do
		{
			aleatorio = 1000 + (rand() % 9999);

		} while (verificaChave(aleatorio, cadastros, (*qtd)) != 0 || aleatorio <= 1000 || aleatorio >= 9999);

		aux.chave = aleatorio;

		cadastros = salvarNovoArquivo(aux, cadastros, qtd);

		printf("\n Deseja cadastrar outro veiculo?\n");
		printf(" 1. SIM;\n");
		printf(" 2. NAO;\n\n");
		printf(" Opcao: ");
		scanf("%i", &op);

	} while (op != 2);

	return (cadastros);

}

no* salvarNovoArquivo(no novo, no *cadastros, int *qtd)
{
	FILE *arquivo = fopen("database.dat", "wb");
	int i;

	(*qtd)++;

	fwrite(qtd, sizeof(int), 1, arquivo);

	for (i = 0; i < ((*qtd) - 1); i++)
		fwrite(&cadastros[i], sizeof(no), 1, arquivo);

	fwrite(&novo, sizeof(no), 1, arquivo);

	fclose(arquivo);

	return(carregarBaseDados(qtd, cadastros));

}

int verificaChave(int x, no* cadastrados, int tam)
{
	for (int i = 0; i <= tam; i++)
	{
		if (x == cadastrados[i].chave)
			return 1;
	}
	return 0;
}

void impressoes(no* cadastros)
{
	menuImpressao opcao;

	system("cls");

	printf("----- Menu Impressao -----\n\n");
	printf("1. Imprimir todos por chave primaria ordenada;\n");
	printf("2. Imprimir todos ordenado pelo nome do carro;\n");
	printf("3. Imprimir todos por categoria ordenados pela chave;\n");
	printf("4. Imprimir todos por categoria ordenados pelo nome do carro\n;");
	printf("5. Imprimir arquivo;\n\n");
	printf("Opcao: ");
	scanf("%i", &opcao);

	switch (opcao)
	{
	case chaveOrdenada:
		break;
	case nomeOrdenado:
		break;
	case categoriaChaveOrdenado:
		break;
	case categoriaNomeOrdenado:
		break;
	case imprimirArquivo:
		break;
	default:
		break;
	}

	
}
