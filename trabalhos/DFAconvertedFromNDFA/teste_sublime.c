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

void main(int argc, char *argv[])
{
	FILE *arqAFN;
	FILE *arqAFD;
	int i,linha,or,tr,x=0,aceita;
	char *busca;
	char Linha[100];
	char estadosAFN[100] = "";
	char estadosFinaisAFN[100] = "";
	char estadosAFD[100] = "";
	char estadosFinaisAFD[100] = "";
	char estadosPuros[100] = "";
	char alfabeto[50] = "";
	char aux[2] = "\0",origem,destino,transicao;
	char *result;
	system("cls");
	// Abre um arquivo TEXTO para LEITURA
	arqAFN = fopen("afn.jff", "rt");
	if (arqAFN == NULL){  // Se houve erro na abertura
		printf("Problemas na abertura do arquivo\n");
		return;
	}

	while (!feof(arqAFN)){
	// Lê uma linha (inclusive com o '\n')
	result = fgets(Linha, 100, arqAFN);  // o 'fgets' lê até 99 caracteres ou até o '\n'
	if (result)  // Se foi possível ler
		if(busca = strstr(Linha,"id=")){
			aux[0]=*(busca+4);
			strcat(estadosPuros,aux);
			
		}
		else 
			if(strstr(Linha,"<fi")){
				printf("\nAchou estado de aceite\n");
				strcat(estadosFinaisAFN,aux);
				strcat(estadosFinaisAFD,aux);
			}
			else 
				if(strstr(Linha,"</sta")){
					strcat(estadosAFN,aux);
					strcat(estadosAFD,aux);
					//InserirElementoVetor(aux[0],estadosAFD,strlen(estadosAFD));
				}
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
	char estAux[strlen(estadosPuros)+1],linhaALinha[strlen(estadosPuros)+1];
	char afd[(strlen(estadosPuros)*strlen(estadosPuros))-1][strlen(alfabeto)];
	char novosEstadosParaOAFD[qntNovosEstAFD][strlen(estadosPuros)];
	char uniao[strlen(estadosPuros)+1];
	char auxOrdenacao[strlen(estadosPuros)+1];

	limpaVetor(strlen(estadosPuros)+1,auxOrdenacao);
	limpaVetor(strlen(estadosPuros)+1,estAux);
	limpaVetor(strlen(estadosPuros)+1,uniao);
	limpaVetor(strlen(estadosPuros)+1,linhaALinha);

	for(i = 0; i<strlen(estadosPuros); i++)
		for (int  j= 0; j < strlen(alfabeto); j++)
			for (int k = 0; k < strlen(estadosPuros); k++)
				afn[i][j][k]='|';//inicia afn sem transicoes (representado pelo simbolo |).
	fseek(arqAFN,0,SEEK_SET);
	while (!feof(arqAFN)){
		result = fgets(Linha, 100, arqAFN);
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
	/********* IMPORANTE: ORDENA AFN ***********/ 
	for(i = 0; i<strlen(estadosPuros); i++)
		for (int  j= 0; j < strlen(alfabeto); j++){
			for (int k = 0; k < strlen(estadosPuros); k++){
				InserirElementoVetor(afn[i][j][k],auxOrdenacao,strlen(estadosPuros));
				if(k == strlen(estadosPuros)-1){
					for (int a = 0; a < strlen(estadosPuros); a++){
						afn[i][j][a] = auxOrdenacao[a];
					}
				}
			}
			limpaVetor(strlen(estadosPuros)+1,auxOrdenacao);
		}



	printf("Estados AFN: %s\nEstados Finais do AFN: %s\nQnt Estados do AFN: %d\n",estadosAFN,estadosFinaisAFN,strlen(estadosAFN));

	printf("Alfabeto: %s\n",alfabeto);

	fclose(arqAFN);

	//validação 1: Aceita vazio ?
	if(argc == 1){
		aux[0] = estadosAFN[0];
		if(strstr(estadosFinaisAFN,aux))//se o estado inicial for de aceite, o automato aceita frase vazia
			aceita = 1;
		else{
			system("cls");
			printf("REJEITA\n");
			exit(1);			
		}
	}
	//validação 2: sentenca possui apenas terminais que fazem parte do alfabeto ?
	char sentenca[strlen(argv[1])];
	strcpy(sentenca,argv[1]);
	for(int t = 0; t < strlen(sentenca); t++){
		aux[0] = sentenca[t];
		if(!strstr(alfabeto,aux)){
			system("cls");
			printf("REJEITA\n");
			exit(1);
		}
	}
	
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
			limpaVetor(strlen(estadosPuros)+1,estAux);
			limpaVetor(strlen(estadosPuros)+2,linhaALinha);
			//printf("\nlinhaALinha limpado: %s\n", linhaALinha);
			for (int k = 1; k < strlen(estadosPuros); k++){
				if(afn[i][j][k] != '|'){
					if(k == 1){
						InserirElementoVetor(afn[i][j][0], estAux, strlen(estadosPuros));
						InserirElementoVetor(afn[i][j][k], estAux, strlen(estadosPuros));
						if(afn[i][j][k+1] == '|' || afn[i][j][k+1] == '\0'){
							//printf("\nestAux com 2: %s Pode talvez inserir na linha: %d\n", estAux,linha);
							//verifica se ja tem o estado em novosEstadosParaOAFD
							for (int m = 0; m <= linha; ++m){
								for (int n = 0; n < strlen(estadosPuros); ++n){ //copia cada linha em linhaALinha
									InserirElementoVetor(novosEstadosParaOAFD[linha][n],linhaALinha,strlen(estadosPuros)+1);
								}
								//printf("linhaALinha[%d]: %s\n",m, linhaALinha);
								if(!strstr(linhaALinha,estAux)){//se nao tiver, ai eu insiro o novo estado na matriz novosEstadosParaOAFD 
									if(strstr(linhaALinha,"|||||")){
										//printf("A linhaAlinha[%d] Nao contem nada\n",linha);
										for(int k=0; k<strlen(estadosPuros); k++){
											novosEstadosParaOAFD[linha][k] = estAux[k];
										}
										//printf("Incluido com sucesso: novosEstadosParaOAFD[%d] = ", linha);
										//for(int k=0; k<strlen(estadosPuros); k++){
										//	printf("%c",novosEstadosParaOAFD[linha][k]);
										//}
										//printf("\n");
									}	
								}
								else{
									break;
								}
							}
							linha++;
							break;//nao fica fazendo compação desnecessaria com o '|', ou seja, sai do for do k
						}
					}
					else{
						InserirElementoVetor(afn[i][j][k], estAux, strlen(estadosPuros));
						if(afn[i][j][k+1] == '|' || afn[i][j][k+1] == '\0'){
							//printf("\nestAux : %s Linha: %d\n", estAux,linha);
							//verifica se ja tem o estado em novosEstadosParaOAFD
							for (int m = 0; m <= linha; ++m){
								for (int n = 0; n < strlen(estadosPuros); ++n){ //copia cada linha em linhaALinha
									InserirElementoVetor(novosEstadosParaOAFD[linha][n],linhaALinha,strlen(estadosPuros)+1);
								}
								//printf("linhaALinha: %s\n", linhaALinha);
								if(!strstr(linhaALinha,estAux)){//se nao tiver, ai eu insiro o novo estado na matriz novosEstadosParaOAFD 
									if(strstr(linhaALinha,"|||||"))
										for(int k=0; k<strlen(estadosPuros); k++){
											novosEstadosParaOAFD[linha][k] = estAux[k];
										}
								}
								else{//se achar o estado na tabela, para de procurar (sai do for do m ali em cima)
									break;
								}
							}
							linha++;
							break;//nao fica fazendo compação desnecessaria com o '|', ou seja, sai do for do k
						}
					}
				}else
					if(k == 1){
						if(afn[i][j][0] != '|')
							afd[i][j] = afn[i][j][0]; //copia transição unitária para afd
					}
			}
			//nesse ponto eu inseri um novo estado na matriz novosEstadosParaOAFD se ele ja nao estiver la.
			
			if(afd[i][j] == '|'){
				//verifica linha a linha da matriz de novos estadosAFN se o estado ja está inserido
				for (int m = 0; m < linha; ++m){
					limpaVetor(strlen(estadosPuros)+2,linhaALinha);
					for (int n = 0; n < strlen(estadosPuros); ++n){ //copia cada linha em linhaALinha
						InserirElementoVetor(novosEstadosParaOAFD[m][n],linhaALinha,strlen(estadosPuros)+1);
						//linhaALinha[n] = novosEstadosParaOAFD[m][n];
					}
					//se a celula do AFD for vazio, preenche com qntEstadoVelho + indice
					printf("\nlinhaALinha: %s\n", linhaALinha);
					printf("\nestAux: %s\n", estAux);
					
					if(strstr(linhaALinha,estAux)){//se coincidir, cria estado novo na matriz do AFD
						
						aux[0] = (m + strlen(estadosPuros)) + '0'; //passa para char.
						//printf("aux[0] = %c\n",aux[0]);
						afd[i][j] = aux[0];
						if(aux[0] - '0' == strlen(estadosPuros)){

							if(!strstr(estadosAFD,aux)){
								//system("pause");
								//printf("\nConcatenando '%c' em estadosAFD\n",aux[0]);
								strcat(estadosAFD,aux);
							}							
						}
						else{
							//printf("\nInserindo '%c' em estadosAFD\n",aux[0]);
							//InserirElementoVetor(aux[0],estadosAFD,strlen(estadosAFD));
							strcat(estadosAFD,aux);
						}	
					}
				}
			}
		}
	}

	/* ----------------------CONSTRUINDO MATRIZ DE TRANSICAO AFD---------------------- */
	
	for (i = strlen(estadosPuros); i < (strlen(estadosPuros)*strlen(estadosPuros))-1; ++i){//contunua à partir de onde parou 
		for(int j = 0; j < strlen(alfabeto); j++){				//(depois de ter copiado do AFN, transformando conjunto de estadosAFN em apenas um)
			for (int m = 0; m < strlen(estadosPuros); ++m)
				if(novosEstadosParaOAFD[i-strlen(estadosPuros)][m] != '|')//percorre cada celula da matriz de novos estadosAFN 
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
						if(strstr(linhaALinha,uniao)){//se ja existir o estado, insere no AFD a transicao para ele 
							afd[i][j] = (m + strlen(estadosPuros)) + '0'; //passa para char.
						}
						else 											  //senão, cria novo estado e insere transicao no AFD
							if(m == qntNovosEstAFD-1){
								for(int l = 0; l < strlen(estadosPuros); l++)
									novosEstadosParaOAFD[linha][l] = uniao[l];
								aux[0] = (linha + strlen(estadosPuros)) + '0';
								afd[i][j] = aux[0];
								//printf("\nConcatenando '%c' em estadosAFD\n",aux[0]);
								printf("\nConcatenando '%c' em estadosAFD\n",aux[0]);
								strcat(estadosAFD,aux);
								linha++;
							}
				}
			limpaVetor(strlen(estadosPuros)+1,uniao);				
		}	
	}

	//preenche vetor de estados finais do afd
	for (int m = 0; m < qntNovosEstAFD; ++m)
		for (int n = 0; n < strlen(estadosPuros); ++n){
			aux[0] = novosEstadosParaOAFD[m][n];
			if( strstr(estadosFinaisAFN,aux) ) {
				aux[0] = (m + strlen(estadosPuros)) + '0';
				strcat(estadosFinaisAFD,aux);
				break;
			}
		}

	// monta AFD.jff
	arqAFD = fopen("afd.jff", "wt");
	if (arqAFD == NULL){  // Se houve erro na abertura
		printf("Problemas na abertura do arquivo\n");
		return;
	}
	//inicializando arquivo AFD.jff
	fputs("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?><!--Created with JFLAP 6.4.~sqn~-->\n",arqAFD);
	fputs("<structure>\n\t<type>fa</type>\n\t<automaton>\n\t\t<!--The list of states.-->\n",arqAFD);
	//criando estados
	aux[0] = estadosAFD[0];
	fputs("\t\t<state id=\"",arqAFD); fprintf(arqAFD,"%d",aux[0]-'0'); fputs("\" name=\"",arqAFD); fprintf(arqAFD,"%d",aux[0]-'0'); fputs("\">\n",arqAFD);
	fputs("\t\t\t<x>",arqAFD); fprintf(arqAFD,"%d",x); fputs(".0</x>\n",arqAFD);
	fputs("\t\t\t<y>108.0</y>\n",arqAFD);
	fputs("\t\t\t<initial/>\n",arqAFD);
	fputs("\t\t</state>\n",arqAFD);
	x += 40;
	for(int e = 1; e < strlen(estadosAFD); e++){ //estados normais
		aux[0] = estadosAFD[e];
		if(!strstr(estadosFinaisAFD,aux)){//verifica se nao é estado final
			fputs("\t\t<state id=\"",arqAFD); fprintf(arqAFD,"%d",aux[0]-'0'); fputs("\" name=\"",arqAFD); fprintf(arqAFD,"%d",aux[0]-'0'); fputs("\">\n",arqAFD);
			fputs("\t\t\t<x>",arqAFD); fprintf(arqAFD,"%d",x); fputs(".0</x>\n",arqAFD);
			fputs("\t\t\t<y>108.0</y>\n",arqAFD);
			fputs("\t\t</state>\n",arqAFD);
			x += 40;
		}	
	}
	for(int e = 0; e < strlen(estadosFinaisAFD); e++){ //estados finais
		aux[0] = estadosFinaisAFD[e];
		fputs("\t\t<state id=\"",arqAFD); fprintf(arqAFD,"%d",aux[0]-'0'); fputs("\" name=\"",arqAFD); fprintf(arqAFD,"%d",aux[0]-'0'); fputs("\">\n",arqAFD);
		fputs("\t\t\t<x>",arqAFD); fprintf(arqAFD,"%d",x); fputs(".0</x>\n",arqAFD);
		fputs("\t\t\t<y>108.0</y>\n",arqAFD);
		fputs("\t\t\t<final/>\n",arqAFD);
		fputs("\t\t</state>\n",arqAFD);
		x += 40;
	}
	//cria transicoes
	fputs("\t\t<!--The list of transitions.-->\n",arqAFD);
	for(int m = 0; m < (strlen(estadosPuros)*strlen(estadosPuros))-1; m++)
		for(int n = 0; n < strlen(alfabeto); n++){
			if(afd[m][n] != '|'){
				fputs("\t\t<transition>\n",arqAFD);
				fputs("\t\t\t<from>",arqAFD); fprintf(arqAFD,"%d",m); fputs("</from>\n",arqAFD);
				fputs("\t\t\t<to>",arqAFD); fprintf(arqAFD,"%d",afd[m][n] - '0'); fputs("</to>\n",arqAFD);
				fputs("\t\t\t<read>",arqAFD); fputc(alfabeto[n],arqAFD); fputs("</read>\n",arqAFD);
				fputs("\t\t</transition>\n",arqAFD);
			}
		}
	fputs("\t</automaton>\n</structure>",arqAFD);

	fclose(arqAFD);

	aux[0]=estadosAFD[0]; //aux = estado atual. Começa no estado inicial; 
	//consome sentença e decide se aceita
	for(int t = 0; t < strlen(sentenca); t++){
		for(int a = 0; a < strlen(alfabeto); a++){//percorro o alfabeto para pegar o indice do terminal lido na entrada (valor numerico do terminal)
			if(sentenca[t] == alfabeto[a])
				if(afd[aux[0] - '0'][a] != '|')
					aux[0] = afd[aux[0] - '0'][a]; //muda pro estado de destino
				else{
					system("cls");
					printf("REJEITA\n");
					exit(1);
				}
		}
	}

	if(strstr(estadosFinaisAFD,aux))
		aceita = 1;
	else
		aceita = 0;


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

	//imprime AFD
	printf("\n\nImprimindo afd...\n    ");
	for (int terminal = 0; terminal < strlen(alfabeto); ++terminal)
	{
		printf(" %c   ",alfabeto[terminal]);
	}
	printf("\n");
	for (i = 0; i < (strlen(estadosPuros)*strlen(estadosPuros))-1; ++i){
		//if(i<strlen(estadosAFD)){
			printf("[%d] ",i);
			for(int j = 0; j < strlen(alfabeto); j++){
				if(afd[i][j] != '|')
					printf("[%c]  ",afd[i][j]);
				else
					printf("     ");
			}
			printf("\n");
		//}
	}

	printf("\nEstados AFD: %s\nEstados Finais AFD: %s\nQnt Estados AFD: %d\n\n",estadosAFD,estadosFinaisAFD,strlen(estadosAFD));

	if(aceita){
		printf("\n\nACEITA\n");
		exit(1);
	}else{
		printf("\n\nREJEITA\n");
		exit(1);
	}

	return;
}
