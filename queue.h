#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <queue>

#include "mandel-tiles-graphic.h"
typedef struct node {
    fractal_param_t* data;
	struct node *previous;  /* pointer to previous element */
}Node;

typedef struct QueueStruct Queue;

#endif  /* QUEUE_H */