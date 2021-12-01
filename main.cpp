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

    int N = 4;
    int queue_size = MAX_QUEUE_WORKERS_RATIO*N;

    pthread_mutex_lock(&queue_access);
    std::shared_ptr<QUEUE_TYPE> q1 = std::make_shared<QUEUE_TYPE>();
    for (int i = 0; i < queue_size; i++){
        q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    }
    pthread_mutex_unlock(&queue_access);

    // Create worker threads
    int ret;
    pthread_t* workers = (pthread_t*) malloc(N*sizeof(pthread_t));
    worker_data* workers_args = (worker_data*) malloc(N*sizeof(worker_data));
    for (int i = 0; i < N; i++){
        workers_args[i] = worker_data(q1);
    }
    for (int i = 0; i < N; i++){
        ret = pthread_create(&workers[i], NULL, worker_thread, 
                             (void*)&workers_args[i]);
        if(ret){ throw "Failed to create thread"; }
    }

    // 'Cook' refilling pot with more tasks and a final EOW
    pthread_mutex_lock(&queue_access);
    pthread_cond_wait(&cook_needed, &queue_access);
    // CHECK(q1->empty());
    // for (int i = 0; i < N; i++){
    //     CHECK(equalQueues(*(workers_args[i].task_queue), *q1));
    // }
    for (int i = 0; i < queue_size-1; i++){
        q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    }
    q1->push_back(generateEOW());
    pthread_cond_broadcast(&pot_filled);
    pthread_mutex_unlock(&queue_access);

    // Second fill, only with EOW
    pthread_mutex_lock(&queue_access);
    pthread_cond_wait(&cook_needed, &queue_access);
    for (int i = 0; i < N-1; i++){
        q1->push_back(generateEOW());
    }
    pthread_cond_broadcast(&pot_filled);
    pthread_mutex_unlock(&queue_access);

    // Wait for them to finish
    // sleep(1);

    for (int i = 0; i < N; i++){
        void* tmp_ret;
        ret = pthread_join(workers[i], &tmp_ret);
        ret = *((int*) tmp_ret);
        // CHECK(ret == EXIT_SUCCESS);
        free(tmp_ret);
    }
    

    pthread_mutex_lock(&queue_access);
    QUEUE_TYPE q2;
    // CHECK(q1->empty());
    // for (int i = 0; i < N; i++){
    //     CHECK(equalQueues(*(workers_args[i].task_queue), *q1));
    //     CHECK(equalQueues(*(workers_args[i].task_queue), q2));
    // }
    pthread_mutex_unlock(&queue_access);

    int total_works_done = 0;
    for (int i = 0; i < N; i++){
        total_works_done += workers_args[i].qtd_worker_jobs;
        std::cout << workers[i] << ": " << workers_args[i].qtd_worker_jobs << std::endl;
        // CHECK(workers_args[i].qtd_worker_jobs <= 10);
    }
    // CHECK(total_works_done == 10);

    pthread_structs_destroy();
    free(workers);
    free(workers_args);

	return 0;
}