#include<stdlib.h> 
#include <stdio.h>
#include <string.h>

int DeterminarPosicaoInsercao (char NovoElem, char V[], int tam){
    int i = 0, k = -1;

    while ( (i < tam) && (k == -1) )
        if (NovoElem == V[i])
            return -1;
        else
            if (V[i] < NovoElem)
                i++;
            else
                k = i;
    return (k);
}

void InserirElementoVetor (char NovoElem, char V[], int tam){
    int pos, i;
    pos = DeterminarPosicaoInsercao (NovoElem, V, tam);
 
    if (pos >= 0){ // Avançar uma posição todos os elementos de V desde posição pos
        for (i = tam-1; i > pos; i--)
            V[i] = V[i-1];
        V[pos] = NovoElem; // Inserir NovoElem na posição pos
    }
}

void limpaVetor(int n, char *estAux){

	for (int i = 0; i < n-1; ++i)
		estAux[i] = '|';
	estAux[n-1] = '\0';
}

void main()
{
	FILE *arq;
	int i,linha,coluna,novosEstados=0,or,tr;
	char *busca;
	char Linha[100];
	char estadosAFN[100] = "";
	char estadosAFD[100] = "";
	char estadosPuros[100] = "";
	char alfabeto[50] = "";
	char aux[2] = "\0",origem,destino,transicao;
	char *result;
	system("cls");
	// Abre um arquivo TEXTO para LEITURA
	arq = fopen("af.jff", "rt");
	if (arq == NULL){  // Se houve erro na abertura
		printf("Problemas na abertura do arquivo\n");
		return;
	}

	while (!feof(arq)){
	// Lê uma linha (inclusive com o '\n')
	result = fgets(Linha, 100, arq);  // o 'fgets' lê até 99 caracteres ou até o '\n'
	if (result)  // Se foi possível ler
		if(busca = strstr(Linha,"id=")){
			aux[0]=*(busca+4);
			strcat(estadosAFN,aux);
			strcat(estadosPuros,aux);
			
		}
		else
			if(strstr(Linha,"<i"))
				strcat(estadosAFN,"i");
			else 
				if(strstr(Linha,"<fi"))
					strcat(estadosAFN,"f");
				else 
					if(strstr(Linha,"</sta"))
						strcat(estadosAFN,";");
					else
						if(busca = strstr(Linha,"<r")){
							aux[0]=*(busca+6);
							if(!strstr(alfabeto,aux))
								strcat(alfabeto,aux);
						}
	}
	for(i=0; i<strlen(alfabeto); i++)
		printf("%c = %d\n",alfabeto[i],i);

	int qntNovosEstAFD=(strlen(estadosPuros)*strlen(estadosPuros))-1-strlen(estadosPuros);
	char afn[strlen(estadosPuros)][strlen(alfabeto)][strlen(estadosPuros)];//tercero eixo no pior dos casos
	char estAux[strlen(estadosPuros)+1],linhaALinha [strlen(estadosPuros)+1];
	char afd[(strlen(estadosPuros)*strlen(estadosPuros))-1][strlen(alfabeto)];
	char novosEstadosParaOAFD[qntNovosEstAFD][strlen(estadosPuros)];
	char uniao[strlen(estadosPuros)+1];

	limpaVetor(strlen(estadosPuros)+1,estAux);
	limpaVetor(strlen(estadosPuros)+1,uniao);

	for(i = 0; i<strlen(estadosPuros); i++)
		for (int  j= 0; j < strlen(alfabeto); j++)
			for (int k = 0; k < strlen(estadosPuros)*strlen(alfabeto); k++)
				afn[i][j][k]='|';//inicia afn sem transicoes (representado pelo simbolo §).
	fseek(arq,0,SEEK_SET);
	while (!feof(arq)){
		result = fgets(Linha, 100, arq);
		if(result)
			if(busca = strstr(Linha,"<fr"))
				origem=*(busca+6);
			else
				if(busca = strstr(Linha,"<to"))
					destino=*(busca+4);
				else
					if(busca = strstr(Linha,"<r")){
						transicao=*(busca+6);
						or = origem - '0';
						for(i = 0; i<strlen(alfabeto); i++) //pega indice da transicao no vetor(alfabeto) 
							if(alfabeto[i]==transicao)	//para mapear em numero
								tr = i;
						i=0;
						while(afn[or][tr][i] != '|')	//como nao existe transicao vazia
							i++;						//enquanto tiver algo no terceiro eixo
														//que seja igual a algum simbolo do alfabeto
						afn[or][tr][i] = destino;
					}
	}
	printf("Estados AFN: %s TamString: %d\n",estadosAFN,strlen(estadosAFN));
	estadosAFD[0] = estadosAFN[0];
	estadosAFD[1] = estadosAFN[1];
	estadosAFD[2] = estadosAFN[2];
	printf("Alfabeto: %s\n",alfabeto);

	fclose(arq);
	
	//inicia AFD sem transicoes.
	for (i = 0; i < (strlen(estadosPuros)*strlen(estadosPuros))-1; i++)
		for (int j = 0; j < strlen(alfabeto); j++)
			afd[i][j] = '|';

	linha = 0;
	for (i = 0; i < strlen(estadosPuros)*strlen(estadosPuros); i++)
		for (int j = 0; j < strlen(estadosPuros); j++)
			novosEstadosParaOAFD[i][j] = '|';

	printf("matriz = %dx%d\n\n",strlen(estadosPuros)*strlen(estadosPuros)-1,strlen(estadosPuros));

	//PERCORRE O AFN E INSTANCIA NOVOS ESTADOS NA MATRIZ novosEstadosParaOAFD
	for(i = 0; i<strlen(estadosPuros); i++){
		for (int  j= 0; j < strlen(alfabeto); j++){
			for (int k = 1; k < strlen(estadosPuros); k++)
				if(afn[i][j][k] != '|')
					if(k == 1){
						novosEstados++;
						novosEstadosParaOAFD[linha][0] = afn[i][j][0];
						estAux[0] = afn[i][j][0];
						coluna++;
						novosEstadosParaOAFD[linha][k] = afn[i][j][k];
						coluna++;
						estAux[k] = afn[i][j][k];
						linha++;
					}
					else{
						novosEstadosParaOAFD[linha][k] = afn[i][j][k];
						coluna++;
						estAux[k] = afn[i][j][k];
					}
				else
					if(k == 1)
						if(afn[i][j][0] != '|')
							afd[i][j] = afn[i][j][0]; //copia transição unitária para afd

			//verifica linha a linha da matriz de novos estadosAFN se o estado ja está inserido
			for (int m = 0; m < qntNovosEstAFD; ++m){
				for (int n = 0; n < strlen(estadosPuros); ++n) //copia cada linha em linhaALinha
					linhaALinha[n] = novosEstadosParaOAFD[m][n];
				if(afd[i][j] == '|')//se a celula do AFD for vazio, preenche com qndEstadoVelho + indice
					if(strstr(linhaALinha,estAux))//se coincidir, cria estado novo na matriz do AFD
						afd[i][j] = (m + strlen(estadosPuros)) + '0'; //passa para char.
			}
			limpaVetor(strlen(estadosPuros)+1,estAux);
			coluna = 0;
		}
	}

	/* ----------------------CONSTRUINDO MATRIZ DE TRANSICAO AFD---------------------- */
	
	for (i = strlen(estadosPuros); i < (strlen(estadosPuros)*strlen(estadosPuros))-1; ++i){//contunua à partir de onde parou 
		for(int j = 0; j < strlen(alfabeto); j++){				//(depois de ter copiado do AFN, transformando conjunto de estadosAFN em apenas um)
			for (int m = 0; m < strlen(estadosPuros); ++m)
				if(novosEstadosParaOAFD[i-strlen(estadosPuros)][m] != '|')//percorre cada celula da matriz de novos estadosAFN 
					//PAREI AQUI, NESSA CONSTRUÇÃO DESSE FOR.
					for (int c = 1; c < strlen(estadosPuros); c++)
						if(afn[(novosEstadosParaOAFD[i-strlen(estadosPuros)][m]-'0')][j][c] != '|')
							if(c == 1){
								InserirElementoVetor(afn[(novosEstadosParaOAFD[i-strlen(estadosPuros)][m]-'0')][j][0],uniao,strlen(estadosPuros));
								InserirElementoVetor(afn[(novosEstadosParaOAFD[i-strlen(estadosPuros)][m]-'0')][j][c],uniao,strlen(estadosPuros));
							}
							else
								InserirElementoVetor(afn[(novosEstadosParaOAFD[i-strlen(estadosPuros)][m]-'0')][j][c],uniao,strlen(estadosPuros));
						else
							if(c == 1)//transicao de apenas 1 estado
								if(afn[(novosEstadosParaOAFD[i-strlen(estadosPuros)][m]-'0')][j][0] != '|')
									InserirElementoVetor(afn[(novosEstadosParaOAFD[i-strlen(estadosPuros)][m]-'0')][j][0],uniao,strlen(estadosPuros));
			if(uniao[1] == '|')
				afd[i][j] = uniao[0];
			else //verifica linha a linha da matriz de novos estadosAFN se o estado ja está inserido
				for (int m = 0; m < qntNovosEstAFD; ++m){
					for (int n = 0; n < strlen(estadosPuros); ++n)
						linhaALinha[n] = novosEstadosParaOAFD[m][n];
					if(afd[i][j] == '|')//se a celula do AFD for vazio, preenche com qndEstadoVelho + indice
						if(strstr(linhaALinha,uniao))//se coincidir, cria estado novo na matriz do AFD
							afd[i][j] = (m + strlen(estadosPuros)) + '0'; //passa para char.
						else
							if(m == qntNovosEstAFD-1){
								for(int l = 0; l < strlen(estadosPuros); l++)
									novosEstadosParaOAFD[linha][l] = uniao[l];
								afd[i][j] = (linha + strlen(estadosPuros)) + '0';
								linha++;
							}
				}
			limpaVetor(strlen(estadosPuros)+1,uniao);				
		}	
	}


	//imprime matriz novosEstadosParaOAFD
	printf("\n\nNovos estadosAFN que foram gerados: \n");
	for (i = 0; i < qntNovosEstAFD; i++){
		for (int j = 0; j < strlen(estadosPuros); j++)
			if(novosEstadosParaOAFD[i][j] != '|'){
				if(novosEstadosParaOAFD[i][j+1] != '|'){
					if(j == 0)
						printf("[");
					if(j == strlen(estadosPuros) - 1){
						printf("%c]",novosEstadosParaOAFD[i][j]);
						printf("\n");
					}
					else
						printf("%c,",novosEstadosParaOAFD[i][j]);
				}
				else
					if(novosEstadosParaOAFD[i][j+1] == '|'){
						printf("%c]",novosEstadosParaOAFD[i][j]);
						printf("\n");
					}
			}
	}

	//imprime AFN
	printf("\n\nImprimindo afn...\n");
	for(i=0; i<strlen(alfabeto); i++)
		printf("\t%c",alfabeto[i]);
	printf("\n");
	for(i = 0; i<strlen(estadosPuros); i++){
		printf("%c\t",estadosPuros[i]);
		for (int  j= 0; j < strlen(alfabeto); j++){
			printf("{");
			for (int k = 0; k < strlen(estadosPuros); k++)
				if(afn[i][j][k] != '|')
					printf("%c,",afn[i][j][k]);
			printf("}");
			printf("\t");
		}
		printf("\n");
	}

	//imprime AFN
	printf("\n\nImprimindo afd...\n    ");
	for (int terminal = 0; terminal < strlen(alfabeto); ++terminal)
	{
		printf(" %c   ",alfabeto[terminal]);
	}
	printf("\n");
	for (i = 0; i < (strlen(estadosPuros)*strlen(estadosPuros))-1; ++i){
		if(afd[i][0] != '|')
			printf("[%d] ",i);
		for(int j = 0; j < strlen(alfabeto); j++){
			if(afd[i][j] != '|')
				printf("[%c]  ",afd[i][j]);
		}
		printf("\n");
	}

	printf("Estados AFD: %s TamString: %d\n",estadosAFD,strlen(estadosAFD));

	system("pause");
	system("cls");	

	return;
}
