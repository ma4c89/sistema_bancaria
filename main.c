#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
   float valor;
   int dataRecebimento_dia, dataRecebimento_mes, dataRecebimento_ano;
   int dataRecebimentoEsperado_dia, dataRecebimentoEsperado_mes, dataRecebimentoEsperado_ano;
   char descricao[50];
   int numero_conta_receita;
   char tipoReceita[50];
} receita;

typedef struct lstItem_receita{
   receita dado;
   struct lstItem_receita *next_rec;
} lstReceita;

typedef struct{
   float valor;
   int dataPagamento_dia, dataPagamento_mes, dataPagamento_ano;
   int dataPagamentoEsperado_dia, dataPagamentoEsperado_mes, dataPagamentoEsperado_ano;
   char tipoDespesa[50];
   int numero_conta_despesa;
} despesa;

typedef struct lstItem_despesa{
   despesa dado;
   struct lstItem_despesa *next_des;
} lstDespesa;

typedef struct{
	int num_conta;
	float saldo;
	char tipoConta[50];
	char instituicaoFinanceira[50];
} conta;

typedef struct lstItem{
   conta dado;
   struct lstItem *next;
   struct lstItem_receita *next_rec;
   struct lstItem_despesa *next_des;
} lstConta;

/*---------------------------------------------------------------FUNÇÕES---------------------------------------------------------------*/

//Contas
lstConta *cria_lista_conta();
void print_contas(lstConta *item);
void print_saldo(lstConta *item);
lstConta *cadastrar_conta(lstConta *lista);
lstConta *excluir_conta(lstConta *lista);
lstConta *transferir(lstConta *item);
lstConta *editar_conta(lstConta *item);
lstConta *busca(lstConta *item, conta dado);
conta *criar_conta(int i);

//Receitas
lstConta *cadastrar_receita(lstConta *mov_conta);
void print_receita(lstReceita *item_rec);
void print_total_receita(lstReceita *item_rec);
void print_item_receita(lstReceita *item_rec);
void print_receita_por_tipo(lstReceita *item_rec);
lstConta *editar_receita(lstConta *mov_conta, lstReceita *item_rec);
lstConta *excluir_receita(lstConta *mov_conta, lstReceita *item_posterior);
lstReceita *busca_receita(lstReceita *item_rec);
receita *criar_receita(int i);
int movimentar_receitas(lstConta *mov_conta);

//Despesa
lstConta *cadastrar_despesa(lstConta *mov_conta);
void print_despesa(lstDespesa *item_des);
void print_total_despesa(lstDespesa *item_des);
void print_item_despesa(lstDespesa *item_des);
void print_despesa_por_tipo(lstDespesa *item_des);
lstConta *editar_despesa(lstConta *mov_conta, lstDespesa *item_des);
lstConta *excluir_despesa(lstConta *mov_conta, lstDespesa *item_posterior);
lstDespesa *busca_despesa(lstDespesa *item_des);
despesa *criar_despesa(int i);
int movimentar_despesas(lstConta *mov_conta);



/*---------------------------------------------------------------CONTAS---------------------------------------------------------------*/

lstConta *cria_lista_conta(){
   return NULL;
}

void print_contas(lstConta *item){

   while(item!=NULL){
      printf(">>Conta: %d | Saldo: %.2f | Tipo de conta: %s | Instituicao Financeira: %s\n", item->dado.num_conta, item->dado.saldo, item->dado.tipoConta, item->dado.instituicaoFinanceira);
      item = item->next;
   }
}

void print_saldo(lstConta *item){
	
	float soma=0;
	
	while(item!=NULL){
      soma = soma + item->dado.saldo;
      item = item->next;
   }
   
   printf("Saldo Total = %.2f", soma);
}

lstConta *cadastrar_conta(lstConta *lista){ 
	
	printf("Insira os dados da nova conta:\n");
	conta *dado_conta = criar_conta(0);
	
   lstConta *novo = malloc(sizeof(lstConta));
   (*novo).dado = *dado_conta;
   (*novo).next = lista;
   (*novo).next_rec = NULL;
   (*novo).next_des = NULL;
   lista = novo;  
   
	return lista;
}

lstConta *excluir_conta(lstConta *lista){
	
	printf("Insira os dados da conta que deseja excluir:\n");
	conta *dado_conta = criar_conta(1);
	
	lstConta *item_posterior=lista;
	lstConta *item_anterior=malloc(sizeof(lstConta));
	lstConta *indesejado=malloc(sizeof(lstConta));
	int i=0;
	
	while(item_posterior!=NULL)
	{
		if(item_posterior->dado.num_conta!=dado_conta->num_conta || (strcmp(item_posterior->dado.tipoConta,dado_conta->tipoConta)!=0) || (strcmp(item_posterior->dado.instituicaoFinanceira,dado_conta->instituicaoFinanceira)!=0))
		{
			item_anterior=item_posterior;
			item_posterior=item_posterior->next;
			i++;
		}
		
		else
		{
			indesejado=item_posterior;
			item_posterior=item_posterior->next;
			
			lstReceita *lista_rec = malloc(sizeof(lstReceita));
			(*lista_rec).next_rec = indesejado->next_rec;
			lista_rec=lista_rec->next_rec;
				
			while(lista_rec!=NULL)
			{
				lstReceita *aux_rec=lista_rec;
				lista_rec=lista_rec->next_rec;
				free(aux_rec);
				aux_rec=NULL;
			}
			
			lstDespesa *lista_des = malloc(sizeof(lstDespesa));
			(*lista_des).next_des = indesejado->next_des;
			lista_des=lista_des->next_des;
			
			while(lista_des!=NULL)
			{
				lstDespesa *aux_des=lista_des;
				lista_des=lista_des->next_des;
				free(aux_des);
				aux_des=NULL;
			}
			
			printf("--  Conta exluida com sucesso!  --\n\n");
			
			if(i==0)
			{
				
				free(indesejado);
				indesejado=NULL;
			
				return item_posterior;
			}
			
			else
			{
				item_anterior->next=item_posterior;
				
				free(indesejado);
				indesejado=NULL;
				
				return lista;
			}
		}
	}
	
	printf("--  Nao foi possivel localizar a conta!  --\n\n");
	return lista;
}


lstConta *transferir(lstConta *item_origem){
	
	lstConta *item_destino = item_origem;
	
	printf("Insira os dados da conta de origem:\n");
    conta *dado_origem = criar_conta(1);
    
    item_origem = busca(item_origem, *dado_origem);
    
    if(item_origem!=NULL)
    {
	    printf("Insira os dados da conta de destino:\n");
	    conta *dado_destino = criar_conta(1);
		
		item_destino = busca(item_destino, *dado_destino);
		
		if(item_destino!=NULL)
		{
			item_destino->dado.saldo=item_destino->dado.saldo+item_origem->dado.saldo;
			item_origem->dado.saldo=0;
			
			printf("--  Transferencia concluida com sucesso! --\n\n");
		}
	}
	
	return 0;
}


lstConta *editar_conta(lstConta *item){
	
	printf("Insira os dados da conta que voce deseja editar:\n");
	conta *antiga = criar_conta(1);
	
	item = busca(item, *antiga);
	
	if(item!=NULL)
	{
		
		printf("Insira os dados atualizados da conta:\n");
		conta *atualizada = criar_conta(0);
		
		item->dado=(*atualizada);
		
		printf("--  Conta atualizada com sucesso!  --\n\n");
	}
	return 0;
}

lstConta *busca(lstConta *item, conta dado){
	
	while(item!=NULL)
	{
		if(item->dado.num_conta!=dado.num_conta || (strcmp(item->dado.tipoConta,dado.tipoConta)!=0) || (strcmp(item->dado.instituicaoFinanceira,dado.instituicaoFinanceira)!=0))
		{
			item=item->next;
		}
		
		else
		{
			return item;
		}
	}
	
	printf("--  Nao foi possivel localizar a conta!  --\n\n");
	return NULL;
}

conta *criar_conta(int i){
	
   conta *novo = malloc(sizeof(conta));
   
   printf("Numero da conta: ");
   scanf("%d",&((*novo).num_conta));
   
   if(i==0)
   {
	   printf("Saldo em conta: ");
	   scanf("%f",&((*novo).saldo));
	}   
   
    while((getchar())!='\n');
   
   printf("Tipo de conta: ");
   gets((*novo).tipoConta);
   
   printf("Instituicao Financeira: ");
   gets((*novo).instituicaoFinanceira);
   
   while((getchar())!='\n');
   
   printf("\n");
   
   return novo;
}

/*---------------------------------------------------------------RECEITAS---------------------------------------------------------------*/

lstConta *cadastrar_receita(lstConta *mov_conta){
	
	printf("Insira os dados da nova receita:\n");
	receita *dado_receita = criar_receita(0);
	
	dado_receita->numero_conta_receita=mov_conta->dado.num_conta;
	mov_conta->dado.saldo=mov_conta->dado.saldo+dado_receita->valor;
	
   lstReceita *novo = malloc(sizeof(lstReceita));
   (*novo).dado = *(dado_receita);
   (*novo).next_rec = mov_conta->next_rec;
   (*mov_conta).next_rec = novo;
   
	return 0;
}

void print_receita(lstReceita *item_rec){
	
   while(item_rec!=NULL){
      print_item_receita(item_rec);
	  item_rec=item_rec->next_rec;
   }
   
   printf("\n\n");
}

void print_total_receita(lstReceita *item_rec){
	
   float soma=0;
	
	while(item_rec!=NULL){
      soma = soma + item_rec->dado.valor;
      item_rec = item_rec->next_rec;
   }
   
   printf("Total de receitas = %.2f\n\n", soma);
}

void print_item_receita(lstReceita *item_rec){
	
   printf(">>Valor: %.2f | Recebimento: %02d/%02d/%d | Recebimento Esperado: %02d/%02d/%d\n", item_rec->dado.valor, item_rec->dado.dataRecebimento_dia, item_rec->dado.dataRecebimento_mes, item_rec->dado.dataRecebimento_ano, item_rec->dado.dataRecebimentoEsperado_dia, item_rec->dado.dataRecebimentoEsperado_mes, item_rec->dado.dataRecebimentoEsperado_ano);
   printf("  Conta: %d | Descricao: %s | Tipo de receita: %s \n", item_rec->dado.numero_conta_receita, item_rec->dado.descricao, item_rec->dado.tipoReceita);
   
}

void print_receita_por_periodo(lstReceita *item_rec){
	
	printf("Insira a data inicial:\n");
	receita *data_inicial = criar_receita(2);
	printf("\n\n");
	
	printf("Insira a data final:\n");
	receita *data_final = criar_receita(2);
	printf("\n\n");
	
	while(item_rec!=NULL)
	{
		if((data_inicial->dataRecebimento_ano)<=(item_rec->dado.dataRecebimento_ano) && (data_final->dataRecebimento_ano)>=(item_rec->dado.dataRecebimento_ano))
		{
			if((data_inicial->dataRecebimento_mes)<=(item_rec->dado.dataRecebimento_mes) && (data_final->dataRecebimento_mes)>=(item_rec->dado.dataRecebimento_mes))
			{
				if((data_inicial->dataRecebimento_dia)<=(item_rec->dado.dataRecebimento_dia) && (data_final->dataRecebimento_dia)>=(item_rec->dado.dataRecebimento_dia))
				{
					print_item_receita(item_rec);
				}
			}
		}
		
		item_rec=item_rec->next_rec;
	}
	
	printf("\n\n");	
}

void print_receita_por_tipo(lstReceita *item_rec){
	
	char tipo[50];
	
	printf("Insira o tipo de receita:\n");
	
	while((getchar())!='\n');
	
	printf("Tipo de receita: ");
	gets(tipo);
	
	while((getchar())!='\n');
	
	
	while(item_rec!=NULL)
	{
		if(strcmp(item_rec->dado.tipoReceita,tipo)==0)
		{
			print_item_receita(item_rec);
		}
		
		item_rec=item_rec->next_rec;
	}
	
	printf("\n\n");	
}

lstConta *editar_receita(lstConta *mov_conta, lstReceita *item_rec){
	
	printf("Insira os dados da receita que deseja editar:\n");
	item_rec=busca_receita(item_rec);
	
	if(item_rec!=NULL)
	{
		mov_conta->dado.saldo=mov_conta->dado.saldo-item_rec->dado.valor;
		
		printf("Insira os dados atualizados da receita:\n");
		receita *atualizada = criar_receita(0);
		
		atualizada->numero_conta_receita=mov_conta->dado.num_conta;
		mov_conta->dado.saldo=mov_conta->dado.saldo+atualizada->valor;
		
		item_rec->dado= (*atualizada);
		
		printf("--  Receita atualizada com sucesso!  --\n\n");
	}
	
	return 0;
}

lstConta *excluir_receita(lstConta *mov_conta, lstReceita *item_posterior){
	
	printf("Insira os dados da receita que deseja excluir:\n");
	
	receita *dado_receita = criar_receita(1);
	
	lstReceita *item_anterior=malloc(sizeof(lstReceita));
	lstReceita *indesejado=malloc(sizeof(lstReceita));
	
	int i=0;
	
	while(item_posterior!=NULL)
	{
		if(item_posterior->dado.valor!=dado_receita->valor || item_posterior->dado.dataRecebimento_dia!=dado_receita->dataRecebimento_dia || item_posterior->dado.dataRecebimento_mes!=dado_receita->dataRecebimento_mes || item_posterior->dado.dataRecebimento_ano!=dado_receita->dataRecebimento_ano || (strcmp(item_posterior->dado.tipoReceita,dado_receita->tipoReceita)!=0))
		{
			item_anterior=item_posterior;
			item_posterior=item_posterior->next_rec;
			i++;
		}
		
		else
		{
			
			indesejado=item_posterior;
			item_posterior=item_posterior->next_rec;
			
			mov_conta->dado.saldo=mov_conta->dado.saldo-dado_receita->valor;
			
			printf("--  Receita exluida com sucesso!  --\n\n");
			
			if(i==0)
			{
				free(indesejado);
				indesejado=NULL;
				
				(*mov_conta).next_rec = item_posterior;
				
				return 0;
			}
			
			else
			{
				item_anterior->next_rec=item_posterior;
				
				free(indesejado);
				indesejado=NULL;
				
				return 0;
			}
		}
	}
	
	printf("--  Nao foi possivel localizar a receita!  --\n\n");
	return 0;
}

lstReceita *busca_receita(lstReceita *item_rec){
	
	receita *dado_receita = criar_receita(1);
	
	while(item_rec!=NULL)
	{
		if(item_rec->dado.valor!=dado_receita->valor || item_rec->dado.dataRecebimento_dia!=dado_receita->dataRecebimento_dia || item_rec->dado.dataRecebimento_mes!=dado_receita->dataRecebimento_mes || item_rec->dado.dataRecebimento_ano!=dado_receita->dataRecebimento_ano || (strcmp(item_rec->dado.descricao,dado_receita->descricao)!=0) || (strcmp(item_rec->dado.tipoReceita,dado_receita->tipoReceita)!=0))
		{
			item_rec=item_rec->next_rec;
		}
		
		else
		{
			return item_rec;
		}
	}
	
	printf("--  Nao foi possivel localizar a receita!  --\n\n");
	return NULL;
}

receita *criar_receita(int i){
	
	receita *novo = malloc(sizeof(receita));
	
	if(i!=2)
	{
		printf("Valor: ");
		scanf("%f",&((*novo).valor));
	}
	
	printf("Dia de recebimento: ");
	scanf("%d",&((*novo).dataRecebimento_dia));
    
    printf("Mes de recebimento: ");
	scanf("%d",&((*novo).dataRecebimento_mes));
	
	printf("Ano de recebimento (escreva com 4 algarismos): ");
	scanf("%d",&((*novo).dataRecebimento_ano));
	
	if(i==0)
	{
		printf("Dia de recebimento esperado: ");
		scanf("%d",&((*novo).dataRecebimentoEsperado_dia));
	    
	    printf("Mes de recebimento esperado: ");
		scanf("%d",&((*novo).dataRecebimentoEsperado_mes));
		
		printf("Ano de recebimento esperado (escreva com 4 algarismos): ");
		scanf("%d",&((*novo).dataRecebimentoEsperado_ano));
	    
	}
	
	if(i!=2)
	{
		while((getchar())!='\n');
		    
		printf("Descricao: ");
		gets((*novo).descricao);	
		
		printf("Tipo de receita: ");
		gets((*novo).tipoReceita);
		
		while((getchar())!='\n');
	}
	
	return novo;
}

int movimentar_receitas(lstConta *mov_conta){
	
	int opcao_1=0, opcao_2=0;
	
	while(opcao_1!=9){   
	
	lstReceita *lista_rec = malloc(sizeof(lstReceita));
	(*lista_rec).next_rec = mov_conta->next_rec;
	lista_rec=lista_rec->next_rec;
		
		printf("-- O que deseja fazer --\n");
	    printf("1 - Cadastrar receita\n"); 
	    printf("2 - Editar receita\n");
	    printf("3 - Remover receita\n");
	    printf("4 - Listar receitas\n");
	    printf("5 - Listar total de receitas\n");
	    printf("9 - Voltar\n");
	    printf("\nDigite a opcao desejada: ");
	    scanf("%d",&opcao_1);
	    printf("Opcao selecionada %d\n\n", opcao_1);
  
	    switch(opcao_1)
	    {
	    	case 1:
	    		cadastrar_receita(mov_conta);
	    	break;
	    	
	    	case 2:
	    		editar_receita(mov_conta, lista_rec);
	    	break;
	    	
	    	case 3:
	    		excluir_receita(mov_conta, lista_rec);
	    	break;
	    	
	    	case 4:
	    		
	    		opcao_2=0;
        		
	        	while(opcao_2!=9){
	        		
	        		printf("-- O que deseja fazer --\n");
				    printf("1 - Todas as receitas\n"); 
				    printf("2 - Listar receitas por periodo\n");
				    printf("3 - Listar receitas por tipo de receita\n");
				    printf("9 - Voltar\n");
				    printf("\nDigite a opcao desejada: ");
				    scanf("%d",&opcao_2);
				    printf("Opcao selecionada %d\n\n", opcao_2);
				    
				    switch(opcao_2)
				    {
				    	
				    	case 1:				    				
				    		print_receita(lista_rec);
				    	break;
				    	
				    	case 2:
				    		print_receita_por_periodo(lista_rec);
				    	break;
				    	
				    	case 3:
				    		print_receita_por_tipo(lista_rec);
				    	break;
				    	
				    	case 9:
				    	break;
				    	
				    	default:
				        	printf("Opcao invalida!\n\n");
				        break;
					}
				}
				
	    	break;
	    	
	    	case 5:
	    		print_total_receita(lista_rec);
	    	break;
	    	
	    	case 9:
			break;
			
			default:
				printf("Opcao invalida!\n\n");
			break;
		}
	}
	
	return 0;
}

/*---------------------------------------------------------------DESPESAS---------------------------------------------------------------*/

lstConta *cadastrar_despesa(lstConta *mov_conta){
	
	printf("Insira os dados da nova despesa:\n");
	despesa *dado_despesa = criar_despesa(0);
	
	dado_despesa->numero_conta_despesa=mov_conta->dado.num_conta;
	mov_conta->dado.saldo=mov_conta->dado.saldo-dado_despesa->valor;
	
   lstDespesa *novo = malloc(sizeof(lstDespesa));
   (*novo).dado = *(dado_despesa);
   (*novo).next_des = mov_conta->next_des;
   (*mov_conta).next_des = novo;
   
	return 0;
}

void print_despesa(lstDespesa *item_des){
	
   while(item_des!=NULL){
      print_item_despesa(item_des);
      item_des=item_des->next_des;
   }
   
   printf("\n\n");
}

void print_total_despesa(lstDespesa *item_des){
	
   float soma=0;
	
	while(item_des!=NULL){
      soma = soma + item_des->dado.valor;
      item_des = item_des->next_des;
   }
   
   printf("Total de despesas = %.2f\n\n", soma);
}

void print_item_despesa(lstDespesa *item_des){
	
   printf(">>Valor: %.2f | Pagamento: %02d/%02d/%d | Pagamento Esperado: %02d/%02d/%d\n", item_des->dado.valor, item_des->dado.dataPagamento_dia, item_des->dado.dataPagamento_mes, item_des->dado.dataPagamento_ano, item_des->dado.dataPagamentoEsperado_dia, item_des->dado.dataPagamentoEsperado_mes, item_des->dado.dataPagamentoEsperado_ano);
   printf("  Conta: %d | Tipo de despesa: %s \n", item_des->dado.numero_conta_despesa, item_des->dado.tipoDespesa);

}

void print_despesa_por_tipo(lstDespesa *item_des){
	
	char tipo[50];
	
	printf("Insira o tipo de despesa:\n");
	
	while((getchar())!='\n');
	
	printf("Tipo de receita: ");
	gets(tipo);
	
	while((getchar())!='\n');
	
	while(item_des!=NULL)
	{
		if(strcmp(item_des->dado.tipoDespesa,tipo)==0)
		{
			print_item_despesa(item_des);
		}
		
		item_des=item_des->next_des;
	}
	
	printf("\n\n");	
}

void print_despesa_por_periodo(lstDespesa *item_des){
	
	printf("Insira a data inicial:\n");
	despesa *data_inicial = criar_despesa(2);
	printf("\n");
	
	printf("Insira a data final:\n");
	despesa *data_final = criar_despesa(2);
	printf("\n");
	
	while(item_des!=NULL)
	{
		if((data_inicial->dataPagamento_ano)<=(item_des->dado.dataPagamento_ano) && (data_final->dataPagamento_ano)>=(item_des->dado.dataPagamento_ano))
		{
			if((data_inicial->dataPagamento_mes)<=(item_des->dado.dataPagamento_mes) && (data_final->dataPagamento_mes)>=(item_des->dado.dataPagamento_mes))
			{
				if((data_inicial->dataPagamento_dia)<=(item_des->dado.dataPagamento_dia) && (data_final->dataPagamento_dia)>=(item_des->dado.dataPagamento_dia))
				{
					print_item_despesa(item_des);
				}
			}
		}
		
		item_des=item_des->next_des;
	}
	
	printf("\n\n");	
}

lstConta *editar_despesa(lstConta *mov_conta, lstDespesa *item_des){
	
	printf("Insira os dados da despesa que deseja editar:\n");
	item_des=busca_despesa(item_des);
	
	if(item_des!=NULL)
	{
		mov_conta->dado.saldo=mov_conta->dado.saldo+item_des->dado.valor;
		
		printf("Insira os dados atualizados da despesa:\n");
		despesa *atualizada = criar_despesa(0);
		
		atualizada->numero_conta_despesa=mov_conta->dado.num_conta;
		mov_conta->dado.saldo=mov_conta->dado.saldo-atualizada->valor;
		
		item_des->dado= (*atualizada);
		
		printf("--  Despesa atualizada com sucesso!  --\n\n");
	}
	
	return 0;
}

lstConta *excluir_despesa(lstConta *mov_conta, lstDespesa *item_posterior){
	
	printf("Insira os dados da despesa que deseja excluir:\n");
	
	despesa *dado_despesa = criar_despesa(1);
	
	lstDespesa *item_anterior=malloc(sizeof(lstDespesa));
	lstDespesa *indesejado=malloc(sizeof(lstDespesa));
	
	int i=0;
	
	while(item_posterior!=NULL)
	{
		if(item_posterior->dado.valor!=dado_despesa->valor || item_posterior->dado.dataPagamento_dia!=dado_despesa->dataPagamento_dia || item_posterior->dado.dataPagamento_mes!=dado_despesa->dataPagamento_mes || item_posterior->dado.dataPagamento_ano!=dado_despesa->dataPagamento_ano || (strcmp(item_posterior->dado.tipoDespesa,dado_despesa->tipoDespesa)!=0))
		{
			item_anterior=item_posterior;
			item_posterior=item_posterior->next_des;
			i++;
		}
		
		else
		{
			
			indesejado=item_posterior;
			item_posterior=item_posterior->next_des;
			
			mov_conta->dado.saldo=mov_conta->dado.saldo+dado_despesa->valor;
			
			printf("--  Despesa exluida com sucesso!  --\n\n");
			
			if(i==0)
			{
				free(indesejado);
				indesejado=NULL;
				
				(*mov_conta).next_des = item_posterior;
				
				return 0;
			}
			
			else
			{
				item_anterior->next_des=item_posterior;
				
				free(indesejado);
				indesejado=NULL;
				
				return 0;
			}
		}
	}
	
	printf("--  Nao foi possivel localizar a despesa!  --\n\n");
	return 0;
}

lstDespesa *busca_despesa(lstDespesa *item_des){
	
	despesa *dado_despesa = criar_despesa(1);
	
	while(item_des!=NULL)
	{
		if(item_des->dado.valor!=dado_despesa->valor || item_des->dado.dataPagamento_dia!=dado_despesa->dataPagamento_dia || item_des->dado.dataPagamento_mes!=dado_despesa->dataPagamento_mes || item_des->dado.dataPagamento_ano!=dado_despesa->dataPagamento_ano || (strcmp(item_des->dado.tipoDespesa,dado_despesa->tipoDespesa)!=0))
		{
			item_des=item_des->next_des;
		}
		
		else
		{
			return item_des;
		}
	}
	
	printf("--  Nao foi possivel localizar a despesa!  --\n\n");
	return NULL;
}

despesa *criar_despesa(int i){
	
	despesa *novo = malloc(sizeof(despesa));
	
	if(i!=2)
	{
		printf("Valor: ");
		scanf("%f",&((*novo).valor));
	}
	
	printf("Dia de pagamento: ");
	scanf("%d",&((*novo).dataPagamento_dia));
	   
	printf("Mes de pagamento: ");
	scanf("%d",&((*novo).dataPagamento_mes));
	
	printf("Ano de pagamento (escreva com 4 algarismos): ");
	scanf("%d",&((*novo).dataPagamento_ano));
	
	if(i==0)
	{
		printf("Dia de pagamento esperado: ");
		scanf("%d",&((*novo).dataPagamentoEsperado_dia));
	    
	    printf("Mes de pagamento esperado: ");
		scanf("%d",&((*novo).dataPagamentoEsperado_mes));
		
		printf("Ano de pagamento esperado (escreva com 4 algarismos): ");
		scanf("%d",&((*novo).dataPagamentoEsperado_ano));
	    
	}
	
	if(i!=2)
	{
		while((getchar())!='\n');
		
		printf("Tipo de despesa: ");
		gets((*novo).tipoDespesa);
		
		while((getchar())!='\n');
	}
	
	printf("\n");
	
	return novo;
}

int movimentar_despesas(lstConta *mov_conta){
	
	int opcao_1=0, opcao_2=0;
	
	while(opcao_1!=9){   
	
	lstDespesa *lista_des = malloc(sizeof(lstDespesa));
	(*lista_des).next_des = mov_conta->next_des;
	lista_des=lista_des->next_des;
		
		printf("-- O que deseja fazer --\n");
	    printf("1 - Cadastrar despesa\n"); 
	    printf("2 - Editar despesa\n");
	    printf("3 - Remover despesa\n");
	    printf("4 - Listar despesas\n");
	    printf("5 - Listar total de despesas\n");
	    printf("9 - Voltar\n");
	    printf("\nDigite a opcao desejada: ");
	    scanf("%d",&opcao_1);
	    printf("Opcao selecionada %d\n\n", opcao_1);
  
	    switch(opcao_1)
	    {
	    	case 1:
	    		cadastrar_despesa(mov_conta);
	    	break;
	    	
	    	case 2:
	    		editar_despesa(mov_conta, lista_des);
	    	break;
	    	
	    	case 3:
	    		excluir_despesa(mov_conta, lista_des);
	    	break;
	    	
	    	case 4:	    		
	    		opcao_2=0;
        		
	        	while(opcao_2!=9){
	        		
	        		printf("-- O que deseja fazer --\n");
				    printf("1 - Todas as despesas\n"); 
				    printf("2 - Listar despesas por periodo\n");
				    printf("3 - Listar despesas por tipo de despesa\n");
				    printf("9 - Voltar\n");
				    printf("\nDigite a opcao desejada: ");
				    scanf("%d",&opcao_2);
				    printf("Opcao selecionada %d\n\n", opcao_2);
				    
				    switch(opcao_2)
				    {
				    	
				    	case 1:				    				
				    		print_despesa(lista_des);
				    	break;
				    	
				    	case 2:
				    		print_despesa_por_periodo(lista_des);
				    	break;
				    	
				    	case 3:
				    		print_despesa_por_tipo(lista_des);
				    	break;
				    	
				    	case 9:
				    	break;
				    	
				    	default:
				        	printf("Opcao invalida!\n\n");
				        break;
					}
				}
				
	    	break;
	    	
	    	case 5:
	    		print_total_despesa(lista_des);
	    	break;
	    	
	    	case 9:
			break;
			
			default:
				printf("Opcao invalida!\n\n");
			break;
		}
	}
	
	return 0;
}


/*---------------------------------------------------------------FUNÇÃO MAIN---------------------------------------------------------------*/

int main(){

   lstConta *lista_conta = cria_lista_conta();
   lstConta *mov_conta;
   
   int opcao_1=0, opcao_2=0;  
   
   conta *dado_conta;

   while(opcao_1!=9){   
      printf("-- O que deseja fazer --\n");
      printf("1 - Cadastrar conta\n"); 
      printf("2 - Editar conta\n");
      printf("3 - Remover conta\n");
      printf("4 - Listar contas\n");
      printf("5 - Transferir saldo entre contas\n");
      printf("6 - Listar saldo total\n");
      printf("7 - Movimentar conta\n");
      printf("9 - Encerrar o programa\n");
      printf("\nDigite a opcao desejada: ");
      scanf("%d",&opcao_1);
      printf("Opcao selecionada %d\n\n", opcao_1);
	
     switch (opcao_1)
	{
		case 1:
			lista_conta=cadastrar_conta(lista_conta);
		break;
		
		case 2:
			editar_conta(lista_conta);
		break;
		
		case 3:
			lista_conta = excluir_conta(lista_conta);
		break;
		
		case 4:
			print_contas(lista_conta);
        	printf("\n\n");
        break;
        
        case 5:
        	transferir(lista_conta);
        break;
        
        case 6:
        	print_saldo(lista_conta);
        	printf("\n\n");
        break;
        
        case 7:
        	printf("Insira os dados da conta que deseja movimentar:\n");
        	dado_conta = criar_conta(1);
        	
        	mov_conta = busca(lista_conta, *dado_conta);
        	
        	if(mov_conta!=NULL)
        	{
        		opcao_2=0;
        		
	        	while(opcao_2!=9){
	        		
	        	printf("-- O que deseja fazer --\n");
				    printf("1 - Receitas\n"); 
				    printf("2 - Depesas\n");
				    printf("9 - Voltar\n");
				    printf("\nDigite a opcao desejada: ");
				    scanf("%d",&opcao_2);
				    printf("Opcao selecionada %d\n\n", opcao_2);
				    
				    switch(opcao_2)
				    {
				    	
				    	case 1:				    				
				    		movimentar_receitas(mov_conta);
				    	break;
				    	
				    	case 2:
				    		movimentar_despesas(mov_conta);
				    	break;
				    	
				    	case 9:
				    	break;
				    	
				    	default:
				        	printf("Opcao invalida!\n\n");
				        break;
					}
				}
			}
        break;
        
        case 9:
		break;
        
        default:
        	printf("Opcao invalida!\n\n");
        break;
	}
          
   }
   
   printf("Programa encerrado!");
   
   return 0;
}
