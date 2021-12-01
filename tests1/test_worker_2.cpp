#include "doctest.h"

#include "mandel-tiles-graphic.h"
#include "parsing.h"
#include "cook.h"
#include "worker.h"

#include <pthread.h>
#include <queue>
#include <unistd.h>
#include <signal.h>
#include <iostream>

/*
Only more jobs after refill

More EOW after refill (two refills needed)
Unbalanced load
*/


