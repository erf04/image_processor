#include <stdio.h>
#include "stdlib.h"
#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "C:\Users\ASUS\CLionProjects\untitled1\stb\stb_image_write.h"
#include "C:\Users\ASUS\CLionProjects\untitled1\stb\stb_image.h"


void gray(unsigned char *start,const unsigned char *end,int x,int y,int channels){
    int gray_channels;
    if (channels==4)
        gray_channels=2;
    else
        gray_channels=1;
    unsigned char *pixel;unsigned char *gray_pixel;unsigned int sum,average;
    unsigned char *gray_image_start= malloc(gray_channels*x*y);
    for (pixel=start,gray_pixel=gray_image_start;pixel!=end;pixel+=channels,gray_pixel+=gray_channels){
        sum= *pixel+ *(pixel+1)+ *(pixel+2);
        average=(unsigned int)(sum/3);
        *gray_pixel=average;
        if (channels==4)
            *(gray_pixel+1)=*(pixel+3);
    }
    stbi_write_jpg("C:\\Users\\ASUS\\CLionProjects\\project\\cmake-build-debug\\out_image.jpg",x,y,gray_channels,gray_image_start,100);



}

void sepia(unsigned char *start,const unsigned char *end,int x,int y,int channels)
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
    stbi_write_jpg("C:\\Users\\ASUS\\CLionProjects\\project\\cmake-build-debug\\out_image.jpg",x,y,channels,sepia_image_start,100);

}

void reflect(unsigned char *start,int size,int x,int y,int channels){

    unsigned char *ref_pixel;unsigned char *pixel;unsigned char *ref_image_start= malloc(3*x*y);
    for (pixel=start,ref_pixel=ref_image_start;pixel<(start+size/2);pixel+=channels,ref_pixel+=channels){
            *ref_pixel=(start+size-pixel);*(ref_pixel+1)=(start+size-pixel+1);*(ref_pixel+2)=(start+size-pixel+2);
    }
    stbi_write_jpg("C:\\Users\\ASUS\\CLionProjects\\project\\cmake-build-debug\\out_image.jpg",x,y,channels,ref_image_start,100);

}

void blur(unsigned char *start,const unsigned char *end,int x,int y,int channels){
    unsigned char *pixel=start,*blur_image_start= malloc(channels*x*y);
    //for (pixel=start,pixel_pixel=pixel_start;pixel<=end;pixel+=channels,pixel_pixel+=channels){
      //  *pixel_pixel=*pixel;*(pixel_pixel+1)=*(pixel+1);*(pixel_pixel+2)=*(pixel+2);
    //}
    unsigned char *blur_pixel=blur_image_start;
    x-=1;y-=1;
    for (int j=0;j<=y;j++){
        //if (pixel>end)
          //  break;
        for (int i=0; i <=x; i++){
            if (pixel>end)
                break;
            float m=1.0;
            double sum_r=*(pixel+i*j+j);

            double sum_g=*(pixel+i*j+j+1);
            double sum_b=*(pixel+i*j+j+2);
            if (0 == j && 0 == i )
            {
                sum_r +=(*(pixel+i*j+j)+*(pixel+j*(i+1)+(i+1))+*(pixel+(j+1)*i+i)+*(pixel+(j+1)*(i+1)+(i+1)))/4.0;
                sum_g +=(*(pixel+j*i+i+1)+*(pixel+j*(i+1)+(i+1)+1)+*(pixel+(j+1)*i+i+1)+*(pixel+(j+1)*(i+1)+(i+1)+1))/4.0;
                sum_b += (*(pixel+j*i+i+2)+*(pixel+j*(i+1)+(i+1)+2)+*(pixel+(j+1)*i+i+2)+*(pixel+(j+1)*(i+1)+(i+1)+2))/4.0;
                m++;
            }
            else if (0 == j && i==x)
            {
                sum_r += (*(pixel+j*i+i)+*(pixel+j*(i-1)+(i-1))+*(pixel+(j+1)*i+i)+ *(pixel+(j+1)*(i-1)+(i-1)))/4.0;
                sum_g += (*(pixel+j*i+i+1)+*(pixel+j*(i-1)+(i-1)+1)+*(pixel+(j+1)*i+i+1)+ *(pixel+(j+1)*(i-1)+(i-1)+1))/4.0;
                sum_b += (*(pixel+j*i+i+2)+*(pixel+j*(i-1)+(i-1)+2)+*(pixel+(j+1)*i+i+2)+ *(pixel+(j+1)*(i-1)+(i-1)+2))/4.0;
                m ++;
            }
            else if (y == j  && i  == 0)
            {
                sum_r += (*(pixel+j*i+i)+*(pixel+j*(i+1)+(i+1))+*(pixel+(j-1)*i+i)+*(pixel+(j-1)*(i+1)+(i+1)))/4.0;
                sum_g += (*(pixel+j*i+i+1)+*(pixel+j*(i+1)+(i+1)+1)+*(pixel+(j-1)*i+i+1)+*(pixel+(j-1)*(i+1)+(i+1)+1))/4.0;
                sum_b += (*(pixel+j*i+i+2)+*(pixel+j*(i+1)+(i+1)+2)+*(pixel+(j-1)*i+i+2)+*(pixel+(j-1)*(i+1)+(i+1)+2))/4.0;
                m += 1;
            }
            else if (x ==i && j==y)
            {
                sum_r += (*(pixel+j*i+i)+*(pixel+j*(i-1)+(i-1)) + *(pixel+(j-1)*i+i) + *(pixel+(j-1)*(i-1)+(i-1)))/4.0;
                sum_g +=(*(pixel+j*i+i+1)+*(pixel+j*(i-1)+(i-1)+1) + *(pixel+(j-1)*i+i+1) + *(pixel+(j-1)*(i-1)+(i-1)+1))/4.0;
                sum_b += (*(pixel+j*i+i+2)+*(pixel+j*(i-1)+(i-1)+2) + *(pixel+(j-1)*i+i+2) + *(pixel+(j-1)*(i-1)+(i-1)+2))/4.0;
                m += 1;
            }
            else if (j==0)
            {
                sum_r += (*(pixel+j*i+i) + *(pixel+j*(i-1)+(i-1)) + *(pixel+j*(i+1)+(i+1)) + *(pixel+(j+1)*(i-1)+(i-1))+*(pixel+(j+1)*i+i) +*(pixel+(j+1)*(i+1)+(i+1)))/6.0;
                sum_g += (*(pixel+j*i+i+1) + *(pixel+j*(i-1)+(i-1)+1) + *(pixel+j*(i+1)+(i+1)+1) + *(pixel+(j+1)*(i-1)+(i-1)+1)+*(pixel+(j+1)*i+i+1) +*(pixel+(j+1)*(i+1)+(i+1)+1))/6.0;
                sum_b += (*(pixel+j*i+i+2) + *(pixel+j*(i-1)+(i-1)+2) + *(pixel+j*(i+1)+(i+1)+2) + *(pixel+(j+1)*(i-1)+(i-1)+2)+*(pixel+(j+1)*i+i+2) +*(pixel+(j+1)*(i+1)+(i+1)+2))/6.0;
                m += 1;
            }
            else if (i==0)
            {
                sum_r += (*(pixel+j*i+i) + *(pixel+(j-1)*i+i) + *(pixel+(j-1)*(i+1)) +*(pixel+j*(i+1)+(i+1)) + *(pixel+(j+1)*i+i) + *(pixel+(j+1)*(i+1)+(i+1)))/6.0;
                sum_g += (*(pixel+j*i+i+1) + *(pixel+(j-1)*i+i+1) + *(pixel+(j-1)*(i+1)+1) +*(pixel+j*(i+1)+(i+1)+1) + *(pixel+(j+1)*i+i+1) + *(pixel+(j+1)*(i+1)+(i+1)+1))/6.0;
                sum_b +=(*(pixel+j*i+i+2) + *(pixel+(j-1)*i+i+2) + *(pixel+(j-1)*(i+1)+2) +*(pixel+j*(i+1)+(i+1)+2) + *(pixel+(j+1)*i+i+2) + *(pixel+(j+1)*(i+1)+(i+1)+2))/6.0;
                m += 1;
            }
            else if (i==x)
            {
                sum_r += (*(pixel+j*i+i) +*(pixel+(j-1)*i+i) + *(pixel+(j-1)*(i-1)+(j-1)) + *(pixel+j*(i-1)+(i-1)) + *(pixel+(j+1)*i+i) + *(pixel+(j+1)*(i-1)+(i-1)))/6.0;
                sum_g +=   (*(pixel+j*i+i+1) +*(pixel+(j-1)*i+i+1) + *(pixel+(j-1)*(i-1)+(j-1)+1) + *(pixel+j*(i-1)+(i-1)+1) + *(pixel+(j+1)*i+i+1) + *(pixel+(j+1)*(i-1)+(i-1)+1))/6.0;
                sum_b +=   (*(pixel+j*i+i+2) +*(pixel+(j-1)*i+i+2) + *(pixel+(j-1)*(i-1)+(j-1)+2) + *(pixel+j*(i-1)+(i-1)+2) + *(pixel+(j+1)*i+i+2) + *(pixel+(j+1)*(i-1)+(i-1)+2))/6.0;
                m += 1;
            }
            else if (j==y)
            {
                sum_r += (*(pixel+j*i+i) + *(pixel+j*(i-1)+(i-1)) + *(pixel+(j-1)*(i-1)+(i-1)) + *(pixel+(j-1)*i+i)+ *(pixel+j*(i+1)+(i+1)) + *(pixel+(j-1)*(i+1)+(i+1)))/6.0;
                sum_g += (*(pixel+j*i+i+1) + *(pixel+j*(i-1)+(i-1)+1) + *(pixel+(j-1)*(i-1)+(i-1)+1) + *(pixel+(j-1)*i+i+1)+ *(pixel+j*(i+1)+(i+1)+1) + *(pixel+(j-1)*(i+1)+(i+1)+1))/6.0;
                sum_b += (*(pixel+j*i+i+2) + *(pixel+j*(i-1)+(i-1)+2) + *(pixel+(j-1)*(i-1)+(i-1)+2) + *(pixel+(j-1)*i+i+2)+ *(pixel+j*(i+1)+(i+1)+2) + *(pixel+(j-1)*(i+1)+(i+1)+2))/6.0;
                m += 1;
            }
            else {
                sum_r+= (*(pixel+j*i+i) + *(pixel+(j-1)*(i-1)+(i-1)) + *(pixel+(j-1)*i+i)+ *(pixel+(j-1)*(i+1)+(i+1)) + *(pixel+j*(i-1)+(i-1))+ *(pixel+j*(i+1)+(i+1)) + *(pixel+(j+1)*(i-1)+(i-1)) + *(pixel+(j+1)*i+i) + *(pixel+(j+1)*(i+1)+(i+1)))/9.0;
                sum_g+=(*(pixel+j*i+i+1) + *(pixel+(j-1)*(i-1)+(i-1)+1) + *(pixel+(j-1)*i+i+1)+ *(pixel+(j-1)*(i+1)+(i+1)+1) + *(pixel+j*(i-1)+(i-1)+1)+ *(pixel+j*(i+1)+(i+1)+1) + *(pixel+(j+1)*(i-1)+(i-1)+1) + *(pixel+(j+1)*i+i+1) + *(pixel+(j+1)*(i+1)+(i+1)+1))/9.0;
                sum_b+=(*(pixel+j*i+i+2) + *(pixel+(j-1)*(i-1)+(i-1)+2) + *(pixel+(j-1)*i+i+2)+ *(pixel+(j-1)*(i+1)+(i+1)+2) + *(pixel+j*(i-1)+(i-1)+2)+ *(pixel+j*(i+1)+(i+1)+2) + *(pixel+(j+1)*(i-1)+(i-1)+2) + *(pixel+(j+1)*i+i+2) + *(pixel+(j+1)*(i+1)+(i+1)+2))/9.0;
            }

            *blur_pixel=(int )sum_r;*(blur_pixel+1)=(int )sum_g;*(blur_pixel+2)=(int )sum_b;
            blur_pixel+=channels;pixel+=channels;

        }

    }
    stbi_write_jpg("C:\\Users\\ASUS\\CLionProjects\\project\\cmake-build-debug\\out_image.jpg",x,y,channels,blur_image_start,100);


}
int main(){
    int x,y,channels;
    unsigned char *image= stbi_load("C:\\Users\\ASUS\\CLionProjects\\project\\Duck.jpg",&x,&y,&channels,0);
    int image_size=x*y*channels;
    unsigned char *end_image=(unsigned char *)(image_size+image);
    blur(image,end_image,x,y,channels);




    stbi_image_free(image);

}