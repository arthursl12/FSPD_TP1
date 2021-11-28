#include <iostream>
#include "mandel-tiles-graphic.h"
#include "tasks.h"

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

	// Initialize pthread structures
    // pthread_cond_t cook_needed;
    // pthread_cond_t pot_filled;
    // pthread_mutex_t queue_access;

    pthread_cond_init (&cook_needed, NULL);
    pthread_cond_init (&pot_filled, NULL);
    pthread_mutex_init(&queue_access, NULL);

    int ret;
    pthread_t cook;
    cook_data* cook_args = (cook_data*) malloc(sizeof(cook_data));
	pthread_attr_t attr;

	/* Initialize and set thread detached attribute */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


    // Fill arguments
    pthread_mutex_lock(&queue_access);
    cook_args->task_queue = NULL;
    cook_args->n_threads = 0;
    cook_args->queue_size = 5;
    cook_args->filename = "mandelbrot_tasks/t";


    // CHECK(cook_args.task_queue->empty());
    pthread_mutex_unlock(&queue_access);

    // Create cook thread
    ret = pthread_create(&cook, &attr, cook_thread, (void *)cook_args);
    if(ret){ std::cout << "RET ruim!" << std::endl; }
    
	void* tmp;
	ret = pthread_join(cook, &tmp);
	QUEUE_TYPE* internal_queue = (QUEUE_TYPE*) tmp;
	// cook_args = (cook_data*) tmp;
    if(ret){ std::cout << "RET ruim!" << std::endl; }

    pthread_mutex_lock(&queue_access);
	std::cout << "Queue (outside): " << internal_queue->size() << std::endl;
    // CHECK_FALSE(cook_args.task_queue->empty());
    // CHECK(cook_args.created_tasks == 5);
    // CHECK(cook_args.completed_tasks == 0);
    pthread_mutex_unlock(&queue_access);

    std::cout << "New? : " << cook_args->filename << std::endl;

    pthread_attr_destroy(&attr);
	pthread_cond_destroy(&pot_filled);
	pthread_cond_destroy(&cook_needed);
	pthread_mutex_destroy(&queue_access);
	free(cook_args);
	delete(internal_queue);
    // pthread_cancel(cook);

	return 0;
}