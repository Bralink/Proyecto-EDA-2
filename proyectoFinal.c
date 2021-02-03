//Librerias que vamos a utilizar 
#include <stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<math.h>
//Stn master nos ayudará a manejar las imagenes 
#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb-master/stb_image_write.h"
//Caracteristicas  de una imagen 
#define COLOR_VALUE 256
#define QUALITY 100 
//Prototipo de funciones 

//void equilizeImage_secuencial(unsigned char *srcIma);
//void equilizeImage_paralell(unsigned char *srcIma);

unsigned char *empty_array_UC(long size);
long *empty_array_LONG(long size);

long *histogram_space(unsigned char *image, long size, int channels);

long *cumulative_distribution_function(long *histogram);
long min_cdf(long *cumulative_distribution);

long *equalized_value_distribution(long *cummulative_value, long min, long  size);
unsigned char *equalized_Image(unsigned char  *first_Image, int channels, long *equalized_value, long size);



int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Using wrong size of a image");
        return -1;
    }

    char *image_source = argv[1];
    int width, height, channels; 
    unsigned char *image = stbi_load(image_source, &width, &height, &channels, 0);
    double tIni, tSecuencia, tParalelo, tFinal; 

    if(image = NULL){
        printf("There is no image");
        return -1; 
    }
    
    long size = height * width; 
    long *new_histogram = histogram_space(image, size, channels);
    long *cdf  = cumulative_distribution_function(new_histogram); 
    long minimun_cdf = min_cdf(cdf);
    long *cumulative_distribution = equalized_value_distribution(cdf, minimun_cdf, size);

    unsigned char *equalizedImage = equalized_Image(image, channels, cumulative_distribution, size);
    long *equalized_histogram = histogram_space(equalizedImage, size, channels);

    const char *image_name = "./out/equalized_image.jpg";
    stbi_write_jpg("equalized_image.jpg", width, height, channels, equalizedImage, QUALITY); 
    return 0; 


}

//Creamos arreglos tipo UC y LONG para poder trabajar con la informacion 
unsigned char *empty_array_UC(long size){
    unsigned char *new_array_UC = malloc(sizeof(unsigned char) * size);
    for(int i = 0; i < size; i++ ){
        new_array_UC[i] = 0;
    }
    return new_array_UC; 
}

long *empty_array_LONG(long size){
    long *new_array_LONG = malloc(sizeof(long) * size);
    for(int i = 0; i < size; i++ ){
        new_array_LONG[i] = 0;
    }
    return new_array_LONG; 
}

long *histogram_space(unsigned char *image, long size, int channels){
    long *new_histogram = empty_array_LONG(COLOR_VALUE); 
    for(int i = 0; i < size * channels; i=i+channels){
        new_histogram[(int)image[i]]; //Aqui decimos que el valor del pixel en la imagen será el valor asignado en el new histogram
    }
    return new_histogram; 
}

long *cumulative_distribution_function(long *histogram){
    long *cdf = empty_array_LONG(COLOR_VALUE);
    cdf[0] = histogram[0];
    for(int i = 1; i< COLOR_VALUE; i++){
        cdf[i] = histogram[i] + cdf[i-1]; //sumas los valores 
    }
    return  cdf; 
}

long min_cdf(long *cdf){
    long min = cdf[0]; 
    for(int i = 0; i < COLOR_VALUE; i++){
        if(min > cdf[i]){
            min = cdf[i];
        }
    }
    return min; 
}

long *equalized_value_distribution(long *cummulative_value, long min, long  size){
    long *equilizeImage_value = empty_array_LONG(COLOR_VALUE);
    double factor = ((double) COLOR_VALUE - 2) / ((double) size - min); 
    for(int i=0; i<COLOR_VALUE; i++){
        equilizeImage_value[i] = round( ((double)(cummulative_value[i] - min) * factor) + 1); //SI esto no funciona separarlo en una variable
    }
    return equilizeImage_value; 
}

unsigned char *equalized_Image(unsigned char  *first_Image, int channels, long *equalized_value, long size){
    unsigned char *second_Image = empty_array_UC(size* channels); 
    for(int i = 0; i < size*channels; i += channels){
        unsigned char original_image_value = first_Image[i]; 
        if(i % channels == 0){
            second_Image[i] = (unsigned char) equalized_value[original_image_value];
        }else{
            second_Image[i] = (unsigned char) original_image_value;
        }
        return second_Image; 
    }
}