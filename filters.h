
typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
}RGB;

typedef struct {
    int width;
    int height;
    RGB * data;
}BMPImage;

#ifndef PROJECT_FILTERS_H
#define PROJECT_FILTERS_H

#endif //PROJECT_FILTERS_H

unsigned char* gray(unsigned char *start,const unsigned char *end,int x,int y,int channels);
BMPImage *bmp_gray(BMPImage* image);
unsigned char* sepia(unsigned char *start,const unsigned char *end,int x,int y,int channels);
BMPImage * bmp_sepia(BMPImage* image);
void reflect(unsigned char *start,int size,int x,int y,int channels);
int find_index(int j,int i,int k,int x,int channels);
unsigned char *blur(unsigned char *start,const unsigned char *end,int x,int y,int channels);
unsigned char* invert(unsigned char *start,const unsigned char *end,int x,int y,int channels);