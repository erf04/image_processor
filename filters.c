#include "filters.h"
#include "stdlib.h"
#include "math.h"
#include "string.h"
#include "stdio.h"
unsigned char* gray(unsigned char *start,const unsigned char *end,int x,int y,int channels){
    unsigned int sum,average;
    int gray_channels;
    if (channels==4)
        gray_channels=2;
    else
        gray_channels=1;
    unsigned char *pixel;unsigned char *gray_pixel;
    unsigned char *gray_image_start= malloc(gray_channels*x*y);
    for (pixel=start,gray_pixel=gray_image_start;pixel!=end;pixel+=channels,gray_pixel+=gray_channels){
        sum= *pixel+ *(pixel+1)+ *(pixel+2);
        average=(unsigned int)(sum/3);
        *gray_pixel=average;
        if (channels==4)
            *(gray_pixel+1)=*(pixel+3);
    }
    return gray_image_start;



}

BMPImage *bmp_gray(BMPImage* image){
    BMPImage* copy=(BMPImage*) malloc(sizeof (BMPImage));
    copy->width=image->width;
    copy->height=image->height;
    copy->data=image->data;
    for (int i=0;i< copy->height * copy->width;i++){
        RGB pixel=copy->data[i];
        unsigned char gray=(pixel.r+pixel.g+pixel.b)/3;
        pixel.r=gray;pixel.g=gray;pixel.b=gray;
        copy->data[i]=pixel;
    }
    return copy;
}

unsigned char* sepia(unsigned char *start,const unsigned char *end,int x,int y,int channels)
{
    unsigned char *pixel;unsigned char *sepia_image_start= malloc(3*x*y);unsigned char *sepia_pixel;
    unsigned int r,g,b;
    for (pixel=start,sepia_pixel=sepia_image_start;pixel!=end;pixel+=channels,sepia_pixel+=channels){
        r=(int)(0.393*(*pixel) + 0.769*(*(pixel+1)) + 0.189*(*(pixel+2)));
        g=(int)(0.349*(*pixel) + 0.686*(*(pixel+1)) + 0.168*(*(pixel+2)));
        b=(int )(0.272*(*pixel) + 0.534*(*(pixel+1)) + 0.131*(*(pixel+2)));
        if(r>255)
            r=255;
        *sepia_pixel=r;
        if (g>255)
            g=255;
        *(sepia_pixel+1)=g;
        if (b>255)
            b=255;
        *(sepia_pixel+2)=b;
    }
    return sepia_image_start;
    //stbi_write_jpg("C:\\Users\\ASUS\\CLionProjects\\project\\cmake-build-debug\\out_image.jpg",x,y,channels,sepia_image_start,100);

}

BMPImage * bmp_sepia(BMPImage* image){
    BMPImage* copy=(BMPImage*) malloc(sizeof (BMPImage));
    copy->width=image->width;
    copy->height=image->height;
    copy->data=image->data;
    for (int i=0;i< copy->height * copy->width;i++){
        RGB pixel=copy->data[i];
        pixel.r=(int)((0.393*pixel.r) + (0.769* pixel.g) + (0.189*pixel.b));
        pixel.g=(int)((0.349*pixel.r) + ( 0.686* pixel.g) + (0.168*pixel.b));
        pixel.b=(int)((0.272*pixel.r) + (0.534* pixel.g) + (0.131*pixel.b));
        if (pixel.r>255)
            pixel.r=255;
        if (pixel.g>255)
            pixel.g=255;
        if (pixel.b>255)
            pixel.b=255;
        copy->data[i]=pixel;
    }
    return copy;
}

/*void reflect(unsigned char *start,unsigned char * end,int x,int y,int channels){

    unsigned char *ref_pixel;unsigned char *pixel;unsigned char *ref_image_start= malloc(3*x*y);
    for (pixel=start,ref_pixel=ref_image_start;pixel<end/2;pixel+=channels,ref_pixel+=channels){
        *ref_pixel=(start+size-pixel);*(ref_pixel+1)=(start+size-pixel+1);*(ref_pixel+2)=(start+size-pixel+2);
    }
    //stbi_write_jpg("C:\\Users\\ASUS\\CLionProjects\\project\\cmake-build-debug\\out_image.jpg",x,y,channels,ref_image_start,100);

}
*/
unsigned char *reflect_filter(unsigned char *image, int x, int y,int channels) {
    int i, j;
    unsigned char* output= malloc(x*y* channels);
    for (i = 0; i < y; i++) {
        for (j = 0; j < x; j++) {
            output[(i * x + j) * 3] = image[((y - i - 1) *x + j) * 3];
            output[(i * x + j) * 3 + 1] = image[((y - i - 1) * x + j) * 3 + 1];
            output[(i * x + j) * 3 + 2] = image[((y - i - 1) * x + j) * 3 + 2];
        }
    }
    return output;
}


int find_index(int j,int i,int k,int x,int channels){
    return j*x*channels+i*channels+k;
}

unsigned char *blur(unsigned char *start,const unsigned char *end,int x,int y,int channels){
    unsigned char *pixel=start,*blur_image_start= malloc(channels*x*y),*p=start;
    unsigned char *blur_pixel;int k=0,i=0,j=0;int m=0;
    double sum=0;
    for(k=0;k<channels;k++){
        m=k;
        blur_pixel=blur_image_start;
        for(j=0;j<y;j++){
            *p=*(start+k+x*j*channels);
            for(i=0;i<x;i++){
                sum=0;
                if (0 == j && 0 == i )
                {
                    sum=(*(pixel+ find_index(j,i,k,x,channels))+*(pixel+ find_index(j,i+1,k,x,channels))+*(pixel+
                                                                                                           find_index(j+1,i+1,k,x,channels))+*(pixel+
                                                                                                                                               find_index(j+1,i,k,x,channels)))/4.0;
                }
                else if (0 == j && i==x-1)
                {
                    sum=(*(pixel+ find_index(j,i,k,x,channels))+*(pixel+ find_index(j,i-1,k,x,channels))+*(pixel+
                                                                                                           find_index(j+1,i-1,k,x,channels))+*(pixel+
                                                                                                                                               find_index(j+1,i,k,x,channels)))/4.0;
                }
                else if (y-1 == j  && i  == 0)
                {
                    sum=(*(pixel+ find_index(j,i,k,x,channels))+*(pixel+ find_index(j-1,i+1,k,x,channels))+*(pixel+
                                                                                                             find_index(j,i+1,k,x,channels))+*(pixel+ find_index(j-1,i,k,x,channels)))/4.0;
                }
                else if (x-1 ==i && j==y-1)
                {
                    sum=(*(pixel+ find_index(j,i,k,x,channels))+*(pixel+ find_index(j-1,i-1,k,x,channels))+*(pixel+
                                                                                                             find_index(j,i-1,k,x,channels))+*(pixel+ find_index(j-1,i,k,x,channels)))/4.0;
                }
                else if (j==0)
                {
                    sum=(*(pixel+ find_index(j,i,k,x,channels))+*(pixel+ find_index(j+1,i+1,k,x,channels))+*(pixel+
                                                                                                             find_index(j,i+1,k,x,channels))+*(pixel+ find_index(j+1,i,k,x,channels))+*(pixel+
                                                                                                                                                                                        find_index(j,i-1,k,x,channels))+*(pixel+ find_index(j+1,i-1,k,x,channels)))/6.0;
                }
                else if (i==0)
                {
                    sum=(*(pixel+ find_index(j,i,k,x,channels))+*(pixel+ find_index(j+1,i+1,k,x,channels))+*(pixel+
                                                                                                             find_index(j,i+1,k,x,channels))+*(pixel+ find_index(j+1,i,k,x,channels))+*(pixel+
                                                                                                                                                                                        find_index(j-1,i,k,x,channels))+*(pixel+ find_index(j-1,i+1,k,x,channels)))/6.0;
                }
                else if (i==x-1)
                {
                    sum=(*(pixel+ find_index(j,i,k,x,channels))+*(pixel+ find_index(j+1,i-1,k,x,channels))+*(pixel+
                                                                                                             find_index(j,i-1,k,x,channels))+*(pixel+
                                                                                                                                               find_index(j,i+1,k,x,channels))+*(pixel+ find_index(j-1,i,k,x,channels))+*(pixel+
                                                                                                                                                                                                                          find_index(j-1,i-1,k,x,channels)))/6.0;
                }
                else if (j==y-1)
                {
                    sum=(*(pixel+ find_index(j,i,k,x,channels))+*(pixel+ find_index(j-1,i+1,k,x,channels))+*(pixel+
                                                                                                             find_index(j,i+1,k,x,channels))+*(pixel+ find_index(j-1,i,k,x,channels))+*(pixel+
                                                                                                                                                                                        find_index(j,i-1,k,x,channels))+*(pixel+ find_index(j-1,i-1,k,x,channels)))/6.0;
                }
                else {
                    sum=(*(pixel+ find_index(j,i,k,x,channels))+*(pixel+ find_index(j+1,i+1,k,x,channels))+*(pixel+
                                                                                                             find_index(j,i+1,k,x,channels))+*(pixel+ find_index(j+1,i,k,x,channels))+*(pixel+
                                                                                                                                                                                        find_index(j,i-1,k,x,channels))+*(pixel+ find_index(j+1,i-1,k,x,channels))+*(pixel+
                                                                                                                                                                                                                                                                     find_index(j,i-1,k,x,channels))+*(pixel+ find_index(j-1,i-1,k,x,channels))+*(pixel+
                                                                                                                                                                                                                                                                                                                                                  find_index(j-1,i+1,k,x,channels)))/9.0;
                }
                *(blur_pixel+k)=sum;
                blur_pixel+=channels;
            }
        }
    }
    //stbi_write_jpg("C:\\Users\\ASUS\\CLionProjects\\project\\cmake-build-debug\\out_image.jpg",x,y,channels,blur_image_start,100);
    return blur_image_start;
}


unsigned char* negative(unsigned char *start,const unsigned char *end,int x,int y,int channels){
    unsigned char*pixel;unsigned char* invert_pixel;
    unsigned char *invert_start= malloc(x*y*channels);
    for (pixel=start,invert_pixel=invert_start;pixel<=end;pixel+=channels,invert_pixel++){
        *invert_pixel=255-*pixel;
    }
    return invert_start;
    //stbi_write_jpg("C:\\Users\\ASUS\\CLionProjects\\project\\cmake-build-debug\\out_image.jpg",x,y,1,invert_start,100);

}

unsigned char * invert(unsigned char *start,const unsigned char *end,int x,int y,int channels){
    unsigned char* pixel;unsigned char* invert_pixel;
    unsigned char* invert_image= malloc(x*y*channels);
    for (pixel=start,invert_pixel=invert_image;pixel<=end;pixel+=channels,invert_pixel+=channels){
        *invert_pixel=255-*pixel;
        *(invert_pixel+1)=255-* (pixel+1);
        *(invert_pixel+2)=255-*(pixel+2);
    }
    return invert_image;

}

BMPImage* bmp_invert(BMPImage* image){
    BMPImage * copy= malloc(sizeof (BMPImage));
    copy->width=image->width;copy->height=image->height;
    copy->data=image->data;
    for (int i=0;i<copy->height* copy->width;i++){
        RGB pixel=copy->data[i];
        pixel.r=255-pixel.r;
        pixel.g=255-pixel.g;
        pixel.b=255-pixel.b;
        copy->data[i]=pixel;
    }
    int pixel=*(int *)&copy->data[1];
    printf("%d",pixel);
    return copy;

}

BMPImage * bmp_negative(BMPImage* image){
    BMPImage * copy= malloc(sizeof (BMPImage));
    copy->width=image->width;copy->height=image->height;
    copy->data=image->data;
    for (int i=0;i<copy->height * copy->width;i++){
        int pixel=*(int *)&copy->data[i];
        pixel=255-pixel;
        copy->data[i]=*(RGB*)& pixel;
    }
    return copy;
}

unsigned char * gaussian_filter(unsigned char* src, int width, int height, int channels, double sigma) {
    unsigned char* gaussian_start = malloc(width * height * channels);
    int radius = ceil(3 * sigma);
    double kernel[2 * radius + 1];
    double sum = 0;
    for (int i = -radius; i <= radius; i++) {
        kernel[i + radius] = exp(-(i * i) / (2 * sigma * sigma));
        sum += kernel[i + radius];
    }
    for (int i = 0; i < 2 * radius + 1; i++) {
        kernel[i] /= sum;
    }
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int c = 0; c < channels; c++) {
                double sum = 0;
                for (int k = -radius; k <= radius; k++) {
                    int xk = x + k;
                    if (xk < 0) xk = -xk;
                    if (xk >= width) xk = 2 * width - xk - 1;
                    sum += kernel[k + radius] * src[(y * width + xk) * channels + c];
                }
                gaussian_start[(y * width + x) * channels + c] = round(sum);
            }
        }
    }
    return gaussian_start;
}

unsigned char* laplacian_filter(unsigned char *image, int width, int height,int channels) {
    int i, j;
    int mask[3][3] = {{-1,-1,-1},
                      {-1, 8,-1},
                      {-1,-1,-1}};
    int sum;
    unsigned char *new_image =  malloc(width*height* sizeof (unsigned char));
    for (i=1; i<width-1; i++) {
        for (j=1; j<height-1; j++) {
            sum = 0;
            sum += mask[0][0]*image[(i-1)*height+j-1];
            sum += mask[0][1]*image[(i-1)*height+j];
            sum += mask[0][2]*image[(i-1)*height+j+1];
            sum += mask[1][0]*image[i*height+j-1];
            sum += mask[1][1]*image[i*height+j];
            sum += mask[1][2]*image[i*height+j+1];
            sum += mask[2][0]*image[(i+1)*height+j-1];
            sum += mask[2][1]*image[(i+1)*height+j];
            sum += mask[2][2]*image[(i+1)*height+j+1];
            if (sum < 0) {
                new_image[i*height+j] = 0;
            } else if (sum > 255) {
                new_image[i*height+j] = 255;
            } else {
                new_image[i*height+j] = (unsigned char) sum;
            }
        }
    }
    //memcpy(image, new_image, width*height*sizeof(unsigned char));
    //free(new_image);
    return new_image;
}

unsigned char *mean_filter(unsigned char *image, int width, int height,int channels) {
    int i, j;
    unsigned char* mean_image= malloc(width*height*channels);
    for (i = 1; i < height - 1; i++) {
        for (j = 1; j < width - 1; j++) {
            mean_image[i * width + j] = (image[(i - 1) * width + j - 1] + image[(i - 1) * width + j]
                    + image[(i - 1) * width + j + 1] + image[i * width + j - 1] + image[i * width + j] + image[i * width + j + 1]
                    + image[(i + 1) * width + j - 1] + image[(i + 1) * width + j] + image[(i + 1) * width + j + 1]) / 9;
        }
    }
    return mean_image;
}


