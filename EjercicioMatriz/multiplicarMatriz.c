#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main(int argc, char const *argv[])
{
	srand(time(NULL));

	// Tamaño de la matriz que vamos a multiplicar
	int TAM = 1000;
	/*Obtenemos esta informacion para poder recorrer
	de forma efectiva la multiplicacion, por que multiplicaremos
	simultaneamente desde los extremos al centro y luego si 
	es par cogeremos la posicion central que nos falta
	esto en secuencial es viable pero en paralelo podria dar problemas*/
	int es_par = TAM % 2;
	int limite_for = TAM/2;
	// Reservamos el espacio para las matrizes
	int **matrizA, **matrizB;
	matrizA = (int **) malloc(TAM*sizeof(int*));
	matrizB = (int **) malloc(TAM*sizeof(int*));
	for (int i = 0; i < TAM; ++i){
		matrizA[i]=(int *) malloc (TAM*sizeof(int));
		matrizB[i]=(int *) malloc (TAM*sizeof(int));
	}
	for(int row = 0; row < TAM; row++){
		for(int col = row; col < TAM; col++){
			matrizA[row][col] = rand()%50;
			matrizA[col][row] = matrizA[row][col];
		}
	}
	// Cogemos el instante actual para comprobar la duracion
	double inicio = omp_get_wtime();
	for(int row = 0; row < TAM; row++){
		for(int col = row; col < TAM; col++){
			// En esta variable guardaremos la multiplicacion de cada fila
			int sum1 = 0;
			//recorro desde 0 hasta la mitad de la longitud de la fila
			//y voy leyendo desde 0 hasta n/2 y desde n-1 hasta n/2
			for(int k = 0; k < limite_for; k++){
				sum1 += matrizA[row][k] * matrizA[k][col];
				sum1 += matrizA[row][TAM-k-1] * matrizA[TAM-k-1][col];
			}
			// las filas pares dejan sin leer una posicion que es n/2
			// entonces hago el calculo sobre dicha posicion
			if(es_par){
				sum1 += matrizA[row][limite_for] * matrizA[limite_for][col];
			}
			matrizB[row][col] = sum1;
			matrizB[col][row] = sum1;
		}
	}

	double fin = omp_get_wtime();
	printf("Tiempo: %lf\n",fin-inicio);
	/*for(int i = 0; i < TAM; i++){
		for(int j = 0; j < TAM; j++){
			printf("%d ", matrizA[i][j]);
		}
		printf("\n");
	}

	printf("\n");
	for(int i = 0; i < TAM; i++){
		for(int j = 0; j < TAM; j++){
			printf("%d ", matrizB[i][j]);
		}
		printf("\n");
	}*/

	for (int i = 0; i < TAM; ++i){
		free(matrizA[i]);
		free(matrizB[i]);
	}
	free(matrizA);
	free(matrizB);
	return 0;
}