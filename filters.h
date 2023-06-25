
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
unsigned char* negative(unsigned char *start,const unsigned char *end,int x,int y,int channels);
unsigned char* invert(unsigned char *start,const unsigned char *end,int x,int y,int channels);
BMPImage* bmp_invert(BMPImage* image);
BMPImage * bmp_negative(BMPImage* image);
unsigned char * gaussian_filter(unsigned char* src, int width, int height, int channels, double sigma);
unsigned char* laplacian_filter(unsigned char *image, int rows, int cols,int channels);
unsigned char* mean_filter(unsigned char *start,int x,int y,int channels);
unsigned char* reflect_filter(unsigned char *image, int x, int y,int channels);