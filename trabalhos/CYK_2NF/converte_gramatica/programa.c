#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// OS NÃO TERMINAIS VÃO DE A-Z (ASCI = 65-90)
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
    int i = 0;
    int tamanhoLine;
    char line[100];
	char nt[50];
	char regra[50];
	char letra = 'A';
	int k;

	system("cls");

    FILE *arquivo;
    arquivo = fopen("palavras.txt", "r+");

    if (arquivo == NULL)
        return EXIT_FAILURE;

    while(fgets(line, 100, arquivo) != NULL)
    {
		if(line[0] != '$')
			nt[i] = line[0];
        i++;
    }
    nt[i] = '\0';
	fseek( arquivo, 0, SEEK_SET );
	i=0;
    while(fgets(line, 100, arquivo) != NULL)
	{
		tamanhoLine = strlen(line);
       for(int j = 2; j<strlen(line); j++){
       		if(line[j] != '|' && line[j] != '\n'){
       			regra[i] = line[j];
       			i++;
       		}
       		else{
       			k = j-1; // pega posição antes da barra para reduzir a regra
       			regra[i] = '\0';
       			if(strlen(regra) > 2){
       				while(verificaLetraEmNT(nt,letra)){
       					letra++;
       				}
       				line[j - strlen(regra) + 1] = letra;
       				nt[strlen(nt)] = letra;
       				nt[strlen(nt) + 1] = '\0';
       				printf("valor de k: %d\n",k);
       				for(int a = 0 ; a < strlen(regra) - 2 ; a++){
						for(int a = k ; a < strlen(line) ; a++){
							line[a] = line[a+1];
							k--;
						}
					}
					//puts(line);
       			}
       			i=0;
       		}
       	}
    }
    //printf("\n\nretorno da funcao: %d",verificaLetraEmNT(nt,'A'));
    //printf("\nab\bc\n");
	//puts("\nImprimindo nao-terminais:");
	//puts(nt);

    fclose(arquivo);

    return EXIT_SUCCESS;
}