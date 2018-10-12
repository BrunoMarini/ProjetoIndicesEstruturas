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

typedef struct {
	int chave;
	int posicao;
} listaDenso;

no* salvarNovoArquivo(no, no*, int*);
no* adicionarRegistros(no*, int*);
no* carregarBaseDados(int*, no*);
no* modificarRegistro(no*, listaDenso*, int);
listaDenso* criarIndiceDenso(int, no*, listaDenso*);
void impressoes(no*, listaDenso*, int qtd);
void consultarRegistro(no*, listaDenso*, int);
void bubble(listaDenso[], int);
int buscaBinaria(listaDenso*, int, int);

int verificaChave(int, no*, int);

void main()
{
	menuPrincipal opcao;
	no *cadastros = NULL;
	listaDenso *indiceDenso = NULL;
	int qtdCadastros;

	cadastros = carregarBaseDados(&qtdCadastros, cadastros);
	indiceDenso = criarIndiceDenso(qtdCadastros, cadastros, indiceDenso);

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
		consultarRegistro(cadastros, indiceDenso, qtdCadastros);
		break;
	case alterarRegistro:
		cadastros = modificarRegistro(cadastros, indiceDenso, qtdCadastros);
		break;
	case apagarRegistro:
		break;
	case inserirRegistro:
		cadastros = adicionarRegistros(cadastros, &qtdCadastros);
		indiceDenso = criarIndiceDenso(qtdCadastros, cadastros, indiceDenso);
		break;
	case impressao:
		impressoes(cadastros, indiceDenso, qtdCadastros);
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
	int aleatorio, op;

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

	if(novo.chave != -1)
		(*qtd)++;

	fwrite(qtd, sizeof(int), 1, arquivo);

	for (i = 0; i < ((*qtd) - 1); i++)
		fwrite(&cadastros[i], sizeof(no), 1, arquivo);

	if(novo.chave != -1)
		fwrite(&novo, sizeof(no), 1, arquivo);

	fclose(arquivo);

	return(carregarBaseDados(qtd, cadastros));

}

no* modificarRegistro(no* cadastros, listaDenso* indice, int qtd)
{
	int posicao, codigo, op, i, j, peso;
	char aux[50];
	float consumo;
	no x;

	system("cls");

	printf("----- Modificar Registro -----\n\n");
	printf("Digite o codigo que deseja encontrar: ");
	scanf("%i", &codigo);

	posicao = buscaBinaria(indice, qtd, codigo);

	if (posicao != -1)
	{
		printf("\n Chave  Nome Carro      Montadora       Consumo  Peso\n\n");

		printf(" %i   ", cadastros[posicao].chave);
		printf("%s ", cadastros[posicao].nomeCarro);
		for (j = 15 - strlen(cadastros[posicao].nomeCarro); j > 0; j--) printf(" ");
		printf("%s ", cadastros[posicao].montadora);
		for (j = 15 - strlen(cadastros[posicao].montadora); j > 0; j--) printf(" ");
		if (cadastros[posicao].consumo < 10) printf("0");
		printf("%.2f ", cadastros[posicao].consumo);
		printf("   %i\n", cadastros[posicao].peso);

		printf("\nDeseja alterar o nome do carro?\n\n");
		printf("1. Sim;\n2. Nao;\n\nOpcao: ");
		scanf("%i", &op);
		
		if (op == 1)
		{
			getchar();
			printf("\nDigite o novo nome ao carro: ");
			fgets(aux, 50, stdin);
			for (i = 0; i < strlen(aux); i++)
				if (aux[i] == '\n')
					aux[i] = '\0';
			strcpy(cadastros[posicao].nomeCarro, aux);
		}

		printf("\nDeseja alterar o nome da montadora?\n\n");
		printf("1. Sim;\n2. Nao;\n\nOpcao: ");
		scanf("%i", &op);

		if (op == 1)
		{
			getchar();
			printf("\nDigite o novo nome a montadora:");
			fgets(aux, 50, stdin);
			for (i = 0; i < strlen(aux); i++)
				if (aux[i] == '\n')
					aux[i] = '\0';
			strcpy(cadastros[posicao].montadora, aux);
		}

		printf("\nDeseja alterar o consumo do carro?\n\n");
		printf("1. Sim;\n2. Nao;\n\nOpcao: ");
		scanf("%i", &op);

		if (op == 1)
		{
			printf("\nDigite o novo consumo: ");
			scanf("%f", &consumo);
			cadastros[posicao].consumo = consumo;
		}

		printf("\nDeseja alterar o peso do carro?\n\n");
		printf("1. Sim;\n2. Nao;\n\nOpcao: ");
		scanf("%i", &op);

		if (op == 1)
		{
			printf("\nDigite o novo peso: ");
			scanf("%i", &peso);
			cadastros[posicao].peso = peso;
		}
		
		x.chave = -1;

		printf("\n\n Atualizado com Sucesso!\n");

		return(salvarNovoArquivo(x, cadastros, &qtd));

	}
	else
	{
		printf("ERRO! NAO FOI ENCONTRADO O CADASTRO DE CHAVE %i NA BASE DE DADOS\n", codigo);
		return(cadastros);
	}


}

void impressoes(no* cadastros, listaDenso* lista, int qtd)
{
	int i, j;
	int posicao;
	menuImpressao opcao;

	system("cls");

	printf("----- Menu Impressao -----\n\n");
	printf("1. Imprimir todos por chave primaria ordenada;\n");
	printf("2. Imprimir todos ordenado pelo nome do carro;\n");
	printf("3. Imprimir todos por categoria ordenados pela chave;\n");
	printf("4. Imprimir todos por categoria ordenados pelo nome do carro;\n");
	printf("5. Imprimir arquivo;\n\n");
	printf("Opcao: ");
	scanf("%i", &opcao);

	system("cls");

	switch (opcao)
	{
	case chaveOrdenada:

		printf(" Chave  Nome Carro      Montadora       Consumo  Peso\n\n");

		for (i = 0; i < qtd; i++)
		{
			posicao = lista[i].posicao;

			printf(" %i   ", cadastros[posicao].chave);
			printf("%s ", cadastros[posicao].nomeCarro);
			for (j = 15 - strlen(cadastros[posicao].nomeCarro); j > 0; j--) printf(" ");
			printf("%s ", cadastros[posicao].montadora);
			for (j = 15 - strlen(cadastros[posicao].montadora); j > 0; j--) printf(" ");
			if (cadastros[posicao].consumo < 10) printf("0");
			printf("%.2f ", cadastros[posicao].consumo);
			printf("   %i\n", cadastros[posicao].peso);
		}

		break;
	case nomeOrdenado:
		break;
	case categoriaChaveOrdenado:
		break;
	case categoriaNomeOrdenado:
		break;
	case imprimirArquivo:

		printf(" Chave  Nome Carro      Montadora       Consumo  Peso\n\n");

		for (i = 0; i < qtd; i++)
		{
			printf(" %i   ", cadastros[i].chave);
			printf("%s ", cadastros[i].nomeCarro);
			for (j = 15 - strlen(cadastros[i].nomeCarro); j > 0; j--) printf(" ");
			printf("%s ", cadastros[i].montadora);
			for (j = 15 - strlen(cadastros[i].montadora); j > 0; j--) printf(" ");
			if (cadastros[i].consumo < 10) printf("0");
			printf("%.2f ", cadastros[i].consumo);
			printf("   %i\n", cadastros[i].peso);
		}

		break;
	default:
		break;
	}


}

listaDenso* criarIndiceDenso(int qtdCadastros, no* cadastros, listaDenso* indice)
{
	int i;

	indice = (listaDenso*)malloc(qtdCadastros * sizeof(listaDenso));

	for (i = 0; i < qtdCadastros; i++)
	{
		indice[i].chave = cadastros[i].chave;
		indice[i].posicao = i;
	}

	bubble(indice, qtdCadastros);

	return (indice);

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

void bubble(listaDenso lista[], int tam)
{
	int i, j;
	listaDenso aux;
	int teste = 1;

	for (i = 0; i < tam - 1 && teste; i++)
	{
		teste = 0;

		for (j = 0; j < tam - i - 1; j++)
		{
			if (lista[j].chave > lista[j + 1].chave)
			{
				teste = 1;
				aux.chave = lista[j].chave;
				aux.posicao = lista[j].posicao;

				lista[j].chave = lista[j + 1].chave;
				lista[j].posicao = lista[j + 1].posicao;

				lista[j + 1].chave = aux.chave;
				lista[j + 1].posicao = aux.posicao;
			}
		}
	}
}

void consultarRegistro(no* cadastros, listaDenso* indice, int qtd)
{
	int codigo, posicao, j;

	system("cls");

	printf("----- Consultar Registro -----\n\n");
	printf("Digite o codigo que deseja encontrar: ");
	scanf("%i", &codigo);

	posicao = buscaBinaria(indice, qtd, codigo);

	system("cls");

	if (posicao != -1)
	{
		printf(" Chave  Nome Carro      Montadora       Consumo  Peso\n\n");

		printf(" %i   ", cadastros[posicao].chave);
		printf("%s ", cadastros[posicao].nomeCarro);
		for (j = 15 - strlen(cadastros[posicao].nomeCarro); j > 0; j--) printf(" ");
		printf("%s ", cadastros[posicao].montadora);
		for (j = 15 - strlen(cadastros[posicao].montadora); j > 0; j--) printf(" ");
		if (cadastros[posicao].consumo < 10) printf("0");
		printf("%.2f ", cadastros[posicao].consumo);
		printf("   %i\n", cadastros[posicao].peso);
	}
	else
	{
		printf("ERRO! NAO FOI ENCONTRADO O CADASTRO DE CHAVE %i NA BASE DE DADOS\n", codigo);
	}

}

int buscaBinaria(listaDenso* indice, int tam, int n)
{
	int sup = 0, inf = tam - 1, meio;

	while (inf >= sup)
	{
		meio = (sup + inf) / 2;

		if (indice[meio].chave == n)
			return (indice[meio].posicao);
		if (indice[meio].chave > n)
			inf = meio - 1;
		else
			sup = meio + 1;
	}

	return (-1);

}
