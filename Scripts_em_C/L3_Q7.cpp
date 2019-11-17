/*Codifique um programa que manipule um arquivo contendo registros descritos pelos seguintes campos:
código venda, nome venda, valor da venda e mês. A manipulação do arquivo em questão e feita
através da execução das operações disponibilizadas pelo seguinte menu:*/
//DONE: Criar o arquivo de dados;
//DONE: Incluir um determinado registro no arquivo;
//DONE: Excluir um determinado vendedor no arquivo;
//DONE: Alterar o valor de uma venda no arquivo;
//DONE: Imprimir os registros na saída padrão;
//DONE: Excluir o arquivo de dados;
//DONE: Finalizar o programa.
//DONE: Codigo Unico
//DONE: Ordenar por codigo
/*Os registros devem estar ordenados no arquivo, de forma crescente, de acordo com as informações
contidas nos campos código venda e mês. Não deve existir mais de um registro no arquivo com
mesmos valores nos campos código venda e mês.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define arquivoBin "vendas.bin"

void Sair ()
{
	printf("Houve um erro no processo!");
	exit(1);
}

void CriarArquivo()
{
	FILE *fp = fopen(arquivoBin, "wb");
	if (fp == NULL) Sair();
	else printf("Arquivo %s criado para armazenar os dados!\n", arquivoBin);
}

void DeletarArquivo() 
{
	if (remove(arquivoBin) == 0) printf("Arquivo deletado com sucesso!\n");
	else Sair();
}

struct Venda 
{
	int codigo, mes;
	char nome[30];
	float valor;
};

struct ListaDeVendas
{
	int size;
	Venda vendas[1000];	
};

ListaDeVendas listaDeVendas;

int ExisteCodigoAndMes (int codigo, int mes) 
{
	for (int i = 0; i < listaDeVendas.size; i++) 
		if(listaDeVendas.vendas[i].codigo == codigo && listaDeVendas.vendas[i].mes == mes) return i;
	
	return -1;
}

void VendaCpy (Venda *venda1, Venda *venda2)
{
	(*venda1).codigo = (*venda2).codigo;
	strcpy((*venda1).nome, (*venda2).nome);
	(*venda1).mes = (*venda2).mes;
	(*venda1).valor = (*venda2).valor; 
};

void ListaDeVendasAdd (Venda venda)
{
	Venda tempVenda;
	if (listaDeVendas.size == 1000) Sair();
	
	listaDeVendas.size++;
	for (int i = 0; i < listaDeVendas.size; i++)
	{
		if (listaDeVendas.vendas[i].codigo == 0 || listaDeVendas.vendas[i].codigo > venda.codigo || 
			(listaDeVendas.vendas[i].codigo == venda.codigo && listaDeVendas.vendas[i].mes > venda.mes))
		{
			VendaCpy(&tempVenda, &listaDeVendas.vendas[i]);//tempVenda = listaDeVendas.vendas[i];
			VendaCpy(&listaDeVendas.vendas[i], &venda);//listaDeVendas.vendas[i] = venda;
			VendaCpy(&venda, &tempVenda);//venda = tempVenda;		
		}
	}
}

void LoadDataFromFile () 
{
	listaDeVendas.size = 0;
	Venda venda;
	FILE *fp = fopen(arquivoBin, "rb");
	if (fp == NULL) return;
	
	printf("Carregando dados do arquivo...\n");
	while (1) 
	{
		fscanf(fp, "%s | %d | %d | %f", &venda.nome, &venda.codigo, &venda.mes, &venda.valor);
		if (feof(fp)) break;
		ListaDeVendasAdd(venda);
		
		printf("\t%s | %d | %d | %f\n", venda.nome, venda.codigo, venda.mes, venda.valor);//Debug
	}
	
	if (listaDeVendas.size == 0) printf("Ainda nao ha nenhuma informacao registrada!\n");
	else printf("%d informacoes de venda carregadas!\n", listaDeVendas.size);

	fclose(fp);
}

void SaveDataToFile () 
{
	FILE *fp = fopen(arquivoBin, "wb");
	if (fp == NULL) Sair();
	
	for (int i = 0; i < listaDeVendas.size; i++)
		fprintf(fp, "%s | %d | %d | %f\n", 
				listaDeVendas.vendas[i].nome, listaDeVendas.vendas[i].codigo, listaDeVendas.vendas[i].mes, listaDeVendas.vendas[i].valor);
	
	fclose(fp);
}

void ExibirVendas () 
{
	printf("\nLista de vendas:\n");
	
	for (int i = 0; i < listaDeVendas.size; i++) 
		printf("Codigo: %d\nNome do vendedor: %s\nMes da venda: %d\nValor da Venda: %f\n\n", 
				listaDeVendas.vendas[i].codigo, listaDeVendas.vendas[i].nome, listaDeVendas.vendas[i].mes, listaDeVendas.vendas[i].valor);
}

void InserirVenda ()
{
	Venda novaVenda;
	
	if (listaDeVendas.size == 1000) 
	{
		printf("Limite de dados atingido. Favor excluir alguma entrada\n");
		return;
	}
	
	do
	{
		fflush(stdin);
		printf("\nDigite um codigo inteiro maior que zero: ");
		scanf("%d", &novaVenda.codigo);	
	} 
	while (novaVenda.codigo <= 0);
	
	fflush(stdin);
	printf("Digite o nome do venda: ");
	scanf("%s", &novaVenda.nome);
	
	fflush(stdin);
	printf("Digite o mes da venda: ");
	scanf("%d", &novaVenda.mes);
	
	fflush(stdin);
	printf("Digite o valor da venda: ");
	scanf("%f", &novaVenda.valor);
	
	if (ExisteCodigoAndMes(novaVenda.codigo, novaVenda.mes) >= 0)
	{
		printf("Nao foi possivel adicionar informacao, ja existe codigo e mes. Tente alterar o valor pelo menu");
	}
	else 
	{
		ListaDeVendasAdd(novaVenda);
		ExibirVendas();
		SaveDataToFile();
		printf("Venda incluida com sucesso!!\n");
	}
}

void ExcluirVendedor()
{
	Venda tempVendas[listaDeVendas.size], nullVenda; 
	int count = 0, i;
	char nome[30];
	
	fflush(stdin);
	printf("Digite o nome do vendedor que deseja excluir: ");
	scanf("%s", &nome);
	
	for (i = 0; i < listaDeVendas.size; i++) 
	{
		if (strcmp(listaDeVendas.vendas[i].nome, nome)) VendaCpy(&listaDeVendas.vendas[count++], &listaDeVendas.vendas[i]);
	}
	for (i = count; i < listaDeVendas.size; i++) VendaCpy(&listaDeVendas.vendas[i], &nullVenda); //Resetar as vendas que ficaram de fora
	
	listaDeVendas.size = count;
	SaveDataToFile();
	printf("Todas as entradas do vendedor %s foram excluidas!\n", nome);		
}

void AlterarValorVenda ()
{
	int mes, codigo, vendaId;
	float novoValor;
	
	fflush(stdin);
	printf("\nDigite o codigo do vendedor: ");
	scanf("%d", &codigo);	
 	
	fflush(stdin);
	printf("Digite o mes da venda: ");
	scanf("%d", &mes);
		
	vendaId = ExisteCodigoAndMes(codigo, mes);
	if (vendaId >= 0)
	{
		fflush(stdin);
		printf("Digite o valor da venda: ");
		scanf("%f", &novoValor);
	
		listaDeVendas.vendas[vendaId].valor = novoValor;
		SaveDataToFile();
		printf("Valor alterado com sucesso!!\n");
	}
	else 
	{		
		printf("Venda nao encontrada, tente digitar o codigo e mes novamente!\n");
	}
}

int main () 
{
	int solicitacao;
	LoadDataFromFile();
	
	while (1) 
	{
		fflush(stdin);
		printf("\n-------------- Menu -------------------\n0 - Finalizar o programa\n");
		printf("1 - Criar o arquivo de dados\n2 - Excluir o arquivo de dados\n3 - Visualizar vendas\n4 - Inserir nova venda\n5 - Alterar o valor de uma venda\n");
		printf("6 - Excluir todas os dados de um vendedor\nSelecione a opcao desejada: ");
		scanf("%d", &solicitacao);
		
		switch (solicitacao) 
		{
			case (0):
				return 0;
			case (1):
				CriarArquivo();
				break;
			case (2):
				DeletarArquivo();
				break;
			case (3):
				ExibirVendas();
				break;
			case (4):
				InserirVenda();
				break;
			case (5):
				AlterarValorVenda();
				break;
			case (6):
				ExcluirVendedor();
				break;
			default:
				printf("Opcao nao eh valida, tente novamente\n");
		}
	}
}
