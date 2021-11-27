#pragma once
#ifndef MANDEL_TILES_GRAPHIC_H
#define MANDEL_TILES_GRAPHIC_H

#define MAXX 640
#define MAXY 480
#define MAXITER 32768

extern FILE* input; // descriptor for the list of tiles (cannot be stdin)
extern int color_pick; // decide how to choose the color palette

// params for each call to the fractal function
typedef struct {
	int left; int low;  // lower left corner in the screen
	int ires; int jres; // resolution in pixels of the area to compute
	double xmin; double ymin;   // lower left corner in domain (x,y)
	double xmax; double ymax;   // upper right corner in domain (x,y)
} fractal_param_t;

int input_params( fractal_param_t* p );
void fractal( fractal_param_t* p );

#endif  /* MANDEL_TILES_GRAPHIC_H */