#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

// Estructura para el encabezado de la imagen
typedef struct {
    int width;
    int height;
} ImageHeader;

// Declaraciones de funciones corregidas
void cargarImagen(int *imagen, int *width, int *height, const char *filename);
void guardarImagen(int *imagen, int width, int height, const char *filename);
void aplicarFiltro(int *imagen, int *imagenProcesada, int width, int height);
int calcularSumaPixeles(int *imagen, int width, int height);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Dar un nombre de archivo de entrada\n");
        exit(1);
    }

    const char *filename = argv[1];
    int width, height;
    
    // Primero leemos las dimensiones
    FILE *archivo = fopen(filename, "rb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        exit(1);
    }
    
    // Leer las dimensiones
    if (fread(&width, sizeof(int), 1, archivo) != 1 ||
        fread(&height, sizeof(int), 1, archivo) != 1) {
        perror("Error al leer las dimensiones");
        fclose(archivo);
        exit(1);
    }
    fclose(archivo);

    // Ahora podemos asignar memoria con las dimensiones correctas
    int *imagen = (int *)malloc(width * height * sizeof(int));
    int *imagenProcesada = (int *)malloc(width * height * sizeof(int));
    
    if (imagen == NULL || imagenProcesada == NULL) {
        perror("Error al asignar memoria");
        exit(1);
    }

    omp_set_num_threads(32);

    // Cargar la imagen con el filename
    cargarImagen(imagen, &width, &height, filename);

    // Aplicar filtro
    aplicarFiltro(imagen, imagenProcesada, width, height);

    // Calcular suma de píxeles
    int sumaPixeles = calcularSumaPixeles(imagenProcesada, width, height);
    printf("Suma de píxeles: %d\n", sumaPixeles);

    // Guardar la imagen con el filename
    guardarImagen(imagenProcesada, width, height, filename);

    free(imagen);
    free(imagenProcesada);
    return 0;
}

void cargarImagen(int *imagen, int *width, int *height, const char *filename) {
    FILE *archivo = fopen(filename, "rb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo para cargar la imagen");
        exit(1);
    }

    // Leer las dimensiones del archivo (8 bytes en total)
    if (fread(width, sizeof(int), 1, archivo) != 1 ||
        fread(height, sizeof(int), 1, archivo) != 1) {
        perror("Error al leer las dimensiones de la imagen");
        fclose(archivo);
        exit(1);
    }

    // Leer los datos de la imagen
    size_t elementosLeidos = fread(imagen, sizeof(int), (*width) * (*height), archivo);
    if (elementosLeidos != (*width) * (*height)) {
        perror("Error al leer la imagen desde el archivo");
        fclose(archivo);
        exit(1);
    }

    fclose(archivo);
}

void guardarImagen(int *imagen, int width, int height, const char *filename) {
    char *output_filename = (char*)malloc(strlen(filename) + 5); // +5 para ".new\0"
    sprintf(output_filename, "%s.new", filename);
    
    FILE *archivo = fopen(output_filename, "wb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo para guardar");
        free(output_filename);
        return;
    }

    // Escribir el encabezado
    ImageHeader header = {width, height};
    if (fwrite(&header, sizeof(ImageHeader), 1, archivo) != 1) {
        perror("Error al escribir el encabezado");
        fclose(archivo);
        free(output_filename);
        return;
    }

    // Escribir los datos de la imagen
    if (fwrite(imagen, sizeof(int), width * height, archivo) != width * height) {
        perror("Error al escribir los datos de la imagen");
    }

    fclose(archivo);
    free(output_filename);
}

void aplicarFiltro(int *imagen, int *imagenProcesada, int width, int height) {
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // #pragma omp parallel for shared(imagen, imagenProcesada, width, height, Gx, Gy) schedule(dynamic)
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            int sumX = 0;
            int sumY = 0;

            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    sumX += imagen[(y + ky) * width + (x + kx)] * Gx[ky + 1][kx + 1];
                    sumY += imagen[(y + ky) * width + (x + kx)] * Gy[ky + 1][kx + 1];
                }
            }

            int magnitude = abs(sumX) + abs(sumY);
            imagenProcesada[y * width + x] = (magnitude > 255) ? 255 : magnitude;
        }
    }
}

int calcularSumaPixeles(int *imagen, int width, int height) {
    int suma = 0;
    // #pragma omp parallel for reduction(+:suma) schedule(static)
    for (int i = 0; i < width * height; i++) {
        suma += imagen[i];
    }
    return suma;
}