#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum menu { consultarRegistros = 1, alterarRegistro, apagarRegistro, inserirRegistro, impressao };

void adicionarRegistros(FILE*);



typedef struct {
	int chave;
	char nomeCarro[50];
	char montadora[50];
	float consumo;
	int peso;

} no;

void main()
{
	menu opcao;
	FILE* arquivo;

	arquivo = fopen("carros.dat", "a+b");

	printf("-------------------- Menu --------------------\n");
	printf(" 1. Consultar um registro;\n");
	printf(" 2. ALterar/Atualizar campo(s) de um registro;\n");
	printf(" 3. Eliminar um registro do arquivo;\n");
	printf(" 4. Inserir um registro no arquivo;\n");
	printf(" 5. Impressao;\n\n");
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
		adicionarRegistros(arquivo);
		break;
	case impressao:
		break;
	default:
		break;
	}



	system("pause");
}

void adicionarRegistros(FILE* arq)
{
	no cadastro;

	arq = fopen("carros.dat", "a+b");

	system("cls");

	do 
	{
		printf("----- Adicionar Registro ----\n");
		
		printf("Digite o nome do carro: ");
		fgets(cadastro.nomeCarro, 50, stdin);
		
		printf("Digite a montadora: ");
		fgets(cadastro.montadora, 50, stdin);

		printf("Digite o consumo do veiculo: ");
		scanf("%f", &cadastro.consumo);

		printf("Digite o peso do veiculo: ");
		scanf("%i", &cadastro.peso);

	} while ();



}
