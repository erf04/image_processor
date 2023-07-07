#include <stdio.h>
#include <string.h>
#include "curl/curl.h"
#include <stdlib.h>
#include "email.h"
#define MAX_LENGTH 10000
// Callback function to write attachment data to file
size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    FILE* file = (FILE*)userp;
    size_t written = fwrite(contents, size, nmemb, file);
    return written;
}

char* my_find_format(char* string){
    char s[5];int i;
    for( i=0;i!=5;i++){
        s[i]=string[i];
    }
    s[i]='\0';
    if (strstr(s,"/9j")){
        return "jpg";
    }
    else if (strstr(s,"Qk")){
        return "bmp";
    }
    else{
        return "png";
    }
}
char* format;
char* fetch(const char* input_address) {
    FILE *input = fopen(input_address, "r");
    fseek(input, 0L, SEEK_END);
    long file_size;
    file_size = ftell(input);

    rewind(input);
    char *input_text = malloc(file_size);
    fread(input_text, file_size, 1, input);
    //printf("-------------\n%s", input_text);
    char *p = strstr(input_text, "X-Attachment-Id: ");
    int i=0;char c;
    while(*(p+i)!='\r' && *(p+i)!='\n'){
        p+=sizeof (char);
        i++;
    }
    while(*(p+(i))=='\r' || *(p+(i)) =='\n'){
        i++;
    }
    char* image_text= malloc(sizeof (char)* 100000);
    //image_text[0]=*(p+(i));
    int j=0;
    while(*(p+i)!='-'){
        image_text[j]=*(p+i);
        i++;j++;
    }
    int len=j;
    image_text[j]='\0';
    format= my_find_format(image_text);
    char* out_address="out.txt";
    FILE * out_file= fopen(out_address,"w");
    fwrite(image_text,len,1,out_file);
    fclose(out_file);
    return out_address;

}

char* decode(char* out_address){
    char  buffer[1000];char* out_image_address= malloc(sizeof (char)* 10000);
    sprintf(out_image_address,"C:\\Users\\acer\\CLionProjects\\project\\output.%s",format);
    sprintf(buffer,"certutil -decode %s %s",out_address,out_image_address);
    system(buffer);
    return out_image_address;

}
char* get_email() {
    CURL* curl;
    CURLcode res;
    FILE* file;

    // Set up libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        // Set IMAP server and account details
        char email[] = "project.c.dr.hamzeh@gmail.com"; // Replace with your Gmail address
        char password[] = "jkxlkjudethkrjju"; // Replace with your Gmail password
        curl_easy_setopt(curl, CURLOPT_CAINFO, "C:\\Users\\acer\\CLionProjects\\email\\cacert-2023-05-30.pem");

        // Set the mailbox name and UID
        char mailbox[] = "INBOX";
        int uid = 6;

        // Create the URL with the UID
        char url[100];
        sprintf(url, "imaps://imap.gmail.com/%s;UID=%d", mailbox, uid);

        // Connect to the IMAP server and log in
        curl_easy_setopt(curl, CURLOPT_USERNAME, email);
        curl_easy_setopt(curl, CURLOPT_PASSWORD, password);
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Perform the fetch request
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            printf("Failed to fetch email: %s\n", curl_easy_strerror(res));
            return NULL;
        }

        // Open a file to save the fetched email
        file = fopen("C:\\Users\\acer\\CLionProjects\\email\\fetch.txt", "wb");
        if (!file) {
            printf("Error opening file for writing.");
            return NULL;
        }

        // Set the write callback function to write data into the file
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

        // Perform the fetch request and save the email content to the file
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            printf("Failed to fetch email: %s\n", curl_easy_strerror(res));
            return NULL;
        }

        // Clean up and close the file
        fclose(file);

        // Clean up and close the IMAP session
        curl_easy_cleanup(curl);
    } else {
        printf("Error initializing libcurl.\n");
        return NULL;
    }

    // Clean up libcurl
    curl_global_cleanup();
    char * out=fetch("C:\\Users\\acer\\CLionProjects\\email\\fetch.txt");

    char* address=decode(out);
    //b64decode("C:\\project\\fetch\\fetch.txt","C:\\project\\fetch\\fetch.txt");

    return address;
}