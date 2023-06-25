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


BMPImage * read_bmp(str address){
    FILE * file=fopen(address,"rb");
    unsigned char header[54];
    fread(header,sizeof (unsigned char ),54,file);
    int width=*(int *)&header[18];int height=*(int*)&header[22];
    int data_offset=*(int*)&header[10];
    //bpp=bits per pixel
    short bpp=*(short*)&header[28];
    if (bpp!=24){
        printf("Error: %d bits per pixel is not supported\n",bpp);
        fclose(file);
        return NULL;
    }
    RGB * data= malloc(sizeof (RGB)*width*height);
    fseek(file, data_offset, SEEK_SET);
    int padding=(4-(width*3)%4)%4;
    for (int y=0;y<height;y++){
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
    for (int y = 0; y <image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            RGB pixel = image->data[y * image->width + x];
            fwrite(&pixel.b, sizeof(unsigned char), 1, out_file);
            fwrite(&pixel.g, sizeof(unsigned char), 1, out_file);
            fwrite(&pixel.r, sizeof(unsigned char), 1, out_file);
        }
        for (int i=0;i<padding;i++){
            fputc(0x00,out_file);
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
    printf(" 1)grayscale  2)sepia  3)blur  4)invert  5)gaussian blur 6)negative 7)reflect \n");
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
                stbi_write_jpg("C:\\Users\\ASUS\\CLionProjects\\project\\cmake-build-debug\\out_image.jpg",x,y,1,filtered,100);
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
                stbi_write_jpg("C:\\Users\\ASUS\\CLionProjects\\project\\cmake-build-debug\\out_image.jpg",x,y,channels,filtered,100);
                break;
            case 5:
                filtered= gaussian_filter(image,x,y,channels,3);
                stbi_write_jpg("C:\\Users\\ASUS\\CLionProjects\\project\\cmake-build-debug\\out_image.jpg",x,y,channels,filtered,100);
                break;
            case 6:
                filtered= negative(image,end_image,x,y,channels);
                stbi_write_jpg("C:\\Users\\ASUS\\CLionProjects\\project\\cmake-build-debug\\out_image.jpg",x,y,1,filtered,100);
                break;
            case 7:
                filtered= reflect_filter(image,x,y,channels);
                stbi_write_jpg("C:\\Users\\ASUS\\CLionProjects\\project\\cmake-build-debug\\out_image.jpg",x,y,channels,filtered,100);
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
            case 4:
                filtered= bmp_invert(image);
                save_bmp(filtered,"C:\\Users\\ASUS\\CLionProjects\\project\\out.bmp");
                break;
            case 6:
                filtered= bmp_negative( image);
                save_bmp(filtered,"C:\\Users\\ASUS\\CLionProjects\\project\\out.bmp");
                break;


        }
        //str out_address=".\\out.bmp";
        //save_bmp(filtered,out_address);
    }











    //stbi_image_free(image);

}