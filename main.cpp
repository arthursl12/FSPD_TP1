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
    
	return 0;
}