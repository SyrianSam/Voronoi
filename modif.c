#include "ima.h"
#include <limits.h>
#include <stdlib.h>
#include <math.h>

// Define constants
#define MAX_COLOR_DISTANCE 765
#define N_SITES 256

// Define a structure to hold the Voronoi site data
typedef struct {
    GLubyte valvR;
    GLubyte valvG;
    GLubyte valvB;
    int position;
} Vono;

// Function to compute square of color difference
double sq2Color(double x, double y, double z) {
  return x * x + y * y + z * z;
}

// Function to generate Voronoi sites
void generateVoronoiSites(Vono* vono, Image* i, int size, FILE* fp) {
  int offset;
  GLubyte * imR, * imG, * imB;
  for(int k=0; k<N_SITES; k++){
    offset = (rand() % (size/3))*3;
    imR = i->data;
    imG = i->data +1;
    imB = i->data +2;

    vono[k].valvR = *(imR + offset);
    vono[k].valvG = *(imG + offset);
    vono[k].valvB = *(imB + offset);
    vono[k].position = offset;
    fprintf(fp, "%x %x %x\n", vono[k].valvR, vono[k].valvG, vono[k].valvB);
  }
}

// Function to find the closest Voronoi site for each pixel
void findClosestVoronoiSite(Vono* vono, Image* i, int size, FILE* fp) {
  int closestDis, rDis, gDis, bDis, dis;
  short closest;
  GLubyte * imR, * imG, * imB;
  for (int j = 0; j < size  / 3 ; j ++) {
    closestDis = MAX_COLOR_DISTANCE;
    imR = i->data;
    imG = i->data +1;
    imB = i->data +2;

    for(int l=0; l<N_SITES; l++){
      rDis = abs((*imR) - vono[l].valvR);
      gDis = abs((*imG) - vono[l].valvG);
      bDis = abs((*imB) - vono[l].valvB);
      dis = (int) sq2Color(rDis, gDis, bDis);

      if(closestDis > dis) {
        closest = l;
        closestDis = dis;
      }
    }

    fwrite(&closest, (size_t) 1, (size_t) 1, fp);
    *imR = vono[closest].valvR;
    *imG = vono[closest].valvG;
    *imB = vono[closest].valvB;

    imR += 3;
    imG += 3;
    imB += 3;
  }
}

// Main function
void gris_uniforme(Image * i) {
  char s[256];
  printf("Enter the name for the compressed file\n");
  scanf("%s", &s[0]);

  FILE *fp;
  fp = fopen(s, "wb");
  if (!fp) {
    fprintf(stderr, "Unable to open file '%s'\n", s);
    exit(1);
  }
	  int size = 3 * i->sizeY * i->sizeX;
  srand(time(NULL));

  // Create an array to hold the Voronoi sites
  Vono vono[N_SITES];

  // Generate the Voronoi sites
  generateVoronoiSites(vono, i, size, fp);

  // Find the closest Voronoi site for each pixel
  findClosestVoronoiSite(vono, i, size, fp);

  printf("done\n");
  fclose(fp);
}

