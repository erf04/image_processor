#include <stdio.h>
#include "stdlib.h"
#include "filters.h"
#define STB_IMAGE_IMPLEMENTATION
#include "C:\Users\ASUS\CLionProjects\project\stb\stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "C:\Users\ASUS\CLionProjects\project\stb\stb_image_write.h"




/*#pragma pack(push, 1)
typedef struct {
    char signature[2];
    int size;
    int reserved;
    int offset;
} BMPHeader;

typedef struct {
    int size;
    int width;
    int height;
    short planes;
    short bitsPerPixel;
    unsigned compression;
    unsigned imageSize;
    int xPixelsPerMeter;
    int yPixelsPerMeter;
    unsigned colorsUsed;
    unsigned colorsImportant;
} BMPInfoHeader;

typedef struct {
    BMPHeader header;
    BMPInfoHeader infoHeader;
} BMPImage;
#pragma pack(pop)
*/

/*typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
}RGB;

typedef struct {
    int width;
    int height;
    RGB * data;
}BMPImage;*/
typedef const char* str;
int get_int(){
    char c;int x=0;
    while((c=(char)getchar())!='\n' && c!=EOF){
        x=x*10+(c-'0');
    }
    return x;
}
str get_str(int* len){
    char c;char * string= malloc(sizeof (char)*1000);int i=0;
    while((c=(char)getchar())!='\n' && c!=EOF){
        *(string+i)=c;i++;
    }
    *(string+i)='\0';
    * len=i;
    if (*string=='\0')
        return NULL;
    return (str) string;
}
int isequal(str s1,str s2){
    while(*s1){
        if (*s1!=*s2)
            return 0;
        s1++;s2++;
    }
    if (!s2)
        return 0;
    return 1;
}
int find_format(str address,int len){
    char* format= malloc((sizeof (char)*4));int i=0;int j=0;
    while(address[len-1-i]!='.'){
        format[2-i]=address[len-1-i];
        i++;
    }
    format[3]='\0';

    if (isequal(format,"jpg"))
        return 1;
    if (isequal(format,"png"))
        return 2;
    if (isequal(format,"bmp"))
        return 3;
    printf("the format not valid!!\n");
}

/*BMPHeader header;
BMPInfoHeader infoHeader;
unsigned char* read_bmp(str address){
    FILE *image=fopen(address,"rb");

    fread(&header,sizeof (header),1,image);

    fread(&infoHeader,sizeof (infoHeader),1,image);
    if (infoHeader.bitsPerPixel != 24) {
        fprintf(stderr, "Error: Only 24-bit BMP files are supported\n");
    }

    unsigned char* image_data= malloc(infoHeader.imageSize);
    fread(&image_data,infoHeader.imageSize,1,image);
    fclose(image);
    return image_data;
}

int write_bmp(FILE *out_file,unsigned char* bmp_data){
    rewind(out_file);
    fwrite(&header,sizeof (header),1,out_file);
    fwrite(&infoHeader,sizeof (infoHeader),1,out_file);
    fwrite(bmp_data,infoHeader.imageSize,1,out_file);
    return 0;
}
*/

/*typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
}RGB;

typedef struct {
    int width;
    int height;
    RGB * data;
}BMPImage;
*/
/*unsigned char* gray(unsigned char *start,const unsigned char *end,int x,int y,int channels){
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

void reflect(unsigned char *start,int size,int x,int y,int channels){

    unsigned char *ref_pixel;unsigned char *pixel;unsigned char *ref_image_start= malloc(3*x*y);
    for (pixel=start,ref_pixel=ref_image_start;pixel<(start+size/2);pixel+=channels,ref_pixel+=channels){
        *ref_pixel=(start+size-pixel);*(ref_pixel+1)=(start+size-pixel+1);*(ref_pixel+2)=(start+size-pixel+2);
    }
    //stbi_write_jpg("C:\\Users\\ASUS\\CLionProjects\\project\\cmake-build-debug\\out_image.jpg",x,y,channels,ref_image_start,100);

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


unsigned char* invert(unsigned char *start,const unsigned char *end,int x,int y,int channels){
    unsigned char*pixel;unsigned char* invert_pixel;
    unsigned char *invert_start= malloc(x*y*channels);
    for (pixel=start,invert_pixel=invert_start;pixel<=end;pixel+=channels,invert_pixel++){
        *invert_pixel=255-*pixel;
    }
    return invert_start;
    //stbi_write_jpg("C:\\Users\\ASUS\\CLionProjects\\project\\cmake-build-debug\\out_image.jpg",x,y,1,invert_start,100);

}
*/

BMPImage * read_bmp(str address){
    FILE * file=fopen(address,"rb");
    unsigned char header[54];
    fread(header,sizeof (unsigned char ),54,file);
    int width=*(int *)&header[18];int height=*(int*)&header[22];
    //bpp=bits per pixel
    short bpp=*(short*)&header[28];
    if (bpp!=24){
        printf("Error: %d bits per pixel is not supported\n",bpp);
        fclose(file);
        return NULL;
    }
    RGB * data= malloc(sizeof (RGB)*width*height);
    int padding=(4-(width*3)%4)%4;
    for (int y=height-1;y>=0;y--){
        for (int x=0;x<width;x++){
            RGB pixel;
            fread(&pixel.r,sizeof (unsigned char),1,file);
            fread(&pixel.g,sizeof (unsigned char),1,file);
            fread(&pixel.b,sizeof (unsigned char),1,file);
            data[y*width+x]=pixel;
        }
        fseek(file,padding,SEEK_CUR);
    }
    fclose(file);
    BMPImage * image= malloc(sizeof (BMPImage));
    image->width=width;
    image->height=height;
    image->data=data;
    return image;
}

void save_bmp(BMPImage* image,str out_address){
    FILE * out_file= fopen(out_address,"wb");
    unsigned char header[54] = {0};
    header[0] = 'B';
    header[1] = 'M';
    *(int*)&header[2] = 54 + 3 * image->width * image->height; // file size
    *(int*)&header[10] = 54; // offset to pixel data
    *(int*)&header[14] = 40; // size of info header
    *(int*)&header[18] = image->width;
    *(int*)&header[22] = image->height;
    *(short*)&header[26] = 1; // number of color planes
    *(short*)&header[28] = 24;
    fwrite(header,sizeof (unsigned char),54,out_file);
    int padding = (4 - (image->width * 3) % 4) % 4;
    unsigned char padByte = 0;
    fwrite(&padByte, sizeof(unsigned char), padding, out_file);
    for (int y = image->height-1; y >=0; y--) {
        for (int x = 0; x < image->width; x++) {
            RGB pixel = image->data[y * image->width + x];
            fwrite(&pixel.b, sizeof(unsigned char), 1, out_file);
            fwrite(&pixel.g, sizeof(unsigned char), 1, out_file);
            fwrite(&pixel.r, sizeof(unsigned char), 1, out_file);
        }
        //unsigned char padByte = 0;
        //fwrite(&padByte, sizeof(unsigned char), padding, out_file);
    }
    fclose(out_file);
}




/*int median(int arr[])
{
    int n = sizeof(arr) / sizeof(arr[0]);
    int temp;
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (arr[j] < arr[i])
            {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
    return arr[n / 2];
}
void median_filter(const unsigned char *image,int width,int height ,int channels){
    // Apply a median filter to the image here
    unsigned char *new_image = malloc(width * height * channels);
    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            int red[9], green[9], blue[9];
            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    red[k * 3 + l + 4] = image[(i + k) * width * channels + (j + l) * channels];
                    green[k * 3 + l + 4] = image[(i + k) * width * channels + (j + l) * channels + 1];
                    blue[k * 3 + l + 4] = image[(i + k) * width * channels + (j + l) * channels + 2];
                }
            }
            new_image[i * width * channels + j * channels] = (unsigned char)median(red);
            new_image[i * width * channels + j * channels + 1] = (unsigned char)median(green);
            new_image[i * width * channels + j * channels + 2] = (unsigned char)median(blue);
        }
    }

    stbi_write_jpg("C:\\Users\\ASUS\\CLionProjects\\project\\cmake-build-debug\\out_image.jpg", width, height, channels, new_image, 100);
    //stbi_image_free(image);
    //free(new_image);
    return ;
}*/


int main(){
    int x,y,channels,len;
    printf("welcome to project sagi!\n");
    printf("put the image address:");
    str address=get_str(&len);
    printf("choose the filter :");
    printf(" 1)grayscale  2)sepia  3)blur  4)invert\n");
    int filter=get_int();

    //str address="C:\\Users\\ASUS\\CLionProjects\\project\\Duck.jpg";
    //str address=".\\input.bmp";
    int n = find_format(address,len);
    str format;
    str out_format;
    if (n==1){
        format="jpg";
        out_format="1)jpg 2)png";
    }
    else if (n==2){
        format="png";
        out_format="1)jpg 2)png";

    }
    else if (n==3){
        format="bmp";
        out_format="1)bmp";
    }
    printf("your image format is %s\n",format);
    printf("you can write the filtered image in these formats: %s\n",out_format);
    printf("please choose one:");
    int out_choose_format=get_int();
    printf("put your output address\n(if your dont ,the program will choose a address for you):");
    str out_address= get_str(&len);
    if (n==1 || n==2){
        unsigned char *image= stbi_load(address,&x,&y,&channels,0);
        int image_size=x*y*channels;
        unsigned char* filtered;
        unsigned char *end_image=(unsigned char *)(image_size+image);
        switch (filter) {
            case 1:
                filtered=gray(image,end_image,x,y,channels);
                if (out_choose_format==1)
                    stbi_write_jpg("C:\\Users\\ASUS\\CLionProjects\\project\\cmake-build-debug\\out_image.jpg",x,y,1,filtered,100);
                else
                    stbi_write_png("C:\\Users\\ASUS\\CLionProjects\\project\\cmake-build-debug\\out_image.png",x,y,1,filtered,100);
                break;
            case 2:
                filtered=sepia(image,end_image,x,y,channels);
                stbi_write_jpg("C:\\Users\\ASUS\\CLionProjects\\project\\cmake-build-debug\\out_image.jpg",x,y,channels,filtered,100);
                break;
            case 3:
                filtered=blur(image,end_image,x,y,channels);
                stbi_write_jpg("C:\\Users\\ASUS\\CLionProjects\\project\\cmake-build-debug\\out_image.jpg",x,y,channels,filtered,100);
                break;
            case 4:
                filtered=invert(image,end_image,x,y,channels);
                stbi_write_jpg("C:\\Users\\ASUS\\CLionProjects\\project\\cmake-build-debug\\out_image.jpg",x,y,1,filtered,100);
                break;
        }
    }
    if (n==3){
        BMPImage *image= read_bmp(address);
        BMPImage * filtered;
        switch (filter) {
            case 1:
                filtered=bmp_gray(image);
                save_bmp(filtered,"C:\\Users\\ASUS\\CLionProjects\\project\\out.bmp");
                break;
            case 2:
                filtered= bmp_sepia(image);
                save_bmp(filtered,"C:\\Users\\ASUS\\CLionProjects\\project\\out.bmp");
                break;


        }
        //str out_address=".\\out.bmp";
        //save_bmp(filtered,out_address);
    }










    //stbi_image_free(image);

}