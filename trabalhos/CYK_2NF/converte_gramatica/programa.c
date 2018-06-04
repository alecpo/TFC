#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// OS NÃO TERMINAIS VÃO DE A-Z (ASCI = 65-90)
long calcularTamanhoArquivo(FILE *arquivo) {
    long posicaoAtual = ftell(arquivo);
    long tamanho;
    fseek(arquivo, 0, SEEK_END);
    tamanho = ftell(arquivo);
    fseek(arquivo, posicaoAtual, SEEK_SET);
    return tamanho;
}
int verificaLetraEmNT(char variavel[], char letra){
	char aux[2];
	aux[1] = '\0';
	aux[0] = letra;

	if(strstr(variavel,aux))
		return 1;
	else
		return 0;
}

int main(void)
{
    int tamanhoStr,i = 0; // i = indice do vetor regra
    int novoNT=0;
	char nt[50];
	char regra[50];
	char novaRegra[50];
	char letra = 'A';
	char aux[3];
	aux[2] = '\0';

	system("cls");

    FILE *arquivo;
    arquivo = fopen("palavras.txt", "r+");

    if (arquivo == NULL)
        return EXIT_FAILURE;
    tamanhoStr = calcularTamanhoArquivo(arquivo)+1;

    char arqStr[tamanhoStr*10];
	fgets(arqStr, tamanhoStr, arquivo);
	nt[novoNT] = arqStr[0]; //armazena em nt a primeira variavel da regra
    i=0;
    novoNT++;
    for(int j = 0 ; j < tamanhoStr; j++){
    	if(arqStr[j] == '$'){
    		nt[novoNT] = arqStr[j+1];
    		j+=3;
    		novoNT++;
    	}

    }
    nt[novoNT] = '\0';

	for(int j = 2; j<strlen(arqStr); j++){
		//printf("teste j: %c\n", arqStr[j]);
		if(arqStr[j] != '|' && arqStr[j] != '$'){
			regra[i] = arqStr[j];
			i++;			
		}
		else{
			regra[i] = '\0';
			puts(regra);
			if(strlen(regra) > 2){
				strcpy(novaRegra,regra);
				for (int a = 0; a < strlen(regra); ++a){
					novaRegra[a] = regra[a+1];
				}
			}

			if(arqStr[j] == '|'){
				if(strlen(regra) > 2){
					while(verificaLetraEmNT(nt,letra)){
						letra++;
					}
					arqStr[j - strlen(regra) + 1] = letra; //substitui segunda letra pela nova variavel
					//aux[0] = '$';
					aux[0] = letra;
					aux[1] = '>';
					strcat(arqStr,aux);
					strcat(arqStr,novaRegra);
					strcat(arqStr,"$");
					nt[strlen(nt)] = letra;//adiciona nova variavel no vetor de não terminais (nt).					
					nt[strlen(nt) + 1] = '\0';
					for(int a = 0 ; a < strlen(regra) - 2 ; a++){
						for(int a = j-1 ; a < strlen(arqStr) ; a++){
							arqStr[a] = arqStr[a + 1];
						}
						j--;
					}
				}
				i=0;
			}
			else{
				if(strlen(regra) > 2){
					while(verificaLetraEmNT(nt,letra)){
						letra++;
					}
					arqStr[j - strlen(regra) + 1] = letra;
					//aux[0] = '$';
					aux[0] = letra;
					aux[1] = '>';
					strcat(arqStr,aux);
					strcat(arqStr,novaRegra);
					strcat(arqStr,"$");
					nt[strlen(nt)] = letra;
					nt[strlen(nt) + 1] = '\0';
					for(int a = 0 ; a < strlen(regra) - 2 ; a++){
						for(int a = j-1 ; a < strlen(arqStr) ; a++){
							arqStr[a] = arqStr[a + 1];
						}
						j--;
					}
				}
				j+=2;
				i=0;
				printf("tam[arqStr]: %d    arqStr[%d]:%c   arqStr[%d]:%c\n\n",strlen(arqStr),j,arqStr[j],j+1,arqStr[j+1]);
			}
		}
	}

	puts("\nImprimindo nao-terminais:");
	puts(nt);
	puts(arqStr);
	printf("%d\n", strlen(arqStr));

    fclose(arquivo);

    return EXIT_SUCCESS;
}