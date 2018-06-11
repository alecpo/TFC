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
void imprimeVetInt(int vet[], int tam){
	for (int i = 0; i < tam; ++i)
	{
		printf("%d ", vet[i]);
	}
	puts("");
}
int retornaIndice (char variaveis[],char variavel){
	for (int i = 0; i < strlen(variaveis); ++i){
		if(variaveis[i] == variavel)
			return i;
	}
}
char removeFrom(char vet[]){
	char c = vet[0];
	for (int i = 0; i < strlen(vet); ++i)
	{
		vet[i] = vet[i+1];
	}
	return c;
}
void arredaEInsere(){
	//char aux[2], *beginRegraUnit;
	char teste[] = "", *beginRegraUnit;
	strcat(teste,"pato donald eh gay");
	
	beginRegraUnit = strstr(teste,"d");
	
	puts(beginRegraUnit);
	/*aux[1] = '\0'; char occurs[], int tam, char varProdutora, char regraUnit[]

	for (int i = 0; i < tam ; ++i){
		if(){
			while(occurs[i] != '$'){
				i++;
			}

		}
	}*/
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
    /*
    puts("\nVariaveis Iniciais:");
	puts(nt);
	puts("Gramatica:");
	puts(arqStr);
	puts("Tamanho inicial da gramatica:");
	printf("%d\n", strlen(arqStr));;
	*/

	for(int j = 2; j<strlen(arqStr); j++){
		//printf("teste j: %c\n", arqStr[j]);
		if(arqStr[j] != '|' && arqStr[j] != '$'){
			regra[i] = arqStr[j];
			i++;			
		}
		else{
			regra[i] = '\0';
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
			}
		}
	}

	/*
	puts("\nVariaveis Finais:");
	puts(nt);
	puts("Gramatica convertida:");*/
	puts(arqStr);/*
	puts("Tamanho final da gramatica:");
	printf("%d\n", strlen(arqStr));
	*/
	fclose(arquivo);

	/*======================== FIM DA CONVERSÃO ========================*/
	/*========================DEFINIÇÃO: LAMBDA = & ====================*/

	int qntNT = strlen(nt), nullable[qntNT], ia; //nullable representa quais terminais são anuláveis (0 ou 1), a ordem é a mesma do vetor "nt",ou seja os indices representam as variaveis, exemplo: 0:E, 1:T etc..
	char toDo[] = "",occurs[] = "",regraAux[] = "", ntCopy[qntNT], aux1[2],aux2[4],firstIn;
	char *var; // var = variavel que produziu &.

	aux1[1] = '\0';
	aux2[3] = '\0';

	/*=============== zera nullable ===============*/
	for (int a = 0; a < strlen(nt); a++){
		nullable[a] = 0;
	}

	/*=============== for all A pertencente a N, occurs(a) = vazio. ===============*/
	for (int a = 0; a < qntNT; a++){
		aux2[0] = nt[a];
		aux2[1] = '>';
		aux2[2] = '$';
		strcat(occurs,aux2);
	}

	/*=============== for all A -> B do ===============*/
	i = 0;
	ia = 0;
	for(int j = 2; j<strlen(arqStr); j++){
		if(arqStr[j] != '$'){
			regra[i] = arqStr[j];
			i++;			
		}
		else{
			regra[i] = '$';
			regra[i+1] = '\0';
			for (int a = 0; a < strlen(regra); a++)
			{
				if(regra[a] != '|' && regra[a] != '$' && regra[a] != '&'){
					aux1[0] = regra[a];
					strcat(regraAux,aux1);
				}
				else{
					if(strlen(regraAux) == 1 && strstr(nt,regraAux)){ // se for produção unitária e a regra for uma variável
						puts(regraAux);
						var = strstr(arqStr,regraAux);
						var-=2;
						//printf("%c -> %s\n",*var,regraAux);
						arredaEInsere(); // occurs(B) := occurs(B) U {A} occurs,strlen(occurs),*var,regraAux
					}
					strcpy(regraAux,"");
					//puts(regraAux);
				}
			}		
			
			if(arqStr[j] == '$')
				j+=2;
			i = 0;
		}
	}

	/*=============== for all A -> BC do ===============*/	

	/*=============== for all A -> & do ===============*/
	i = 0;
	for(int j = 2; j<strlen(arqStr); j++){
		if(arqStr[j] != '$'){
			regra[i] = arqStr[j];
			i++;			
		}
		else{
			regra[i] = '\0';
			if(var = strstr(regra,"&")){
				var = strstr(arqStr,regra); // ?> 'var' -> ..&...                  -> = aponta para
				var-=2; // pega a variavel que produziu &, 'var' -> ?>...&...
				nullable[retornaIndice(nt,*var)] = 1;//seta 1 para todas as variaveis que produzem &.
				aux1[0] = *var;
				strcat(toDo,aux1);
			}
			if(arqStr[j] == '$')
				j+=2;
			i = 0;
		}
	}

	/*=============== while toDo != vazio do ===============*/
	while(strcmp(toDo,"")){
		firstIn = removeFrom(toDo);
	}
	//imprimeVetInt(nullable,strlen(nt));


    

    return EXIT_SUCCESS;
}
