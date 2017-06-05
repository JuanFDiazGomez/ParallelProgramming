#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[])
{
	char clave[26];
	int clave_control[26];
	for(int i = 0; i < 26; i++){
		clave_control[i] = 0;
	}

	srand(time(NULL));

	for(int i = 0; i < 26;i++){
		int rand_num;
		do{
			rand_num = rand()%26;
		}while(clave_control[rand_num]==1 || rand_num == i);
		clave[i] = rand_num + 97;
		clave_control[rand_num] = 1;
		//printf("%c -> %c\n",i+97,rand_num+97);
	}
	FILE *fich;
	FILE *fichCifrado;
	FILE *fichClave;

 	fich=fopen(argv[1],"r");
 	fichCifrado = fopen("cifrado.txt","w");
 	fichClave = fopen("clave.txt","w");

 	for(int i = 0; i < 26;i++){
 		fputc(i+97,fichClave);
 		fputc('=',fichClave);
 		fputc(clave[i],fichClave);
 		fputc('\n',fichClave);
 	}

 	fseek(fich, 0L, SEEK_END);
 	int tam_fichero = ftell(fich);
 	printf("%d\n",tam_fichero);
 	fseek(fich, 0L, SEEK_SET);
 	for (int i = 0; i < tam_fichero; ++i)
 	{
 		int caracter = fgetc(fich);
 		if(caracter > 96 && caracter < 123){
 			fputc(clave[caracter-97],fichCifrado);
 			//printf("%c",clave[caracter-97]);
 		}else{
 			fputc(caracter,fichCifrado);
 			//printf("%c",caracter);
 		}
 		
 	}
 	//printf("\n");
 	//printf("1984.txt ocupa %ld bytes", ftell(fich));
 	fclose(fich);
 	fclose(fichCifrado);
 	fclose(fichClave);
	return 0;
}