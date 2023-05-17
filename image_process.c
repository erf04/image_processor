
#include "stdlib.h"
#include "C:\Users\ASUS\CLionProjects\untitled1\CS50\pSet4\filter\less\helpers.h"
#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "C:\Users\ASUS\CLionProjects\untitled1\stb\stb_image_write.h"

#include "C:\Users\ASUS\CLionProjects\untitled1\stb\stb_image.h"


void gray(int height,int width,RGBTRIPLE image[height][width]){
    int average,i,j;int sum=0;int r,g,b;
    for (i=0;i!=width;i++){
        for(j=0;j!=height;j++){
            r=image[i][j].rgbtRed;b=image[i][j].rgbtBlue;g=image[i][j].rgbtGreen;
            sum+=r+g+b;
            average=(int)(sum/3.0);
            image[i][j].rgbtRed=average;image[i][j].rgbtGreen=average;image[i][j].rgbtBlue=average;
        }
    }
    return;
}

void sepia(int height ,int width,RGBTRIPLE image[height][width]){
    int i,j,r,g,b;
    for (i=0;i!=height;i++){
        for (j=0;j!=width;j++){
            r=image[i][j].rgbtRed;g=image[i][j].rgbtGreen;b=image[i][j].rgbtBlue;
            int rs=(int)(0.393 * r + 0.769 * g + 0.189 * b);
            int gs=(int )(0.349 * r + 0.686 * g + 0.168 * b);
            int bs=(int)(0.272 * r + 0.534 * g + 0.131 * b);
            if (rs>255)
                rs=255;
            image[i][j].rgbtRed=rs;
            if (gs>255)
                gs=255;
            image[i][j].rgbtGreen=gs;
            if (bs>255)
                bs=255;
            image[i][j].rgbtBlue=bs;
        }
    }
    return;

}

int main(){
    int width, height, bpp;
    unsigned char *rgb_image = stbi_load("C:\\Users\\ASUS\\CLionProjects\\untitled1\\image.jpg", &width, &height, &bpp, 3);

    stbi_image_free(rgb_image);
    //calling the function
    gray(height, width, rgb_image);
    //initialize new width and height
    int x = 800;
    int y = 800;

    uint8_t* rgb_image2;
    // get space
    rgb_image2 = malloc(x*y*3);

    // Write your code to populate rgb_image here

    stbi_write_png("C:\\Users\\ASUS\\CLionProjects\\untitled1\\out_image.png", x, y, 3, rgb_image2, x*3);

    return 0;
}
