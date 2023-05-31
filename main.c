#include <stdio.h>

#include <stdio.h>
#include "stdlib.h"
#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "C:\Users\KAVOSH\CLionProjects\untitled32\project_sagi\image_read_write_lib\stb\stb_image.h"
#include "C:\Users\KAVOSH\CLionProjects\untitled32\project_sagi\image_read_write_lib\stb\stb_image_write.h"


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

int find_index(int j,int i,int k,int x,int channels){
    return j*x*channels+i*channels+k;
}
unsigned char *blur(unsigned char *start,const unsigned char *end,int x,int y,int channels){
    unsigned char *pixel=start,*blur_image_start= malloc(channels*x*y),*p=start;
    //for (pixel=start,pixel_pixel=pixel_start;pixel<=end;pixel+=channels,pixel_pixel+=channels){
      //  *pixel_pixel=*pixel;*(pixel_pixel+1)=*(pixel+1);*(pixel_pixel+2)=*(pixel+2);
    //}
    unsigned char *blur_pixel;int k=0,i=0,j=0;int m=0;
    double sum=0;
    for(k=0;k<channels;k++){
        m=k;
        blur_pixel=blur_image_start;sum=0;
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
    stbi_write_jpg("C:\\Users\\KAVOSH\\CLionProjects\\untitled32\\out_image.jpg",x,y,channels,blur_image_start,100);
    return blur_image_start;
}


int main(){
    int x,y,channels;
    unsigned char *image= stbi_load("C:\\Users\\KAVOSH\\CLionProjects\\untitled32\\Beautiful-Full-HD-Images.jpg",&x,&y,&channels,0);
    int image_size=x*y*channels;
    unsigned char *end_image=(unsigned char *)(image_size+image);
    for (int i=0;i!=5;i++)
        image=blur(image,end_image,x,y,channels);




    stbi_image_free(image);
}