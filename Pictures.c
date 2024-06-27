#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define NAME_LEN 30
#define PIC_LEN 30



typedef struct{
    int id;
    char name[NAME_LEN+1];
    char pic_name[PIC_LEN+1];
    float price;
}Picture;



void addPicture(Picture **pic_arr, unsigned *size) {
    int n;
    while (true) {
        printf("Enter number of items to expand: ");
        scanf("%d", &n);
        if (n >= 3 && n <= 30) {
            break;
        } else {
            puts("Invalid input for n. Try again!");
        }
    }



    *size += n;
    Picture *expanded_arr = realloc(*pic_arr, *size * sizeof(Picture));
    if (expanded_arr == NULL) {
        perror("Failed to expand");
        exit(1);
    }
    *pic_arr = expanded_arr;



    for (int i = 0; i < n; i++) {
        printf("Picture %d:\n", i + 1);



        while (true) {
            printf("ID: ");
            scanf("%d", &((*pic_arr)[i].id));
            fflush(stdin);
            if ((*pic_arr)[i].id > 999 && (*pic_arr)[i].id < 10000) {
                break;
            } else {
                puts("Invalid input for id. Try again!");
            }
        }



        printf("Painter Name: ");
        fgets((*pic_arr)[i].name, NAME_LEN, stdin);
        (*pic_arr)[i].name[strlen((*pic_arr)[i].name) - 1] = '\0';



        printf("Picture Name: ");
        fgets((*pic_arr)[i].pic_name, PIC_LEN, stdin);
        (*pic_arr)[i].pic_name[strlen((*pic_arr)[i].pic_name) - 1] = '\0';



        printf("Price: ");
        scanf("%f", &((*pic_arr)[i].price));
        fflush(stdin);
    }
}




float average(Picture pic_arr[], unsigned size, float price){
    float sum = 0.00;
    int counter = 0;
    for(int i = 0; i<size; ++i){
        if(pic_arr[i].price > price){
            sum += pic_arr[i].price;
            counter ++;
        }
    }
    float avg = sum/counter;
    if(avg != 0){
        return avg;
    }
    else return 0.0;
}



int num_input(Picture pic_arr[], unsigned size, char letter){
    FILE *f;
    int counter = 0;
    f = fopen("info.txt", "a");
    if(f == NULL){
        perror("Failed to open text file!");
        exit(2);
    }
    for(int i = 0; i<size; ++i){
        if(pic_arr[i].name[0] == letter){
            fprintf(f, "%d; %s; %s; %.2f\n", pic_arr[i].id,pic_arr[i].name,
                    pic_arr[i].pic_name, pic_arr[i].price);
            counter++;
        }
    }
    fclose(f);
    return counter;
}



void printPic(char name[]){
    FILE *f;
    bool flag = true;
    Picture pic;
    unsigned len;



    if((f = fopen("picture.bin", "rb"))==NULL){
        perror("Failed to open bin file!");
        exit(3);
    };



    while(true){
        if(fread(&pic.id, sizeof(int), 1, f)!=1){
            if(!feof(f)){
                perror("Error!");
                fclose(f);
                exit(4);
            }
            else break;
        }
        if(fread(&len, sizeof(unsigned), 1, f)!=1){
            perror("Reading Error!");
            fclose(f);
            exit(5);
        }
        if(fread(&pic.name, sizeof(char), len, f)!=len){
            perror("Reading Error!");
            fclose(f);
            exit(6);
        }
        pic.name[len] = '\0';
        if(fread(&pic.pic_name, sizeof(char), PIC_LEN-1, f)!=PIC_LEN-1){
            perror("Reading Error!");
            fclose(f);
            exit(7);
        }
        pic.pic_name[PIC_LEN-1] = '\0';
        if(fread(&pic.price, sizeof(float), 1, f)!=1){
            perror("Reading Error!");
            fclose(f);
            exit(8);
        }
        if(strcmp(pic.name, name) == 0){
            if(!flag){
                puts("----------\n");
            }
            else{
                flag = false;
                printf("Bin ID: %d\n", pic.id);
                printf("Bin Name: %s\n", pic.name);
                printf("Bin Picture Name: %s\n", pic.pic_name);
                printf("Bin Price: %.2f\n", pic.price);
            }
        }
    }
    if(flag){
        puts("No matches found!");
    }
    fclose(f);
}



void writeToBin(Picture pic_arr[], unsigned size){
    FILE *f = fopen("picture.bin","wb");
    unsigned len;



    if(f==NULL){
        perror("Writing error!");
        exit(3);
    }
    for(int i=0;i<size;++i){
        fwrite(&pic_arr[i].id,sizeof(int),1,f);
        len = strlen(pic_arr[i].name);
        fwrite(&len,sizeof(unsigned),1,f);
        fwrite(pic_arr[i].name,sizeof(char),len,f);
        fwrite(pic_arr[i].pic_name,sizeof(char),PIC_LEN-1,f);
        fwrite(&pic_arr[i].price, sizeof(float),1,f);
    }
    fclose(f);
}



int main()
{
    const int NUM_PICTURES = 2;
    unsigned size = NUM_PICTURES;
    Picture *pic_arr = malloc(size * sizeof(Picture));
    if (pic_arr == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }



    for (int i = 0; i < NUM_PICTURES; ++i) {
        addPicture(&pic_arr, &size);
    }
    puts("-----------------------------------\n");
    float price;
    printf("Enter price: ");
    scanf("%f", &price);
    printf("Average of picture prices above %.2f: %.2f\n",price, average(pic_arr, NUM_PICTURES, price));
    puts("-----------------------------------\n");
    char letter;
    printf("Enter a letter: ");
    scanf(" %c", &letter);
    int numPainters = num_input(pic_arr, NUM_PICTURES, letter);
    printf("Number of painters whose name starts with % c: %d\n", letter, numPainters);
    puts("-----------------------------------\n");
    /*char name[NAME_LEN];
    printf("Enter name to search for: \n");
    fgets(name, NAME_LEN, stdin);
    fflush(stdin);
    name[strlen(name)-1] = '\0';*/
    writeToBin(pic_arr, NUM_PICTURES);
    printPic("A");
    return 0;
}
