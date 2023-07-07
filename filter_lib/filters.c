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
    copy->data = (RGB*)malloc(image->width * image->height * sizeof(RGB));
    double r,g,b;
    for (int i=0;i< image->height * image->width;i++){
        int row = i / image->width;
        int col = i % image->width;
        RGB pixel = image->data[row * image->width + col];
        r = (pixel.r * .393) + (pixel.g * .769) + (pixel.b * .189);
        g = (pixel.r * .349) + (pixel.g * .686) + (pixel.b * .168);
        //b = (pixel.r * .272) + (pixel.g * .534) + (pixel.b * .131);

        if (r>255)
            r=255;
        if (g>255)
            g=255;
        if (b>255)
            b=255;
        copy->data[i].r=(unsigned char)(round(r));
        copy->data[i].g=(unsigned char)(round(g));
        copy->data[i].b=pixel.b;

    }
    return copy;
}

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

unsigned char* reflect2(unsigned char *image, int x, int y,int channels){
    int i,j;
    unsigned char* output= malloc(x*y*channels);
    for (j=0;j<y;j++){
        for (i=0;i<x;i++){
            output[(j*x+i)*channels]=image[(j*x+(x-1-i))*channels];
            output[(j*x+i)*channels +1]=image[(j*x+(x-1-i))*channels +1];
            output[(j*x+i)*channels+2]=image[(j*x+(x-1-i))*channels +2];
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


unsigned char* negative(unsigned char *start,const unsigned char *end,int x,int y,int channels){
    unsigned char*pixel;unsigned char* invert_pixel;
    unsigned char *invert_start= malloc(x*y*channels);
    for (pixel=start,invert_pixel=invert_start;pixel<=end;pixel+=channels,invert_pixel++){
        *invert_pixel=255-*pixel;
    }
    return invert_start;

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
    return copy;

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

    return new_image;
}

unsigned char *mean_filter(unsigned char *image, int width, int height,int channels) {
    int i, j;
    unsigned char* mean_image= malloc(width*height*channels);
    for (i = 1; i < height - 1; i++) {
        for (j = 1; j < width - 1; j++) {
            mean_image[(i * width + j)*channels] = (image[((i - 1) * width + j)*channels -1] + image[((i - 1) * width + j)*channels]
                                                    + image[((i - 1) * width + j )*channels +1] + image[(i * width + j)*channels - 1] + image[(i * width + j)*channels]
                                                    + image[(i * width + j)*channels + 1]
                                                    + image[((i + 1) * width + j)*channels - 1] + image[((i + 1) * width)*channels + j] + image[((i + 1) * width + j)*channels + 1]) / 9;
        }
    }
    return mean_image;
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
BMPImage * bmp_blur(BMPImage *image){
    BMPImage *copy= malloc(sizeof(BMPImage));
    copy->width=image->width;copy->height=image->height;
    copy->data=image->data;
    double sumr=0,sumg=0,sumb=0;
    int x=copy->width;int y=copy->height;
    for(int j=0;j<y;j++){
        for (int i=0;i<x;i++){
            sumr=copy->data[j*x+i].r;sumg=copy->data[j*x+i].g;sumb=copy->data[j*x+i].b;
            if(0 == j && 0 == i){
                sumr=(copy->data[j*x+i].r+copy->data[(j+1)*x+i].r+copy->data[j*x+(i+1)].r+copy->data[(j+1)*x+(i+1)].r)/4.0;
                sumg=(copy->data[j*x+i].g+copy->data[(j+1)*x+i].g+copy->data[j*x+(i+1)].g+copy->data[(j+1)*x+(i+1)].g)/4.0;
                sumb=(copy->data[j*x+i].b+copy->data[(j+1)*x+i].b+copy->data[j*x+(i+1)].b+copy->data[(j+1)*x+(i+1)].b)/4.0;
            }
            else if(0 == j && i==x-1){
                sumr=(copy->data[j*x+i].r+copy->data[j*x+(i-1)].r+copy->data[(j+1)*x+i].r+copy->data[(j+1)*x+(i-1)].r)/4.0;
                sumg=(copy->data[j*x+i].g+copy->data[j*x+(i-1)].g+copy->data[(j+1)*x+i].g+copy->data[(j+1)*x+(i-1)].g)/4.0;
                sumb=(copy->data[j*x+i].b+copy->data[j*x+(i-1)].b+copy->data[(j+1)*x+i].b+copy->data[(j+1)*x+(i-1)].b)/4.0;
            }
            else if(y-1 == j  && i  == 0){
                sumr=(copy->data[j*x+i].r+copy->data[(j-1)*x+i].r+copy->data[j*x+(i+1)].r+copy->data[(j-1)*x+(i+1)].r)/4.0;
                sumg=(copy->data[j*x+i].g+copy->data[(j-1)*x+i].g+copy->data[j*x+(i+1)].g+copy->data[(j-1)*x+(i+1)].g)/4.0;
                sumb=(copy->data[j*x+i].b+copy->data[(j-1)*x+i].b+copy->data[j*x+(i+1)].b+copy->data[(j-1)*x+(i+1)].b)/4.0;
            }
            else if(x-1 ==i && j==y-1){
                sumr=(copy->data[j*x+i].r+copy->data[(j-1)*x+i].r+copy->data[(j-1)*x+(i-1)].r+copy->data[j*x+(i-1)].r)/4.0;
                sumg=(copy->data[j*x+i].g+copy->data[(j-1)*x+i].g+copy->data[(j-1)*x+(i-1)].g+copy->data[j*x+(i-1)].g)/4.0;
                sumb=(copy->data[j*x+i].b+copy->data[(j-1)*x+i].b+copy->data[(j-1)*x+(i-1)].b+copy->data[j*x+(i-1)].b)/4.0;
            }
            else if(j==0) {
                sumr = (copy->data[j * x + i].r + copy->data[j * x + (i + 1)].r + copy->data[j * x + (i - 1)].r +
                        copy->data[(j + 1) * x + i].r + copy->data[(j + 1) * x + (i + 1)].r +
                        copy->data[(j + 1) * x + (i - 1)].r)/6.0;
                sumg=(copy->data[j*x+i].g+copy->data[j*x+(i+1)].g+copy->data[j*x+(i-1)].g+copy->data[(j+1)*x+i].g+copy->data[(j+1)*x+(i+1)].g+copy->data[(j+1)*x+(i-1)].g)/6.0;
                sumb=(copy->data[j*x+i].b+copy->data[j*x+(i+1)].b+copy->data[j*x+(i-1)].b+copy->data[(j+1)*x+i].b+copy->data[(j+1)*x+(i+1)].b+copy->data[(j+1)*x+(i-1)].b)/6.0;
            }
            else if(i==0){
                sumr=(copy->data[j*x+i].r+copy->data[j*x+(i+1)].r+copy->data[(j+1)*x+(i+1)].r+copy->data[(j+1)*x+i].r+copy->data[(j-1)*x+(i+1)].r+copy->data[j*x+(i-1)].r)/6.0;
                sumg=(copy->data[j*x+i].g+copy->data[j*x+(i+1)].g+copy->data[(j+1)*x+(i+1)].g+copy->data[(j+1)*x+i].g+copy->data[(j-1)*x+(i+1)].g+copy->data[j*x+(i-1)].g)/6.0;
                sumb=(copy->data[j*x+i].b+copy->data[j*x+(i+1)].b+copy->data[(j+1)*x+(i+1)].b+copy->data[(j+1)*x+i].b+copy->data[(j-1)*x+(i+1)].b+copy->data[j*x+(i-1)].b)/6.0;

            }
            else if(i==x-1){
                sumr=(copy->data[j*x+i].r+copy->data[j*x+(i-1)].r+copy->data[(j+1)*x+(i-1)].r+copy->data[(j+1)*x+i].r+copy->data[(j-1)*x+(i-1)].r+copy->data[j*x+(i-1)].r)/6.0;
                sumg=(copy->data[j*x+i].g+copy->data[j*x+(i-1)].g+copy->data[(j+1)*x+(i-1)].g+copy->data[(j+1)*x+i].g+copy->data[(j-1)*x+(i-1)].g+copy->data[j*x+(i-1)].g)/6.0;
                sumb=(copy->data[j*x+i].b+copy->data[j*x+(i-1)].b+copy->data[(j+1)*x+(i-1)].b+copy->data[(j+1)*x+i].b+copy->data[(j-1)*x+(i-1)].b+copy->data[j*x+(i-1)].b)/6.0;
            }
            else if(j==y-1){
                sumr=(copy->data[j*x+i].r+copy->data[j*x+(i-1)].r+copy->data[(j-1)*x+(i-1)].r+copy->data[(j-1)*x+i].r+copy->data[(j-1)*x+(i+1)].r+copy->data[j*x+(i+1)].r)/6.0;
                sumg=(copy->data[j*x+i].g+copy->data[j*x+(i-1)].g+copy->data[(j-1)*x+(i-1)].g+copy->data[(j-1)*x+i].g+copy->data[(j-1)*x+(i+1)].g+copy->data[j*x+(i+1)].g)/6.0;
                sumb=(copy->data[j*x+i].b+copy->data[j*x+(i-1)].b+copy->data[(j-1)*x+(i-1)].b+copy->data[(j-1)*x+i].b+copy->data[(j-1)*x+(i+1)].b+copy->data[j*x+(i+1)].b)/6.0;
            }
            else{
                sumr=(copy->data[j*x+i].r+copy->data[j*x+(i-1)].r+copy->data[(j-1)*x+(i-1)].r+copy->data[(j-1)*x+i].r+copy->data[(j-1)*x+(i+1)].r+copy->data[j*x+(i+1)].r+copy->data[(j+1)*x+i].r+copy->data[(j+1)*x+(i+1)].r+copy->data[(j+1)*x+(i-1)].r)/9.0;
                sumg=(copy->data[j*x+i].g+copy->data[j*x+(i-1)].g+copy->data[(j-1)*x+(i-1)].g+copy->data[(j-1)*x+i].g+copy->data[(j-1)*x+(i+1)].g+copy->data[j*x+(i+1)].g+copy->data[(j+1)*x+i].g+copy->data[(j+1)*x+(i+1)].g+copy->data[(j+1)*x+(i-1)].g)/9.0;
                sumb=(copy->data[j*x+i].b+copy->data[j*x+(i-1)].b+copy->data[(j-1)*x+(i-1)].b+copy->data[(j-1)*x+i].b+copy->data[(j-1)*x+(i+1)].b+copy->data[j*x+(i+1)].b+copy->data[(j+1)*x+i].b+copy->data[(j+1)*x+(i+1)].b+copy->data[(j+1)*x+(i-1)].b)/9.0;
            }
            copy->data[j*x+i].r=(unsigned char)sumr;
            copy->data[j*x+i].g=(unsigned char)sumg;
            copy->data[j*x+i].b=(unsigned char)sumb;

        }
    }
    return copy;
}

BMPImage *bmp_reflect(BMPImage *image){
    BMPImage *copy= malloc(sizeof(BMPImage));
    copy->width=image->width;copy->height=image->height;
    copy->data=image->data;int x=copy->width,y=copy->height;int temp=0;
    for (int i = 0;  i< x/2; i++) {
        for (int j = 0; j < y; j++) {
            temp=copy->data[j * x + i].r;
            copy->data[j * x + i].r = copy->data[(j *x + (x-i-1))].r;
            copy->data[(j *x + (x-i-1))].r=temp;

            temp=copy->data[j * x + i].g;
            copy->data[j * x + i].g = copy->data[(j *x + (x-i-1))].g;
            copy->data[((j *x + (x-i-1)))].g=temp;

            temp=copy->data[j * x + i].b;
            copy->data[j * x + i].b = copy->data[(j *x + (x-i-1))].b;
            copy->data[(j *x + (x-i-1))].b=temp;
        }
    }
    return copy;

}
BMPImage *bmp_reflect2(BMPImage *image) {
    BMPImage *copy = malloc(sizeof(BMPImage));
    copy->width = image->width;
    copy->height = image->height;
    copy->data = image->data;
    int x = copy->width, y = copy->height;
    int temp = 0;
    for (int j = 0; j < y / 2; j++) {
        for (int i = 0; i < x; i++) {
            temp = copy->data[j * x + i].r;
            copy->data[j * x + i].r = copy->data[((y - j - 1) * x + i)].r;
            copy->data[((y - j - 1) * x + i)].r = temp;

            temp = copy->data[j * x + i].g;
            copy->data[j * x + i].g = copy->data[((y - j - 1) * x + i)].g;
            copy->data[((y - j - 1) * x + i)].g = temp;

            temp = copy->data[j * x + i].b;
            copy->data[j * x + i].b = copy->data[((y - j - 1) * x + i)].b;
            copy->data[((y - j - 1) * x + i)].b = temp;
        }
    }
    return copy;
}

unsigned char* rotate(unsigned char *image, int width, int height,int channels){
    unsigned char *rotated_image = malloc(width * height * channels);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int index = (i * width + j) * channels;
            int rotated_index = ((width - j - 1) * height + i) * channels;
            for (int k = 0; k < channels; k++) {
                rotated_image[rotated_index + k] = image[index + k];
            }
        }
    }
    return rotated_image;

}

unsigned char* bright(unsigned char* image,int width,int height,int channels){
    unsigned char *bright_image= malloc(width * height * channels);
    for (int i = 0; i < width * height * channels; i++) {
        bright_image[i] = fminf((float)image[i] * 1.5f, 255.0f);
        return bright_image;
    }


}
BMPImage *bmp_rotate(BMPImage *image){
    int y=image->height,x=image->width;
    BMPImage *copy= malloc(sizeof (BMPImage));
    copy->data=malloc(sizeof(RGB)*y*x);
    copy->width=y;
    copy->height=x;
    for (int j = 0; j < y ; j++) {
        for (int i = 0; i < x; i++) {
            copy->data[(x-i-1)*y+j].r=image->data[j*x+i].r;
            copy->data[(x-i-1)*y+j].g=image->data[j*x+i].g;
            copy->data[(x-i-1)*y+j].b=image->data[j*x+i].b;
        }
    }
    return copy;
}
unsigned char* red(unsigned char *image,const unsigned char *end, int width, int height,int channels){
    int size=width*height;
    unsigned char* red_image= malloc(size*channels);
    unsigned char *pixel=image;
    for (pixel=image;pixel<=end;pixel+=channels){
        *(pixel+1)=0;
        *(pixel+2)=0;
    }
    return pixel;
}
unsigned char* green(unsigned char *image,const unsigned char *end, int width, int height,int channels){
    int size=width*height;
    unsigned char* red_image= malloc(size*channels);
    unsigned char *pixel=image;
    for (pixel=image;pixel<=end;pixel+=channels){
        *(pixel)=0;
        *(pixel+2)=0;
    }
    return pixel;
}
unsigned char* blue(unsigned char *image,const unsigned char *end, int width, int height,int channels){
    int size=width*height;
    unsigned char* red_image= malloc(size*channels);
    unsigned char *pixel=image;
    for (pixel=image;pixel<=end;pixel+=channels){
        *(pixel)=0;
        *(pixel+1)=0;
    }
    return pixel;
}