#include <unistd.h>     
#include <math.h>
#include <GL/glut.h>
#include <GL/glu.h>	
#include "ima.h"

Image *image;
Vono *vono;

#define ESCAPE 27

void Keyboard(unsigned char key, int x, int y)  {
  switch(key){
  case ESCAPE :
    exit(0);                   
    break;
  default:
    fprintf(stderr, "Unused key\n");
  }
}

void Mouse(int button, int state, int x, int y) {

  switch(button){
  case GLUT_LEFT_BUTTON:
  case GLUT_MIDDLE_BUTTON:
  case GLUT_RIGHT_BUTTON:
    break;    
  }
  glutPostRedisplay();
}

void InitializeGL() {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void InitializeWindow(int sizeX, int sizeY) {
  glutReshapeWindow(sizeX, sizeY);
}

int Init(char *s) {
  image = (Image *) malloc(sizeof(Image));
  vono = (Vono *) malloc(sizeof(Vono)*N_SITES);
  if (image == NULL) {
    fprintf(stderr, "Out of memory\n");
    return(-1);
  }
  if (imageLoad_PPM(s, image)==-1) 
	return(-1);
  printf("tailles %d %d\n",(int) image->sizeX, (int) image->sizeY);

  InitializeGL();
  InitializeWindow(image->sizeX, image->sizeY);

  return (0);
}

int ReInit() {
  InitializeGL();
  InitializeWindow(image->sizeX, image->sizeY);

  return (0);
}

void Display(void) {
  
  GLint w, h;

  glClear(GL_COLOR_BUFFER_BIT);

  w = glutGet(GLUT_WINDOW_WIDTH);
  h = glutGet(GLUT_WINDOW_HEIGHT);
  glDrawPixels(image->sizeX, image->sizeY, GL_RGB, GL_UNSIGNED_BYTE, 
	       image->data);

  glFlush();
}


void Reshape(int w, int h) {
  glViewport(0, 0, (GLsizei)w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble)h);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void menuFunc(int item) {
  char s[256];

  switch(item){
  case 0:
    printf("the image will be compressed and stored in a file\n");
    gris_uniforme(image);  // compression function
    Display();
    break;
  case 1:
    printf("Please enter the name of a compressed image file \n");
    scanf("%s" , &s[0]);
    decompression(s, image);  //decompression function
    Display();
    break;
  case 2:
  case 3:
  case 4:
  case 5:
    printf("Enter the name for the image of this size\n");
    scanf("%s", &s[0]);
    imagesave_PPM(s, image);
    break;
  case 6:
    printf("Image size: %ld %ld\n", (int) image->sizeX, (int) image->sizeY);
    break;
  default:
    break;
  }
}

void InitGLUT(int *argc, char **argv) {
  glutInit(argc, argv); 
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  glutInitWindowSize(640,480);  
  glutInitWindowPosition(100, 100);  
  glutCreateWindow("VPUP8");  
}

void SetupGLUTCallbacks() {
  glutDisplayFunc(Display);  
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Keyboard);
  glutMouseFunc(Mouse);
}

void SetupGLUTMenu() {
  glutCreateMenu(menuFunc);
  glutAddMenuEntry("Quit", 0);
  glutAddMenuEntry("gris", 1);
  glutAddMenuEntry("Gris", 2);
  glutAddMenuEntry("GRIS", 3);
  glutAddMenuEntry("grey", 4);
  glutAddMenuEntry("Save", 5);
  glutAddMenuEntry("Information", 6);
  glutAttachMenu(GLUT_LEFT_BUTTON);
}

int main(int argc, char **argv) {  

  if (argc<2) {
    fprintf(stderr, "Usage : palette file_name\n");
    exit(0);
  }

  InitGLUT(&argc, argv);
  Init(argv[1]);
  SetupGLUTCallbacks();
  SetupGLUTMenu();
  glutMainLoop();  

  return 1;
}
