#include <iostream>
#include "mandel-tiles-graphic.h"
#include "cook.h"
#include "worker.h"

#define N_THREADS_DEFAULT 4
#define QUEUE_SIZE -1 	
// QUEUE_SIZE = -1 => QUEUE_SIZE = MAX_QUEUE_WORKERS_RATIO * N_THREADS

/****************************************************************
 * Essa versao do programa, sequencial le a descricao de um bloco
 * de imagem do conjunto de mandelbrot por vez e faz a geracao
 * e exibicao do mesmo na janela grafica. Na versao paralela com 
 * pthreads, como indicado no enunciado, devem ser criados tres
 * tipos de threads: uma thread de entrada que le a descricao dos
 * blocos e alimenta uma fila para os trabalhadores, diversas
 * threads trabalhadoras que retiram um descritor de bloco da fila
 * e fazem o calculo da imagem e depositam um registro com os
 * dados sobre o processamento do bloco, como descrito no enunciado,
 * e uma thread final que recebe os registros sobre o processamento
 * e computa as estatisticas do sistema (que serao a unica saida
 * visivel do seu programa na versao que segue o enunciado.
 ****************************************************************/
int main (int argc, char* argv[]){
	int n_threads = -1;

	// ==== Argument Parsing ====
	if ((argc!=2)&&(argc!=3)) {
		fprintf(stderr,"usage %s filename [n_threads]\n", argv[0] );
		exit(-1);
	} 
	if (argc==3) {
		n_threads = atoi(argv[2]);
		if (n_threads > 20){
			perror("n_threads > 20");
			exit(-1);
		}
	} 
	if ((input=fopen(argv[1],"r"))==NULL) {
		perror("fdopen");
		exit(-1);
	}

	// Define number of threads, use default if not provided in parameters
	if (n_threads == -1){
		n_threads = N_THREADS_DEFAULT;
	}

	// ==== Main program (creation of cook_thread basically) ====
	pthread_structs_init();
    int ret;
    pthread_t cook;

	// Fill arguments for cook_thread (with mutex)
    pthread_mutex_lock(&queue_access);
    cook_data cook_args(n_threads, QUEUE_SIZE, argv[1]);
    pthread_mutex_unlock(&queue_access);

    // Create cook thread
    ret = pthread_create(&cook, NULL, cook_thread, (void *)&cook_args);
    if(ret){ throw "Failed to create thread"; }
    
    // Wait for it to finish
    ret = pthread_join(cook, NULL);
	
	pthread_structs_destroy();
	return 0;
}