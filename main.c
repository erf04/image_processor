#include <stdio.h>
#include "stdlib.h"
#include "filters.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb\stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb\stb_image_write.h"
#include "curl/curl.h"

typedef const char* str;
int get_int(){
    char c;int x=0;
    while((c=(char)getchar())!='\n' && c!=EOF){
        x=x*10+(c-'0');
    }
    return x;
}
char* get_str(int* len){
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

int find_len(str string){
    int i=0;
    while(string[i++]!='\0');
    return i;
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
char* my_strcat(char* address,char* format){
    int i=0;
    while(address[i]!='\0')
        i++;
    for(int j=0;j!=4;j++){
        address[i++]=format[j];
    }
    return address;
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
            fread(&(pixel.r),sizeof (unsigned char),1,file);
            fread(&(pixel.g),sizeof (unsigned char),1,file);
            fread(&(pixel.b),sizeof (unsigned char),1,file);
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
            fwrite(&(pixel.b), sizeof(unsigned char), 1, out_file);
            fwrite(&(pixel.g), sizeof(unsigned char), 1, out_file);
            fwrite(&(pixel.r), sizeof(unsigned char), 1, out_file);
        }
        for (int i=0;i<padding;i++){
            fputc(0x00,out_file);
        }
        //unsigned char padByte = 0;
        //fwrite(&padByte, sizeof(unsigned char), padding, out_file);
    }
    fclose(out_file);
}



void get_from_url(str url,str outfilename)
{

    char buffer[1000];
    snprintf(buffer,1000,"wget %s -O %s",url,outfilename);
    system(buffer);


}





int main(){
    //CURL * curl=curl_easy_init();
    int x,y,channels,len;
    printf("welcome to project sagi!\n");
    printf("choose the way:1)local 2)url\n");
    int mode=get_int(),n;char* address= malloc(sizeof (char)*100);
    if (mode==1){
        printf("put the image address:");
        address=get_str(&len);
        n = find_format(address,len);
    }
    else{
        printf("put the image url:");
        str url=get_str(&len);
        n=find_format(url,len);
        address="image.jpg";
        /*if (n==1){
            my_strcat(address,".jpg");
        }
        else if (n==2){
            strcat(address,".png");
        }
        else{
            strcat(address,".bmp");
        }
         */
        get_from_url(url,address);
    }


    printf("choose the filter :");
    printf(" 1)grayscale  2)sepia  3)blur  4)invert\n 5)gaussian blur 6)negative 7)reflect(vertical) 8)reflect(horizontal) \n");
    int filter=get_int();

    //address="C:\\Users\\ASUS\\CLionProjects\\project\\Duck.jpg";
    //str address=".\\input.bmp";

    str format;
    str out_format;

    printf("your image format is %s\n",format);
    //printf("you can write the filtered image in these formats: %s\n",out_format);
    //printf("please choose one:");
    //int out_choose_format=get_int();
   // printf("put your output address\n(if your dont ,the program will choose a address for you):");
    //str out_address= get_str(&len);
    str out_address;
    if (n==1 || n==2){
        if (n==1){
            out_address="out_image.jpg";
        }
        else{
            out_address="out_image.png";
        }

        unsigned char *image= stbi_load(address,&x,&y,&channels,0);

        int image_size=x*y*channels;
        unsigned char* filtered;
        unsigned char *end_image=(unsigned char *)(image_size+image);
        switch (filter) {
            case 1:
                filtered=gray(image,end_image,x,y,channels);
                if (n==1){
                    stbi_write_jpg(out_address,x,y,1,filtered,100);
                }
                else{
                    stbi_write_png(out_address,x,y,1,filtered,0);
                }


                break;
            case 2:
                filtered=sepia(image,end_image,x,y,channels);
                if (n==1){
                    stbi_write_jpg(out_address,x,y,channels,filtered,100);
                }
                else{
                    stbi_write_png(out_address,x,y,channels,filtered,0);
                }

                break;
            case 3:
                filtered=blur(image,end_image,x,y,channels);
                if (n==1){
                    stbi_write_jpg(out_address,x,y,channels,filtered,100);
                }
                else{
                    stbi_write_png(out_address,x,y,channels,filtered,0);
                }

                break;
            case 4:
                filtered=invert(image,end_image,x,y,channels);
                if (n==1){
                    stbi_write_jpg(out_address,x,y,channels,filtered,100);
                }
                else{
                    stbi_write_png(out_address,x,y,channels,filtered,0);
                }
                break;
            case 5:
                filtered= gaussian_filter(image,x,y,channels,3);
                if (n==1){
                    stbi_write_jpg(out_address,x,y,channels,filtered,100);
                }
                else{
                    stbi_write_png(out_address,x,y,channels,filtered,0);
                }
                break;
            case 6:
                filtered= negative(image,end_image,x,y,channels);
                if (n==1){
                    stbi_write_jpg(out_address,x,y,1,filtered,100);
                }
                else{
                    stbi_write_png(out_address,x,y,1,filtered,0);
                }
                break;
            case 7:
                filtered= reflect_filter(image,x,y,channels);
                if (n==1){
                    stbi_write_jpg(out_address,x,y,channels,filtered,100);
                }
                else{
                    stbi_write_png(out_address,x,y,channels,filtered,0);
                }
                break;


        }
    }
    if (n==3){
        str out_address="out_image.bmp";
        BMPImage *image= read_bmp(address);
        BMPImage * filtered;
        switch (filter) {
            case 1:
                filtered=bmp_gray(image);
                save_bmp(filtered,out_address);
                break;
            case 2:
                filtered= bmp_sepia(image);
                save_bmp(filtered,out_address);
                break;
            case 3:
                filtered=bmp_blur(image);
                save_bmp(filtered,out_address);
                break;
            case 4:
                filtered= bmp_invert(image);
                save_bmp(filtered,out_address);
                break;
            case 6:
                filtered= bmp_negative( image);
                save_bmp(filtered,out_address);
                break;
            case 7:
                filtered=bmp_reflect(image);
                save_bmp(filtered,out_address);
                break;
            case 8:
                filtered=bmp_reflect2(image);
                save_bmp(filtered,out_address);
                break;


        }
        //str out_address=".\\out.bmp";
        //save_bmp(filtered,out_address);
    }











    //stbi_image_free(image);

}