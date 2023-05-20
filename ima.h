#include <stdio.h> 
#include <stdlib.h>
#include <assert.h>

#include <GL/gl.h>	

#define N_SITES 256

struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    GLubyte *data;
};

struct Vono{    
	GLubyte valvR;
	GLubyte valvG;
	GLubyte valvB;
	int position; //stores memory offset for each point
};

typedef struct Vono Vono;
typedef struct Image Image;
typedef unsigned short utab [3][3][3];

int imageLoad_PPM(char *filename, Image *image);
void imagesave_PPM(char *filename, Image *image);
void imagesave_VONO(char *filename, Vono *vono, Image *image);
int decompression(char * filename, Image *image);
void gris_uniforme (Image *);
