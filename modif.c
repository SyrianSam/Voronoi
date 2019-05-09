/* il s agit de prendre une image et de la modifier */

#include "ima.h"
#include <limits.h>
#include <stdlib.h>
#include <math.h>

 double sq2(double x, double y)
{
	return x * x + y * y;
}

 double sq2Color(double x, double y, double z)
{
  return x * x + y * y + z * z;
}


void gris_uniforme(Image * i) {
	char s[256];
	printf("Entrer le nom pour le fichier compresser\n");
    scanf("%s", &s[0]);


  int j,k,l = 0, size;
  GLubyte * imR, * imG, * imB, valR, valG, valB;


   FILE *fp;
    //open file for output
    fp = fopen(s, "wb");
    if (!fp) {
         fprintf(stderr, "Unable to open file '%s'\n", s);
         exit(1);
    }
   //memDis, xDis, yDis,
  int  dis, closestDis, rDis, gDis, bDis;

  short closest;
  

  size =  3 * i->sizeY * i->sizeX;

	srand(time(NULL));

	Vono vono[N_SITES];


	for(k=0; k<N_SITES; k++){

	// generate N_SITES amount of random points, limited by the size	
	//divide and multiply by 3 to make sure the offset is always on the Red byte
  int offset =  (rand() % (size/3))*3;
  
  
  imR =  i->data ;   //pointer to first glubyte (red) 
  imG =  i->data +1; 
  imB =  i->data +2;  

  //assign the color values of the random point to the variables
  valR = * (imR + offset); 
  valG = * (imG + offset);
  valB = * (imB + offset);

  //store the RGB color values and position (offset) in the vonoroi array (aka the cLUT)
    vono[k].valvR = valR;
    vono[k].valvG = valG;
  	vono[k].valvB = valB;
    vono[k].position =  offset;  //no longer needed after modification

  
    // debug
  // printf("Vr %d Vg %d Vb %d \n", vono[k].valvR, 
  // 	vono[k].valvG, vono[k].valvB);


    //print the CLUT to a file
    fprintf(fp, "%x %x %x\n", vono[k].valvR, 
  	vono[k].valvG, vono[k].valvB);
	 
		}

     for (j = 0; j < size  / 3 ; j ++) {
  
  closestDis = sq2Color(255, 255, 255); //reset the closestDis variable for each pixel

      for(l=0; l<N_SITES; l++){
      //check color distance between the current pixel and our 256 stored colors 


        rDis = abs((* imR) - vono[l].valvR);
        gDis = abs((* imG) - vono[l].valvG);
        bDis = abs((* imB) - vono[l].valvB);


      dis = (int) sq2Color(rDis, gDis, bDis);  

   if( closestDis > dis){
      closest = l;
      closestDis = dis;

      }
}

      // pixel data
      //fwrite(img->data, (size_t) 1, (size_t) (3 * img->sizeX * img->sizeY), fp);
      fwrite(&closest, (size_t) 1, (size_t) 1, fp);
      //fprintf(fp, "%x\n", closest); // prints the cLUT index for each pixel into our compressed file
      
      
      *imR = vono[closest].valvR;
     imR += 3;
     *imG = vono[closest].valvG;
     imG += 3;
       *imB = vono[closest].valvB;
     imB += 3; 

   
      }

      printf("done\n");

		  fclose(fp);
	}

/*

void compression(Image * i) {
  char s[256];
  printf("Entrer le nom pour le fichier compresser\n");
    scanf("%s", &s[0]);


  int j,k,l = 0, size;
  GLubyte * imR, * imG, * imB, valR, valG, valB;


   FILE *fp;
    //open file for output
    fp = fopen(s, "wb");
    if (!fp) {
         fprintf(stderr, "Unable to open file '%s'\n", s);
         exit(1);
    }

  int memDis, xDis, yDis, dis, closestDis, rDis, gDis, bDis;

  short closest;
  

  size =  3 * i->sizeY * i->sizeX;

  srand(time(NULL));

  Vono vono[N_SITES];


  for(k=0; k<N_SITES; k++){

  // generate N_SITES amount of random points, limited by the size  
  //divide and multiply by 3 to make sure the offset is always on the Red byte
  int offset =  (rand() % (size/3))*3;
  
  
  imR =  i->data ;   //pointer to first glubyte (red) 
  imG =  i->data +1; 
  imB =  i->data +2;  

  //assign the color values of the random point to the variables
  valR = * (imR + offset); 
  valG = * (imG + offset);
  valB = * (imB + offset);

  //store the RGB color values and position (offset) in the vonoroi array (aka the cLUT)
    vono[k].valvR = valR;
    vono[k].valvG = valG;
    vono[k].valvB = valB;
    vono[k].position =  offset;  //no longer needed after modification

  
    // debug
  // printf("Vr %d Vg %d Vb %d \n", vono[k].valvR, 
  //  vono[k].valvG, vono[k].valvB);


    //print the CLUT to a file
    fprintf(fp, "%x %x %x\n", vono[k].valvR, 
    vono[k].valvG, vono[k].valvB);
   
    }

     for (j = 0; j < size  / 3 ; j ++) {
  
  closestDis = sq2Color(255, 255, 255); //reset the closestDis

      for(l=0; l<N_SITES; l++){
      //check color distance between the current pixel and our 256 stored colors 


        rDis = abs((* imR) - vono[l].valvR);
        gDis = abs((* imG) - vono[l].valvG);
        bDis = abs((* imB) - vono[l].valvB);


      dis = (int) sq2Color(rDis, gDis, bDis);  

   if( closestDis > dis){
      closest = l;
      closestDis = dis;

      }
}


      fprintf(fp, "%x\n", closest); // prints an index for each pixel
      // (decompression) assign color value of closest vonoroi point to the current pixel

      *imR = vono[closest].valvR;
    imR += 3;
    *imG = vono[closest].valvG;
    imG += 3;
      *imB = vono[closest].valvB;
    imB += 3; 

   
      }

      printf("done\n");

      fclose(fp);
  }


void rangeFinder(Image * i) {
  char s[256];
 printf("Entrer le nom pour le fichier compresser\n");
    scanf("%s", &s[0]);


  int j,k,l = 0, size;
  GLubyte * imR, * imG, * imB, valR, valG, valB;

   size =  3 * i->sizeY * i->sizeX;

  int  rcLUT[size/3];
  int  gcLUT[size/3];
  int  bcLUT[size/3];
   FILE *fp;
    //open file for output
    fp = fopen(s, "wb");
    if (!fp) {
         fprintf(stderr, "Unable to open file '%s'\n", s);
         exit(1);
    }

  int memDis, xDis, yDis, dis, closestDis, closest;
  GLubyte maxR, maxG,maxB = 0;
  GLubyte minR, minG,minB = 255;

 

  srand(time(NULL));

  Vono vono[size];

  for (k = 0; k < size/6; k++){

  for(j=0; j<size ;j += 3){

     imR =  i->data +j;  
     imG =  i->data +1 +j; 
     imB =  i->data +2 +j;  

     if (*imR + *imG + *imB > maxR + maxG + maxB){
        maxR = *imR;
         maxG = *imG;
          maxB = *imB;
     }

     if (*imR + *imG + *imB < minR + minG + minB){
        minR = *imR;
         minG = *imG;
          minB = *imB;
     }





  }

       rcLUT[size/3 -k] = minR;
       gcLUT[size/3 -k] = minG;
       bcLUT[size/3 -k] = minB;

       rcLUT[k] = maxR;
       gcLUT[k] = maxG;
       bcLUT[k] = maxB;




}


//  distribute 256 colors within the min max range ()


   printf("min %d %d %d \n max %d %d %d \n", minR, minG, minB, maxR, maxG, maxB );

   //set up the poles
 


   // generate a CLUT within the min-max color range



  float stepR = (float)(maxR - minR)  * 1000 /254;
  float stepG = (float)(maxG - minG) * 1000 /254;
  float stepB = (float)(maxB - minB) * 1000 /254;

  int rCounter =  (int) (stepR) ;
  int gCounter =   (int)(stepG );
  int bCounter =  (int) (stepB );

printf("counters %d %d %d \n", rCounter, gCounter, bCounter );



  for(k=1;k<255;k++)
  {

    

     if (rCounter > 1000)
     {
      rcLUT[k] =  (int) rcLUT[k-1] + 1;
      rCounter -= 1000;
     }else{
      rcLUT[k] =   (int) rcLUT[k-1];
     }
     if (gCounter > 1000)
     {
      gcLUT[k] =  (int) gcLUT[k-1] + 1 ;
     gCounter -= 1000;
     }else{
      gcLUT[k] =   (int) gcLUT[k-1];
     }
     if (bCounter > 1000)
     {
      bcLUT[k]  =  bcLUT[k-1] +1;
     bCounter -= 1000;
    }else{
      bcLUT[k] =  (int) bcLUT[k-1];
     }

     //debug

    printf("counters %d %d %d \n", rCounter, gCounter, bCounter );


     printf("cLUT %d %d %d \n",  rcLUT[k] ,  gcLUT[k]  , bcLUT[k]  );

      rCounter += stepR ;
     gCounter += stepG ;
     bCounter += stepB ;

  }




   printf("steps %f %f %f \n", stepR, stepG, stepB );



 
 

      fclose(fp);
  }
*/