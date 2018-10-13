#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
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

typedef struct {
	int posicao;
	char nome[50];
} tabInv;

no* salvarNovoArquivo(no, no*, int*);
no* adicionarRegistros(no*, int*);
no* carregarBaseDados(int*, no*);
no* modificarRegistro(no*, listaDenso*, int);
no* eliminarRegistro(no*, listaDenso*, int*);
listaDenso* criarIndiceDenso(int, no*, listaDenso*);
void impressoes(no*, listaDenso*, int, int, int*, char[5][50], int[], int[]);
void catChaveOrd(no*, int[], int*, int, char[5][50]);
void consultarRegistro(no*, listaDenso*, int);
void bubble(listaDenso[], int);
int buscaBinaria(listaDenso*, int, int);
int* tabelaInversa(int*, int*, no*, int);

int verificaChave(int, no*, int);

void main()
{
	menuPrincipal opcao;
	no *cadastros = NULL;
	listaDenso *indiceDenso = NULL;
	int qtdCadastros, count, posicao;
	int *ordemAlfabetica = NULL, pri;
	int pri_cat[5], catNome[100];
	char montadoras[5][50] = { "Volkswagen", "Chevrolet", "Ford", "Fiat", "Hyundai" };

	cadastros = carregarBaseDados(&qtdCadastros, cadastros);
	indiceDenso = criarIndiceDenso(qtdCadastros, cadastros, indiceDenso);
	ordemAlfabetica = tabelaInversa(ordemAlfabetica, &pri, cadastros, qtdCadastros);
	catChaveOrd(cadastros, pri_cat, catNome, qtdCadastros, montadoras);


	do
	{

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
			ordemAlfabetica = tabelaInversa(ordemAlfabetica, &pri, cadastros, qtdCadastros);
			catChaveOrd(cadastros, pri_cat, catNome, qtdCadastros, montadoras);
			break;
		case apagarRegistro:
			cadastros = eliminarRegistro(cadastros, indiceDenso, &qtdCadastros);
			indiceDenso = criarIndiceDenso(qtdCadastros, cadastros, indiceDenso);
			ordemAlfabetica = tabelaInversa(ordemAlfabetica, &pri, cadastros, qtdCadastros);
			catChaveOrd(cadastros, pri_cat, catNome, qtdCadastros, montadoras);
			break;
		case inserirRegistro:
			cadastros = adicionarRegistros(cadastros, &qtdCadastros);
			indiceDenso = criarIndiceDenso(qtdCadastros, cadastros, indiceDenso);
			ordemAlfabetica = tabelaInversa(ordemAlfabetica, &pri, cadastros, qtdCadastros);
			catChaveOrd(cadastros, pri_cat, catNome, qtdCadastros, montadoras);
			break;
		case impressao:
			impressoes(cadastros, indiceDenso, qtdCadastros, pri, ordemAlfabetica, montadoras, pri_cat, catNome);
			break;
		case sair:
			break;
		default:
			break;
		}

	} while (opcao != 6);

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
	int aleatorio, op, i;

	system("cls");

	do
	{
		printf("----- Adicionar Registro ----\n\n");

		getchar();

		printf("Digite o nome do carro: ");
		fgets(aux.nomeCarro, 50, stdin);

		for (i = 0; i < strlen(aux.nomeCarro); i++)
			if (aux.nomeCarro[i] == '\n')
				aux.nomeCarro[i] = '\0';

		printf("Digite a montadora: ");
		fgets(aux.montadora, 50, stdin);

		for (i = 0; i < strlen(aux.montadora); i++)
			if (aux.montadora[i] == '\n')
				aux.montadora[i] = '\0';

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
	int i, sub;

	if (novo.chave != -1)
	{
		(*qtd)++;
		sub = 1;
	}
	else
		sub = 0;
		
	fwrite(qtd, sizeof(int), 1, arquivo);

	for (i = 0; i < ((*qtd) - sub); i++)
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
		if (cadastros[posicao].peso < 1000) printf("0");
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

void impressoes(no* cadastros, listaDenso* lista, int qtd, int pri, int *tabelaInversaAlfabeto, char montadoras[5][50], int pri_cat[], int catNome[])
{
	int i, j, po;
	int posicao;
	menuImpressao opcao;

	system("cls");

	printf("\n----- Menu Impressao -----\n\n");
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
		
		printf(" Chave  Nome Carro      Montadora       Consumo  Peso\n\n");

		posicao = pri;
		i = qtd;

		while (i > 0)
		{
				printf(" %i   ", cadastros[posicao].chave);
				printf("%s ", cadastros[posicao].nomeCarro);
				for (j = 15 - strlen(cadastros[posicao].nomeCarro); j > 0; j--) printf(" ");
				printf("%s ", cadastros[posicao].montadora);
				for (j = 15 - strlen(cadastros[posicao].montadora); j > 0; j--) printf(" ");
				if (cadastros[posicao].consumo < 10) printf("0");
				printf("%.2f ", cadastros[posicao].consumo);
				printf("   %i\n", cadastros[posicao].peso);
				
				posicao = tabelaInversaAlfabeto[posicao];
				i--;
		}

		break;
	case categoriaChaveOrdenado:
		break;
	case categoriaNomeOrdenado:

		for (i = 0; i < 5; i++)
		{
			printf("\n\n --------------  %s  --------------\n\n", montadoras[i]);
			   printf(" Chave  Nome Carro      Consumo  Peso\n\n");

			posicao = pri_cat[i];

			do
			{
				printf(" %i   ", cadastros[posicao].chave);
				printf("%s ", cadastros[posicao].nomeCarro);
				for (j = 15 - strlen(cadastros[posicao].nomeCarro); j > 0; j--) printf(" ");
				if (cadastros[posicao].consumo < 10) printf("0");
				printf("%.2f ", cadastros[posicao].consumo);
				printf("   %i\n", cadastros[posicao].peso);
				posicao = catNome[posicao];

			} while (posicao != -1);

		}

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

no* eliminarRegistro(no* cadastros, listaDenso* indice, int *qtd)
{
	int posicao, codigo, j, i;
	no aux;
	aux.chave = -1;
	system("cls");

	printf("----- Eliminar Registro -----\n\n");
	printf("Digite o codigo que deseja do registro a eliminar: ");
	scanf("%i", &codigo);

	posicao = buscaBinaria(indice, *qtd, codigo);

	printf(" Chave  Nome Carro      Montadora       Consumo  Peso\n\n");

	printf(" %i   ", cadastros[posicao].chave);
	printf("%s ", cadastros[posicao].nomeCarro);
	for (j = 15 - strlen(cadastros[posicao].nomeCarro); j > 0; j--) printf(" ");
	printf("%s ", cadastros[posicao].montadora);
	for (j = 15 - strlen(cadastros[posicao].montadora); j > 0; j--) printf(" ");
	if (cadastros[posicao].consumo < 10) printf("0");
	printf("%.2f ", cadastros[posicao].consumo);
	printf("   %i\n", cadastros[posicao].peso);

	printf("\nTem certeza que deseja excluir esse registro?");
	printf("\n 1. Sim;\n 2. Nao;\n\nOpcao: ");
	scanf("%i", &codigo);

	if (codigo == 1)
	{
		for (i = posicao; i < *qtd; i++)
			cadastros[i] = cadastros[i + 1];

		(*qtd)--;

		return(salvarNovoArquivo(aux, cadastros, qtd));

	}
	return (cadastros);

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

void bubbleString(tabInv lista[], int tam)
{
	int i, j;
	tabInv aux;
	int teste = 1;

	for (i = 0; i < tam - 1 && teste; i++)
	{
		teste = 0;

		for (j = 0; j < tam - i - 1; j++)
		{
			if (strcmp(strlwr(lista[j].nome), strlwr(lista[j + 1].nome)) > 0)
			{
				teste = 1;
				strcpy(aux.nome, lista[j].nome);
				aux.posicao = lista[j].posicao;

				strcpy(lista[j].nome, lista[j + 1].nome);
				lista[j].posicao = lista[j + 1].posicao;

				strcpy(lista[j + 1].nome, aux.nome);
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

int* tabelaInversa(int *ordem, int *pri, no* cadastros, int qtd)
{
	int i;
	tabInv *aux;

	ordem = (int*)malloc(qtd * sizeof(int));
	aux = (tabInv*)malloc(qtd * sizeof(tabInv));

	for (i = 0; i < qtd; i++)
	{
		strcpy(aux[i].nome, cadastros[i].nomeCarro);
		aux[i].posicao = i;
	}

	bubbleString(aux, qtd);
	
	for (i = 0; i < qtd; i++)
	{
		if (i == 0)
			*pri = aux[i].posicao;
		ordem[aux[i].posicao] = aux[i + 1].posicao;
	}

	return (ordem);
}

void catChaveOrd(no* cadastros, int pri_cat[], int *cadas, int qtd, char montadoras[5][50])
{
	int count[5] = { 0 };
	int i, j;
	int countMont[5] = { 0 };

	for (i = 0; i < 100; i++)
		cadas[i] = -1;


	tabInv *mont1, *mont2, *mont3, *mont4, *mont5;

	for (i = 0; i < qtd; i++)
	{
		     if (strcmp(cadastros[i].montadora, montadoras[0]) == 0)
			count[0]++;
		else if (strcmp(cadastros[i].montadora, montadoras[1]) == 0)
			count[1]++;
		else if (strcmp(cadastros[i].montadora, montadoras[2]) == 0)
			count[2]++;
		else if (strcmp(cadastros[i].montadora, montadoras[3]) == 0)
			count[3]++;
		else if (strcmp(cadastros[i].montadora, montadoras[4]) == 0)
			count[4]++;
	}

	mont1 = (tabInv*)malloc(count[0] * sizeof(tabInv));
	mont2 = (tabInv*)malloc(count[1] * sizeof(tabInv));
	mont3 = (tabInv*)malloc(count[2] * sizeof(tabInv));
	mont4 = (tabInv*)malloc(count[3] * sizeof(tabInv));
	mont5 = (tabInv*)malloc(count[4] * sizeof(tabInv));

	for (i = 0; i < qtd; i++)
	{
		if (strcmp(cadastros[i].montadora, montadoras[0]) == 0) //VOLKSWAGEN
		{
			strcpy(mont1[countMont[0]].nome, cadastros[i].nomeCarro);
			mont1[countMont[0]].posicao = i;
			countMont[0]++;
		}
		else if (strcmp(cadastros[i].montadora, montadoras[1]) == 0) //CHEVROLET
		{
			strcpy(mont2[countMont[1]].nome, cadastros[i].nomeCarro);
			mont2[countMont[1]].posicao = i;
			countMont[1]++;
		}
		else if (strcmp(cadastros[i].montadora, montadoras[2]) == 0) //FORD
		{
			strcpy(mont3[countMont[2]].nome, cadastros[i].nomeCarro);
			mont3[countMont[2]].posicao = i;
			countMont[2]++;
		}
		else if (strcmp(cadastros[i].montadora, montadoras[3]) == 0) //FIAT
		{
			strcpy(mont4[countMont[3]].nome, cadastros[i].nomeCarro);
			mont4[countMont[3]].posicao = i;
			countMont[3]++;
		}
		else if (strcmp(cadastros[i].montadora, montadoras[4]) == 0) //HYUNDAI
		{
			strcpy(mont5[countMont[4]].nome, cadastros[i].nomeCarro);
			mont5[countMont[4]].posicao = i;
			countMont[4]++;
		}
	}

	bubbleString(mont1, countMont[0]);
	bubbleString(mont2, countMont[1]);
	bubbleString(mont3, countMont[2]);
	bubbleString(mont4, countMont[3]);
	bubbleString(mont5, countMont[4]);

	pri_cat[0] = mont1[0].posicao;
	pri_cat[1] = mont2[0].posicao;
	pri_cat[2] = mont3[0].posicao;
	pri_cat[3] = mont4[0].posicao;
	pri_cat[4] = mont5[0].posicao;

	for(i = 0; i < countMont[0]; i++)
		cadas[mont1[i].posicao] = mont1[i + 1].posicao;
	
	cadas[mont1[i - 1].posicao] = (-1);

	for (i = 0; i < countMont[1]; i++)
		cadas[mont2[i].posicao] = mont2[i + 1].posicao;
	
	cadas[mont2[i - 1].posicao] = (-1);

	for (i = 0; i < countMont[2]; i++)
		cadas[mont3[i].posicao] = mont3[i + 1].posicao;
	
	cadas[mont3[i - 1].posicao] = (-1);

	for (i = 0; i < countMont[3]; i++)
		cadas[mont4[i].posicao] = mont4[i + 1].posicao;
	
	cadas[mont4[i - 1].posicao] = (-1);

	for (i = 0; i < countMont[4]; i++)
		cadas[mont5[i].posicao] = mont5[i + 1].posicao;

	cadas[mont5[i - 1].posicao] = (-1);

	free(mont1);
	free(mont2);
	free(mont3);
	free(mont4);
	free(mont5);
}
