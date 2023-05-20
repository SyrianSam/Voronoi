#include "ima.h"

#define INSPIREPAR "RPFELGUEIRAS"
#define CREATOR "JJ"
#define RGB_COMPONENT_COLOR 255

int decompression(char *filename, Image *img);
int imageLoad_PPM(char *filename, Image *img);
void imagesave_PPM(char *filename, Image *img);
void imagesave_VONO(char *filename, Vono *vono, Image *img);

int decompression(char *filename, Image *img) {
    FILE *fp;
    int i, j, l = 0, size;
    GLubyte *imR, *imG, *imB;

    fp = fopen(filename, "rb");

    Vono vono[N_SITES];
    int index;

    /* allocation memoire */
    size = img->sizeX * img->sizeY * 3;
    printf("Size image %lu %lu => %d\n", img->sizeX, img->sizeY, size);
    img->data = (GLubyte *) malloc((size_t) size * 2 * sizeof(GLubyte));
    assert(img->data);

    for (i = 0; i < N_SITES; i++) {
        fscanf(fp, "%x %x %x\n", &vono[i].valvR, &vono[i].valvG, &vono[i].valvB);
    }

    imR = img->data;
    imG = img->data + 1;
    imB = img->data + 2;

    //fill the image
    for (j = 0; j < size / 3; j++) {
        fread(&index, (size_t) 1, (size_t) 1, fp);

        *imR = vono[index].valvR;
        imR += 3;
        *imG = vono[index].valvG;
        imG += 3;
        *imB = vono[index].valvB;
        imB += 3;
    }

    fclose(fp);
    return 1;
}

int imageLoad_PPM(char *filename, Image *img) {
    char d, buff[16];
    FILE *fp;
    int b, c, rgb_comp_color, size, sizex, sizey;
    GLubyte tmp, *ptrdeb, *ptrfin, *lastline;

    fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "Unable to open file '%s'\n", filename);
        exit(1);
    }

    if (!fgets(buff, sizeof(buff), fp)) {
        perror(filename);
        exit(1);
    }

    if (buff[0] != 'P' || buff[1] != '6') {
        fprintf(stderr, "Invalid image format (must be 'P6')\n");
        exit(1);
    }

    c = getc(fp);
    while (c == '#') {
        while (getc(fp) != '\n')
            ;
        c = getc(fp);
    }
    ungetc(c, fp);

    if (fscanf(fp, "%lu %lu", &img->sizeX, &img->sizeY) != 2) {
        fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
        exit(1);
    }

    if (fscanf(fp, "%d", &rgb_comp_color) != 1) {
        fprintf(stderr, "Invalid rgb component (error loading '%s')\n", filename);
        exit(1);
    }
    fscanf(fp, "%c ", &d);

    if (rgb_comp_color != RGB_COMPONENT_COLOR) {
        fprintf(stderr, "'%s' does not have 8-bits components\n", filename);
        exit(1);
    }

    /* allocation memoire */
    size = img->sizeX * img->sizeY * 3;
    printf("Size image %lu %lu => %d\n", img->sizeX, img->sizeY, size);
    img->data = (GLubyte *) malloc((size_t) size * sizeof(GLubyte));
    assert(img->data);

    if (fread(img->data, (size_t) 1, (size_t) size, fp) == 0) {
        fprintf(stderr, "Error loading image '%s'\n", filename);
    }

    sizex = img->sizeX;
    sizey = img->sizeY;
    lastline = img->data + size - sizex * 3;

    for (b = 0; b < img->sizeY / 2; b++) {
        ptrdeb = img->data + b * sizex * 3;
        ptrfin = lastline - (b * sizex * 3);
        for (c = 0; c < 3 * sizex; c++) {
            tmp = *ptrdeb;
            *ptrdeb = *ptrfin;
            *ptrfin = tmp;
            ptrfin++;
            ptrdeb++;
        }
    }

    int red, green, blue, alpha;
    unsigned int pixel = *img->data;
    blue = (pixel >> 16) & 0xff;
    green = (pixel >> 8) & 0xff;
    red = pixel & 0xff;
    printf("red %d green %d blue %d  \n", red, green, blue);
    printf("%ld => %ld\n %u \n", (int) img->data, (int) lastline, pixel);

    fclose(fp);
    return 1;
}

void imagesave_PPM(char *filename, Image *img) {
    FILE *fp;
    fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Unable to open file '%s'\n", filename);
        exit(1);
    }

    fprintf(fp, "P6\n");
    fprintf(fp, "# Created by %s\n", CREATOR);
    fprintf(fp, "%lu %lu\n", img->sizeX, img->sizeY);
    fprintf(fp, "%d\n", RGB_COMPONENT_COLOR);
    fwrite(img->data, (size_t) 1, (size_t) (3 * img->sizeX * img->sizeY), fp);
    fclose(fp);
}

void imagesave_VONO(char *filename, Vono *vono, Image *img) {
    FILE *fp;
    fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Unable to open file '%s'\n", filename);
        exit(1);
    }

    fprintf(fp, "V6\n");
    fprintf(fp, "# Created by %s\n", CREATOR);
    fprintf(fp, "%lu %lu\n", img->sizeX, img->sizeY);
    fprintf(fp, "%d\n", RGB_COMPONENT_COLOR);

    for (int k = 0; k < N_SITES; k++) {
        printf("Vr %d Vg %d Vb %d pos %d  \n", vono[k].valvR, vono[k].valvG, vono[k].valvB, vono[k].position);
    }

    fwrite(&vono[0], 1, sizeof(&vono) * N_SITES, fp);
    fclose(fp);
}
