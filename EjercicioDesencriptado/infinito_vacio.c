#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

struct dobleLetra
{
	char *texto;
	int rep_times;
};

void ordenarBurbujaStructs(struct dobleLetra*, int);

int main(int argc, char const *argv[])
{
	if(argc > 1){
		//Obtenemos el archivo pasado por comando
		FILE *fich;
	 	fich=fopen(argv[1],"r");
	 	if(fich!=NULL){
		 	//Diccionarios uno para saber que letra le corresponde y el otro para la inversa
			//char diccionario_provisional[]={'-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-'};
			//char diccionario_provisional_invertido[]={'-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-'};
			// array con tantas posiciones como letras en el abecedario para contar cuantas veces se repiten
			// su indice sera el caracter-97 de la tabla ascii
			int letras_repetidas[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
			// obtenemos el fichero que hemos pasado como parametro para su ejecucion
	 		// nos ponemos al final del fichero
		 	fseek(fich, 0L, SEEK_END);
		 	// y obtenemos el tamaño que tiene
		 	int tam_fichero = ftell(fich);
		 	// creo un array de caracteres tan grande como el tamaño del texto
		 	char txt[tam_fichero];
		 	// nos posicionamos al principio del texto nuevamente
		 	fseek(fich, 0L, SEEK_SET);
		 	for (int i = 0; i < tam_fichero; ++i)
		 	{
		 		// leemos por completo el archivo
		 		txt[i] = fgetc(fich);
		 	}

		 	//Struct para guardar las letras, las que encuentre dobles y cada letra por separado
		 	// las guardo en un struct para poder aplicar el metodo de la burbuja, teniendo la letra
		 	// y las veces que se repite
		 	char *dobleLetra = (char*) malloc(52*sizeof(char));
		 	struct dobleLetra dobleLetraArray[26];
		 	struct dobleLetra singleLetraArray[26];
		 	for(int i = 0; i < 26; i++){
		 		//reservo la memoria para que no me falte en ningun momento
		 		singleLetraArray[i].texto = (char*) malloc(sizeof(char));
		 		singleLetraArray[i].rep_times=0;
		 		dobleLetraArray[i].texto = (char*) malloc(sizeof(char));
		 		dobleLetraArray[i].rep_times=0;

		 	}
		 	// Conjunto de variables para poder controlar si encontramos dos letras iguales juntas
		 	// y si al encontrarlas fue la primera para asignarle la l automaticamente
		 	char letraAnterior=0;
		 	int nuevaPalabra=0;
		 	int indexDobleLetra=0;
		 	int dobleL=-1;

		 	// me aseguro el tamaño de lo leido del fichero
			int TAM_TXT = strlen(txt);
			// Array de un tamaño tan grande como el texto
			// aqui guardaremos el texto con solo caracteres de a-z y con espacios
			char format_txt[TAM_TXT];
			// puntero a char para guardar palabras
			char *word;
			// indices de los arrays, usaremos la cantidad letras en el texto para comprobar estadisticas
			int char_txt=0, char_word=0, words_in_text=0;
			// puntero para asignarle el valor de los caracteres que vamos leyendo
			char c[1];
			// Tamaño del array de Palabras de diferente longitud con las que vamos a
			// querer tener un control especial
			int cantidad_longitudes_diferentes = 5;
			// array para saber cuantas palabras encontramos de cada longitud que queremos tratar
			int cantidadPalabrasPorNumLetras[cantidad_longitudes_diferentes];
			for(int i = 0; i < cantidad_longitudes_diferentes;i++){
				// inicializamos a 0 el array que indicara cuantas palabras de cada tamaño existen
				cantidadPalabrasPorNumLetras[i]=0;
			}

			// Matriz con lineas de texto para comprobar si una palabra ya fue clasificada
			char **lineaPalabras = (char **) malloc(sizeof(char*)*cantidad_longitudes_diferentes);
			for(int i = 0; i < cantidad_longitudes_diferentes; i++){
				// Inicializamos el cada fila de la matriz con el tamaño del texto para no quedarnos cortos nunca
				lineaPalabras[i]=(char *) malloc(sizeof(char)*TAM_TXT);
			}
			
			//Filtramos el texto para quedarnos unicamente con palabras separadas por espacio
			// vamos contando cuantras palabras hay en el texto
			// char_txt se mueve a lo largo del texto hasta que el indice devuelva un char '/0'
			for(*c=txt[char_txt];*c!='\0';*c=txt[char_txt]){
				// cogemos solo de la a 'a' a la 'z'
				if(*c>96 && *c<123){
					// Con este if comprobaremos si es una doble letra y trataremos ese caso
					if(letraAnterior==*c){
						// si la doble letra es las dos primeras es una ll
						if(nuevaPalabra == 1){
							// Guardo el caracter que corresponde a la l
							dobleL=*c-97;
						}
						// buscamos esa letra si ya a sido asignada anteriormente
						char *posicionDobleLetra = strstr(dobleLetra,c);
						if(posicionDobleLetra==NULL){
							dobleLetra = strcat(dobleLetra,c);
							dobleLetra = strcat(dobleLetra," ");
							dobleLetraArray[indexDobleLetra].texto[0]=*c;
							dobleLetraArray[indexDobleLetra].rep_times=1;
							indexDobleLetra++;
						}else{
							// en el caso de que fuera asignada sabemos en que posicion del array ya insertamos
							// ya que restando a la posicion de memoria en la que se encontraba dentro 
							// de la linea del texto, la posicion del inicio de dicha cadena
							// dividimo por el 2 ya que es un caracter y un espacio y nos da el indice
							dobleLetraArray[(posicionDobleLetra-dobleLetra)/2].rep_times++;
						}
					}
					// asignamos el caracter que estamos leyendo a nuestra cadena con todo el texto
					format_txt[char_txt] = *c;
					// obtenemos el al cual le vamos a asignar dicha letra
					int n = (*c)-'a';
					//filtramos cada letra y la usamos como indice para ver cual es la mas repetida
					letras_repetidas[n]++;
					singleLetraArray[n].texto[0]=*c;
					singleLetraArray[n].rep_times++;
					//mientras sea una nueva palabra le añadiremos ++ para ver si es doble ll o no
					nuevaPalabra++;
				}else{
					format_txt[char_txt] = ' ';
					words_in_text++;
					nuevaPalabra=0;
				}
				char_txt++;
				// guardamos la letra anterior para las dobles letras
				letraAnterior = *c;
			}
			// Creamos una matriz con tantas filas como longitudes queramos tratar
			// y a cada fila le asignamos un espacio tan grande como palabras hay en el
			struct dobleLetra words_array[cantidad_longitudes_diferentes][words_in_text];
			struct dobleLetra palabra;
			// con strtok cogeremos cada palabra del texto ya que lo preparamos separandolo unicamente por espacios
			// y me estara devolviendo cada palabra hasta que devuelva null
			for(word = strtok(format_txt," "); word != NULL;word = strtok(NULL, " ")){
				//obtenemos el tamaño de la palabra para saber conde ubicarla
				int longitudPalabra = strlen(word);
				// si la longitud de la palabra es superior a las longitudes que queremos controlar
				if(longitudPalabra>cantidad_longitudes_diferentes-1){
					if(strstr(lineaPalabras[cantidad_longitudes_diferentes-1],word) == NULL ){
						words_array[cantidad_longitudes_diferentes-1][cantidadPalabrasPorNumLetras[cantidad_longitudes_diferentes-1]].texto=word;
						cantidadPalabrasPorNumLetras[cantidad_longitudes_diferentes-1]++;
						// vamos concatenando las palabras encontradas para comprobar si se repiten o no
						lineaPalabras[cantidad_longitudes_diferentes-1] = strcat(lineaPalabras[cantidad_longitudes_diferentes-1],word);
						lineaPalabras[cantidad_longitudes_diferentes-1] = strcat(lineaPalabras[cantidad_longitudes_diferentes-1]," ");
					}
				}else{
					// puntero en el que guardaremos en que posicion encontro la palabra
					char *temp_dir_memo;
					if((temp_dir_memo = strstr(lineaPalabras[longitudPalabra-1],word)) == NULL ){
						words_array[longitudPalabra-1][cantidadPalabrasPorNumLetras[longitudPalabra-1]].texto=word;
						words_array[longitudPalabra-1][cantidadPalabrasPorNumLetras[longitudPalabra-1]].rep_times=1;
						cantidadPalabrasPorNumLetras[longitudPalabra-1]++;
						lineaPalabras[longitudPalabra-1] = strcat(lineaPalabras[longitudPalabra-1],word);
						lineaPalabras[longitudPalabra-1] = strcat(lineaPalabras[longitudPalabra-1]," ");
					}else{
						// con esto sabemos en que indice de la matriz se encuentra ya guardada la palabra
						int pos = temp_dir_memo - lineaPalabras[longitudPalabra-1];
						words_array[longitudPalabra-1][pos/(longitudPalabra+1)].rep_times++;
					}
				}
			}
			// como ya no necesitamos esta informacion la liberamos
			for(int i = 0; i < cantidad_longitudes_diferentes; i++){
				free(lineaPalabras[i]);
			}

			// usaremos el metodo de la buruja para ordenar nuestros struct por la repeticion de cada palabra
			ordenarBurbujaStructs(dobleLetraArray, indexDobleLetra);
			ordenarBurbujaStructs(singleLetraArray, 26);
			for(int i = 0; i < cantidad_longitudes_diferentes; i++){
				ordenarBurbujaStructs(words_array[i], cantidadPalabrasPorNumLetras[i]);
			}
			
			#pragma omp parallel shared(dobleL,txt,words_array,letras_repetidas,dobleLetraArray,singleLetraArray)
			{	
				char diccionario_provisional[]={'-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-'};
				char diccionario_provisional_invertido[]={'-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-'};
				if(dobleL!=0){
					diccionario_provisional[dobleL]='l';
					diccionario_provisional_invertido['l'-97]=dobleL;
				}
				int id = omp_get_thread_num();
				if(id==0){
					FILE *fichDescifrado = fopen("Descifrado_diccionario1.txt","w");
					// La de es la palabra de dos letras mas repetida
					diccionario_provisional[(words_array[1][0].texto[0])-97] = 'd';
					diccionario_provisional_invertido['d'-97] = words_array[1][0].texto[0];

					diccionario_provisional[(words_array[1][0].texto[1])-97] = 'e';
					diccionario_provisional_invertido['e'-97] = words_array[1][0].texto[1];

					// la que es la mas repetida de palabras de 3 letras
					diccionario_provisional[(words_array[2][0].texto[0])-97] = 'q';
					diccionario_provisional_invertido['q'-97] = words_array[2][0].texto[0];

					diccionario_provisional[(words_array[2][0].texto[1])-97] = 'u';
					diccionario_provisional_invertido['u'-97] = words_array[2][0].texto[1];

					// Suponiendo que la letra doble que mas se repite sea la l, comprobamos que como letra 
					// aparezca menos veces que la r, suponiendo que sea la segunda mas repetida sea la r
					if(letras_repetidas[(dobleLetraArray[0].texto[0])-97] < letras_repetidas[(dobleLetraArray[1].texto[0])-97]){
						diccionario_provisional[(dobleLetraArray[0].texto[0])-97] = 'l';
						diccionario_provisional_invertido['l'-97] = dobleLetraArray[0].texto[0];
						//asignamos r y c suponiendo que rr sea repita mas que cc comprobando que efectivamente r aparce mas que c
						if(letras_repetidas[(dobleLetraArray[1].texto[0])-97]>letras_repetidas[(dobleLetraArray[2].texto[0])-97]){
							diccionario_provisional[(dobleLetraArray[1].texto[0])-97] = 'r';
							diccionario_provisional_invertido['r'-97] = dobleLetraArray[1].texto[0];
							diccionario_provisional[(dobleLetraArray[2].texto[0])-97] = 'c';
							diccionario_provisional_invertido['c'-97] = dobleLetraArray[2].texto[0];
						}else{
							diccionario_provisional[(dobleLetraArray[1].texto[0])-97] = 'c';
							diccionario_provisional_invertido['c'-97] = dobleLetraArray[1].texto[0];
							diccionario_provisional[(dobleLetraArray[2].texto[0])-97] = 'r';
							diccionario_provisional_invertido['r'-97] = dobleLetraArray[2].texto[0];
						}
					}else{
						diccionario_provisional[(dobleLetraArray[0].texto[0])-97] = 'r';
						diccionario_provisional_invertido['r'-97] = dobleLetraArray[0].texto[0];
						diccionario_provisional[(dobleLetraArray[1].texto[0])-97] = 'l';
						diccionario_provisional_invertido['l'-97] = dobleLetraArray[1].texto[0];
					}
					// las letras que mas se repites por porcentaje
					int x=4,j=0;
					char asignadas[]={'a','o','n','i'};
					for(int i = 0; i < x;i++){
						// ampliaremos el valor de x si la que vamos a asignar ya a sido asignada, porque estas cuatro van bien
						if(diccionario_provisional[(singleLetraArray[i].texto[0])-97] == '-'){
							diccionario_provisional[(singleLetraArray[i].texto[0])-97]=asignadas[j];
							diccionario_provisional_invertido[asignadas[j]-97]=singleLetraArray[i].texto[0];
							j++;
						}else{
							x++;
						}
					}
					//buscamos la s gracias a los
					x=2;
					for(int i = 0; i < cantidadPalabrasPorNumLetras[x];i++){
						if(words_array[x][i].texto[0] == diccionario_provisional_invertido['l'-97] && words_array[x][i].texto[1] == diccionario_provisional_invertido['o'-97]){
							if(diccionario_provisional[(words_array[x][i].texto[2])-97] == '-'){
								diccionario_provisional[(words_array[x][i].texto[2])-97] = 's';
								diccionario_provisional_invertido['s'-97] = words_array[x][i].texto[2];
							}
						}
					}
					//ahora buscamos la c con la palabra con 
					x=2;
					for(int i = 0; i < cantidadPalabrasPorNumLetras[x];i++){
						if(words_array[x][i].texto[1] == diccionario_provisional_invertido['o'-97] && words_array[x][i].texto[2] == diccionario_provisional_invertido['n'-97]){
							// siempre comprbamos que esa posicion no se le haya sido asignado otro valor, para evitar errores
							if(diccionario_provisional[(words_array[x][i].texto[0])-97] == '-'){
								diccionario_provisional[(words_array[x][i].texto[0])-97] = 'c';
								diccionario_provisional_invertido['c'-97] = words_array[x][i].texto[0];
							}
						}
					}
					// la y es la letra que mas veces aparece sola
					// si no se da el caso de ser la primera, ya se habra asignado esa posicion a otra
					if(diccionario_provisional[(words_array[0][0].texto[0])-97] == '-'){
						diccionario_provisional[(words_array[0][0].texto[0])-97] = 'y';
						diccionario_provisional_invertido['y'-97] = words_array[0][0].texto[0];
					}else{
						//entonces se lo asignamos a esta
						diccionario_provisional[(words_array[0][1].texto[0])-97] = 'y';
						diccionario_provisional_invertido['y'-97] = words_array[0][1].texto[0];
					}

					// la si aparece una -or la que falta normalmente es una p
					x=2;
					for(int i = 0; i < cantidadPalabrasPorNumLetras[x];i++){
						if(words_array[x][i].texto[1] == diccionario_provisional_invertido['o'-97] && words_array[x][i].texto[2] == diccionario_provisional_invertido['r'-97]){
							diccionario_provisional[(words_array[x][i].texto[0])-97] = 'p';
							diccionario_provisional_invertido['p'-97] = words_array[x][i].texto[0];
						}
						// bastante debil, una forma de buscar la m por la palabra muy
						if(words_array[x][i].texto[1] == diccionario_provisional_invertido['u'-97] && words_array[x][i].texto[2] == diccionario_provisional_invertido['y'-97]){
							diccionario_provisional[(words_array[x][i].texto[0])-97] = 'm';
							diccionario_provisional_invertido['m'-97] = words_array[x][i].texto[0];
						}
					}
					// bucamos la m sabiendo que m va antes que p y no quedan letras sin asignar que puedan darse el caso
					for(int i = 4; i < cantidad_longitudes_diferentes; i++){
						for(int j = 0; j < cantidadPalabrasPorNumLetras[i]; j++){
							for(int z = 2; words_array[i][j].texto[z]!=0;z++){
								if(words_array[i][j].texto[z]==diccionario_provisional_invertido['p'-97] && diccionario_provisional[words_array[i][j].texto[z-1]-97]=='-'){
									if(diccionario_provisional_invertido['m'-97]=='-'){
										diccionario_provisional[words_array[i][j].texto[z-1]-97]='m';
										diccionario_provisional_invertido['m'-97]=words_array[i][j].texto[z-1];
									}
								}
							}
						}
					}
					//para esta busqueda necesitamos que la m haya sido encontrada asi k le pondremos esa condicion
					if(diccionario_provisional_invertido['m'-97]!='-'){
						for(int i = 4; i < cantidad_longitudes_diferentes; i++){
							for(int j = 0; j < cantidadPalabrasPorNumLetras[i]; j++){
								for(int z = 2; words_array[i][j].texto[z]!=0;z++){
									if(words_array[i][j].texto[z-1]==diccionario_provisional_invertido['m'-97] && diccionario_provisional[words_array[i][j].texto[z]-97]=='-'){
										diccionario_provisional[words_array[i][j].texto[z]-97]='b';
										diccionario_provisional_invertido['b'-97]=words_array[i][j].texto[z];
									}
								}
							}
						}
					}
					// en este punto ya es dificil ir sacando mas excepto la t que se repite bastante en el diccionario
					// por lo que buscaremos las que mas se repiten en el diccionario y la que no este aun asignada supondremos la t
					x=0;
					while(diccionario_provisional_invertido['t'-97]=='-'){
						if(diccionario_provisional[singleLetraArray[x].texto[0]-97]=='-'){
							diccionario_provisional[singleLetraArray[x].texto[0]-97]='t';
							diccionario_provisional_invertido['t'-97]=singleLetraArray[x].texto[0];
						}
						x++;
					}
					
					for(int i = 0; i < strlen(txt);i++){
						if(txt[i]>96 && txt[i]<123){
							// mostramos todo el texto cambiando los caracteres codificados por su equivalente
							fputc(diccionario_provisional[txt[i]-97],fichDescifrado);
						}else{
							fputc(txt[i],fichDescifrado);
						}
					}
				}else if(id==1){
					FILE *fichDescifrado = fopen("Descifrado_diccionario2.txt","w");
					// La de es la palabra de dos letras mas repetida
					diccionario_provisional[(words_array[1][0].texto[0])-97] = 'd';
					diccionario_provisional_invertido['d'-97] = words_array[1][0].texto[0];

					diccionario_provisional[(words_array[1][0].texto[1])-97] = 'e';
					diccionario_provisional_invertido['e'-97] = words_array[1][0].texto[1];

					// la que es la mas repetida de palabras de 3 letras
					diccionario_provisional[(words_array[2][0].texto[0])-97] = 'q';
					diccionario_provisional_invertido['q'-97] = words_array[2][0].texto[0];

					diccionario_provisional[(words_array[2][0].texto[1])-97] = 'u';
					diccionario_provisional_invertido['u'-97] = words_array[2][0].texto[1];

					// Suponiendo que la letra doble que mas se repite sea la l, comprobamos que como letra 
					// aparezca menos veces que la r, suponiendo que sea la segunda mas repetida sea la r
					if(letras_repetidas[(dobleLetraArray[0].texto[0])-97] < letras_repetidas[(dobleLetraArray[1].texto[0])-97]){
						diccionario_provisional[(dobleLetraArray[0].texto[0])-97] = 'l';
						diccionario_provisional_invertido['l'-97] = dobleLetraArray[0].texto[0];
						//asignamos r y c suponiendo que rr sea repita mas que cc comprobando que efectivamente r aparce mas que c
						if(letras_repetidas[(dobleLetraArray[1].texto[0])-97]>letras_repetidas[(dobleLetraArray[2].texto[0])-97]){
							diccionario_provisional[(dobleLetraArray[1].texto[0])-97] = 'r';
							diccionario_provisional_invertido['r'-97] = dobleLetraArray[1].texto[0];
							diccionario_provisional[(dobleLetraArray[2].texto[0])-97] = 'c';
							diccionario_provisional_invertido['c'-97] = dobleLetraArray[2].texto[0];
						}else{
							diccionario_provisional[(dobleLetraArray[1].texto[0])-97] = 'c';
							diccionario_provisional_invertido['c'-97] = dobleLetraArray[1].texto[0];
							diccionario_provisional[(dobleLetraArray[2].texto[0])-97] = 'r';
							diccionario_provisional_invertido['r'-97] = dobleLetraArray[2].texto[0];
						}
					}else{
						diccionario_provisional[(dobleLetraArray[0].texto[0])-97] = 'r';
						diccionario_provisional_invertido['r'-97] = dobleLetraArray[0].texto[0];
						diccionario_provisional[(dobleLetraArray[1].texto[0])-97] = 'l';
						diccionario_provisional_invertido['l'-97] = dobleLetraArray[1].texto[0];
					}
					// las letras que mas se repites por porcentaje
					int x=4,j=0;
					char asignadas[]={'a','o','s','n'};
					for(int i = 0; i < x;i++){
						if(diccionario_provisional[(singleLetraArray[i].texto[0])-97] == '-'){
							diccionario_provisional[(singleLetraArray[i].texto[0])-97]=asignadas[j];
							diccionario_provisional_invertido[asignadas[j]-97]=singleLetraArray[i].texto[0];
							j++;
						}else{
							// ampliaremos el valor de x si la que vamos a asignar ya a sido asignada, porque estas cuatro van bien
							x++;
						}
					}
					// la y es la letra que mas veces aparece sola
					if(diccionario_provisional[(words_array[0][0].texto[0])-97] == '-'){
						diccionario_provisional[(words_array[0][0].texto[0])-97] = 'y';
						diccionario_provisional_invertido['y'-97] = words_array[0][0].texto[0];
					}else{
						diccionario_provisional[(words_array[0][1].texto[0])-97] = 'y';
						diccionario_provisional['y'-97] = words_array[0][1].texto[0];
					}
					// como ya tenemos las vocales menos la i, si hay una de dos letras seguramente sera una i si una esta rellena y la otra no
					x=1;
					for(int i = 0; i < cantidadPalabrasPorNumLetras[x];i++){
						//Podriamos añadir un filtro aqui para eliminar abreviaturas de nombres controlando las veces que esta repetida
						if(diccionario_provisional[(words_array[x][i].texto[0])-97] != '-' && diccionario_provisional[(words_array[x][i].texto[1])-97] == '-'){
							diccionario_provisional[(words_array[x][i].texto[1])-97] = 'i';
							diccionario_provisional_invertido['i'-97] = words_array[x][i].texto[1];
						}
					}
					// la si aparece una -or la que falta normalmente es una p
					x=2;
					for(int i = 0; i < cantidadPalabrasPorNumLetras[x];i++){
						if(words_array[x][i].texto[1] == diccionario_provisional_invertido['o'-97] && words_array[x][i].texto[2] == diccionario_provisional_invertido['r'-97]){
							diccionario_provisional[(words_array[x][i].texto[0])-97] = 'p';
							diccionario_provisional_invertido['p'-97] = words_array[x][i].texto[0];
						}
						// bastante debil
						if(words_array[x][i].texto[1] == diccionario_provisional_invertido['u'-97] && words_array[x][i].texto[2] == diccionario_provisional_invertido['y'-97]){
							diccionario_provisional[(words_array[x][i].texto[0])-97] = 'm';
							diccionario_provisional_invertido['m'-97] = words_array[x][i].texto[0];
						}
					}
					// probamos otra forma de obtener m
					if(diccionario_provisional_invertido['m'-97] = 'm'){
						// bucamos la m sabiendo que m va antes que p y no quedan letras sin asignar que puedan darse el caso
						for(int i = 4; i < cantidad_longitudes_diferentes; i++){
							for(int j = 0; j < cantidadPalabrasPorNumLetras[i]; j++){
								for(int z = 2; words_array[i][j].texto[z]!=0;z++){
									if(words_array[i][j].texto[z]==diccionario_provisional_invertido['p'-97] && diccionario_provisional[words_array[i][j].texto[z-1]-97]=='-'){
										if(diccionario_provisional_invertido['m'-97]=='-'){
											diccionario_provisional[words_array[i][j].texto[z-1]-97]='m';
											diccionario_provisional_invertido['m'-97]=words_array[i][j].texto[z-1];
										}
									}
								}
							}
						}
					}
					//para esta busqueda necesitamos que la m haya sido encontrada asi k le pondremos esa condicion
					if(diccionario_provisional_invertido['m'-97]!='-'){
						for(int i = 4; i < cantidad_longitudes_diferentes; i++){
							for(int j = 0; j < cantidadPalabrasPorNumLetras[i]; j++){
								for(int z = 3; words_array[i][j].texto[z]!=0;z++){
									if(words_array[i][j].texto[z-2]==diccionario_provisional_invertido['m'-97]
									&& words_array[i][j].texto[z]==diccionario_provisional_invertido['r'-97]
									&& diccionario_provisional[words_array[i][j].texto[z-1]-97]=='-'){
										diccionario_provisional[words_array[i][j].texto[z-1]-97]='b';
										diccionario_provisional_invertido['b'-97]=words_array[i][j].texto[z-1];	
									}
								}
							}
						}
					}
					//en este punto ya es dificil ir sacando mas excepto la t que se repite bastante en el diccionario
					// por lo que buscaremos las que mas se repiten en el diccionario y la que no este aun asignada supondremos la t
					x=0;
					while(diccionario_provisional_invertido['t'-97]=='-'){
						if(diccionario_provisional[singleLetraArray[x].texto[0]-97]=='-'){
							diccionario_provisional[singleLetraArray[x].texto[0]-97]='t';
							diccionario_provisional_invertido['t'-97]=singleLetraArray[x].texto[0];
						}
						x++;
					}
					
					for(int i = 0; i < strlen(txt);i++){
						if(txt[i]>96 && txt[i]<123){
							// el texto resultante se almacenara en un fichero
							fputc(diccionario_provisional[txt[i]-97],fichDescifrado);
						}else{
							fputc(txt[i],fichDescifrado);
						}
					}
				}
			}
			// liberamos toda la memoria reservada
			for(int i = 0; i < 26; i++){
		 		free(singleLetraArray[i].texto);
		 		free(dobleLetraArray[i].texto);
		 	}
			free(dobleLetra);
			free(lineaPalabras);
			fclose(fich);
		}else{
			printf("Error al leer el archivo\n");
		}
	}else{
		printf("Ha de introducir el nombre del archivo en la ejecucion del programa\n");
	}
	printf("Finalizado, puede encontrar el posible resultado en los txt generados.\n");
	return 0;
}

//metodo para ordenar strucs de DoblesLetras
void ordenarBurbujaStructs(struct dobleLetra dobleLetraArray[], int indexDobleLetra){
	struct dobleLetra temp;
	for(int i = 1; i < indexDobleLetra; i++){
		for(int j = 0; j < (indexDobleLetra-i); j++){
			if(dobleLetraArray[j].rep_times < dobleLetraArray[j+1].rep_times){
				temp = dobleLetraArray[j];
				dobleLetraArray[j] = dobleLetraArray[j+1];
				dobleLetraArray[j+1] = temp;
			}
		}
	}
}