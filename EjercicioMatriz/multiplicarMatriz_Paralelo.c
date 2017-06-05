#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int TAM;
int main(int argc, char const *argv[])
{
	TAM = 2000;
	int **matrizA, **matrizB;
	// reservo memoria para las matrices
	matrizA = (int **) malloc(TAM*sizeof(int*));
	matrizB = (int **) malloc(TAM*sizeof(int*));
	for (int i = 0; i < TAM; ++i){
		matrizA[i]=(int *) malloc (TAM*sizeof(int));
		matrizB[i]=(int *) malloc (TAM*sizeof(int));
	}
	srand(time(NULL));
	// relleno la matriz A haciendola simetrica con numeros aleatorios
	for(int row = 0; row < TAM; row++){
		for(int col = row; col < TAM; col++){
			matrizA[row][col] = rand()%4;
			matrizA[col][row] = matrizA[row][col];
		}
	}
	// iniciamos el proceso de paralelizacion
	double inicio = omp_get_wtime();
	#pragma omp parallel shared(matrizA, matrizB,TAM)
	{	
		#pragma omp for schedule(guided)
		for(int row = 0; row < TAM; row++){
			for(int col = row; col < TAM; col++){
				int sum = 0;
				for(int k = 0; k < TAM; k++){
					sum+=matrizA[row][k]*matrizA[k][col];
				}
				matrizB[row][col] = sum;
				matrizB[col][row] = sum;
			}
		}
	}
	double fin = omp_get_wtime();
	printf("Tiempo: %lf\n",fin-inicio);
	/*
	for(int i = 0; i < TAM; i++){
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
	}
	*/
	for (int i = 0; i < TAM; ++i){
		free(matrizA[i]);
		free(matrizB[i]);
	}
	free(matrizA);
	free(matrizB);
	return 0;
}
