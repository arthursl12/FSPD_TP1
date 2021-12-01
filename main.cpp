#include <iostream>
#include "mandel-tiles-graphic.h"
#include "cook.h"
#include "worker.h"

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
	// // int i,j,k;
	// fractal_param_t p;

	// if ((argc!=2)&&(argc!=3)) {
	// 	fprintf(stderr,"usage %s filename [color_pick]\n", argv[0] );
	// 	exit(-1);
	// } 
	// if (argc==3) {
	// 	color_pick = atoi(argv[2]);
	// } 
	// if ((input=fopen(argv[1],"r"))==NULL) {
	// 	perror("fdopen");
	// 	exit(-1);
	// }


    // std::cout << "File: " << argv[1] << ", color: " << color_pick << std::endl;
	// // init_gr();  // Essa biblioteca nao funciona com Pthreads! REMOVA
	// while (input_params(&p)!=EOF) {
	// 	fractal(&p); // No exercicio a funcao nao vai exibir nada! :-(
	// }
	// // end_gr();  // Essa biblioteca nao funciona com Pthreads! REMOVA

	pthread_structs_init();

    pthread_mutex_lock(&queue_access);
    std::shared_ptr<QUEUE_TYPE> q1 = std::make_shared<QUEUE_TYPE>();
    q1->push_back(generateEOW());
    worker_data worker_args(q1);
    pthread_mutex_unlock(&queue_access);

    // Create worker thread
    int ret;
    pthread_t worker;
    ret = pthread_create(&worker, NULL, worker_thread, (void *)&worker_args);
    if(ret){ throw "Failed to create thread"; }

    // Wait for it to finish
    void* tmp_ret;
    ret = pthread_join(worker, &tmp_ret);
    ret = *((int*) tmp_ret);
    free(tmp_ret);

    // Check sucess exit
    ret == EXIT_SUCCESS;

    // Check equal queues
    pthread_mutex_lock(&queue_access);
    QUEUE_TYPE q2;
    equalQueues(*worker_args.task_queue, *q1);
    equalQueues(*worker_args.task_queue, q2);
    pthread_mutex_unlock(&queue_access);

    worker_args.qtd_worker_jobs == 0;

    pthread_structs_destroy();

	return 0;
}