
> **NOTA:** Este es repositorio es una copia del repositorio: [paralelas-distribuidas-1er-parcial](https://gitlab.com/john.sanabria/paralelas-distribuidas-1er-parcial.git/) del usuario (Maestro): [@john.sanabria](https://gitlab.com/john.sanabria), con el fin de resolver el primer examen parcial del curso Infraestructuras Paralelas y Distribuidas de la Universidad del Valle.

# Conclusiones y Resultados: Implementación Original (*main branch*)

## Resultados

Para comenzar, presento los datos recopilados (tiempos de ejecución en segundos). En un principio se ejecutó el programa con la modificación indicada para la función [aplicarFiltro](main.c). Con ello se consiguieron los siguientes resultados.

> Todas las pruebas fueron realizadas en un equipo con un procesador Ryzen 5700x de 8 núcleos y 16 hilos y 32Gb de RAM 3600Mhz en un SSD 1TB M.2 (con una velocidad nominal de lectura secuencial: hasta 2500 MB/s y escritura secuencial: hasta 2100 MB/s).

**Llamado Secuencial:**
- 0:05.97
- 0:05.81
- ~~0:05.77~~ - *a eliminar*
- 0:06.16
- 0:05.98

**OMP con Llamado Secuencial (optimizado con la bandera ```-O3``` al compilar - 16 hilos):**
- ~~0:05.32~~ - *a eliminar*
- 0:05.26
- 0:05.14
- 0:05.28
- 0:05.13 \
**Speedup:** (23.92/4)/(20.81/4) = 1.1494

> El *speedup* se calcula dividiento el tiempo promedio secuencial sobre el tiempo promedio en paralelo. Considérese que cuando hablamos de tiempo de ejecución, entre menos en mejor.

**OMP con Llamado Secuencial (optimizado con ```-O3``` - 32 hilos):**
- ~~0:05.16~~ - *a eliminar*
- 0:05.04
- 0:05.07
- 0:05.09
- 0:05.05 \
**Speedup:** (23.92/4)/(20.25/4) = 1.1812

**OMP con Llamado Secuencial (optimizado con ```-O3``` - 100 hilos):**
- ~~0:05.26~~ - *a eliminar*
- 0:05.17
- 0:05.12
- 0:05.14
- 0:05.19 \
**Speedup:** (23.92/4)/(21.03/4) = 1.1600

Lo anterior nos confirma que el punto *dulce* para paralelizar el programa lo encontramos al establecer 32 hilos para la ejecución de cada sección paralela, donde se alcanza una mejora en rendimiento del 18.12%; superior a las ejecuciones con 16 y 100 hilos. Y, aunque puede parecer menos de lo esperado, hay que tener en cuenta que al tratarse de imágenes estámos muy dependientes de las operaciones de entrada y salida, además que todo el tiempo de ejecución también depende de los tiempos que se tomen [fromBin2PNG.py](fromBin2PNG.py) y [fromPNG2Bin.py](fromPNG2Bin.py) los cuales se ejecutan de manera secuencial. Por lo tanto, encontramos una mejora que no es sustancial y que se puede mejorar si consideramos que el llamado desde la consola o desde nuestro [all.sh](all.sh), podemos realizar el llamado de nuestro programa de manera paralela.

> En [all.sh](all.sh) encontrará comentada la implementación original como aquella correspondiente a la prueba anterior (sobre todas las imágenes en *images*), y estará sin comentario aquel script definitivo, el cual hace un llamado en paralelo según los recursos disponibles. Los resultados se encuentran a continuación: 


**OMP con Llamado Paralelo (optimizado con -O3 - 100 hilos):**
- 0:01.48
- ~~0:01.62~~ - *a eliminar*
- 0:01.54
- 0:01.57
- 0:01.51 \
**Speedup:** (23.92/4)/(8.02/4) = 3.9246

Lo cual corresponde a una ganancia de rendimiento de un 292.46%, Lo cual es un incremento mucho más significativo comparándolo con haber aplicado solo las modificaciones con OpenMP.

## Conclusión

Tal como se comentó anteriormente, a pesar de los cambios realizados con OpenMP para lograr paralelismo, y aún aplicando la optimización que brinda compilar con ```-O3``` (``` gcc -fopenmp -O3 -o main main.c ```). 

> Se optó por ```-O3``` debido a que se vió una mejora inmediata sobre los resultados encontrados en ese entonces, antes los tiempos eran mucho mas cercanos a los secuenciales.

No se logró un incremento considerable sobre el rendimiento ganado, por lo que se optó por buscar formas de paralelizar el llamado de las tareas desde el *shell*.

Dicha optimización o forma de paralelizar permitió un mayor aprovechamiento de los recursos. Ya no existe la cola que obliga a esperar a que operaciones como la lectura y escritura de los archivos fueran realizadas.

Adicionalmente, cabe reconocerse que la implementación de OpenMP fue muy cautelosa y se procuró mantener un valor de hilos y un tipo de *scheduling* acorde al tipo de operaciones a utilizar en cada de sección paralela. En el caso de *aplicarFiltro* se optó por un *scheduling* *dinámico* mientras que en *calcularSumaPixeles* se dejó uno *estático*, con el fin de mantener la correctitud procurando el mayor uso de los recursos en el menor tiempo posible.

# Conclusiones y Resultados: Implementación Corregida (*feature/image-fix branch*)

Luego de revisar el código se notó que la razón por la cual las imágenes no tenían el filtro bien aplicado era porque estaban *"hardcodeadas"* las dimensiones de la imagen, lo cual se pudo corregir, modificando los archivos [fromBin2PNG.py](fromBin2PNG.py) y [fromPNG2Bin.py](fromPNG2Bin.py), así como el archivo [main.c](main.c) para conseguir tal fin. Entonces ya con una lógica capaz de procesar las imágenes según su tamaño, claramente los tiempos de cómputo incrementaron.

Se tuvieron los siguientes resultados (en *minutos:segundos*):

**Llamado Bash Paralelo sin OMP**
- 0:03.07
- 0:03.28
- 0:03.22
- 0:03.12
- 0:03.18

**Llamado Bash Paralelo con OMP**
- 0:03.52
- 0:03.18
- 0:03.47
- 0:03.18
- 0:03.19

**Llamado Bash Secuencial sin OMP**
- 0:11.48
- 0:11.47
- 0:11.42
- 0:11.66
- 0:11.59

**Llamado Bash Secuencial con OMP**
- 0:11.86
- 0:11.84
- 0:11.70
- 0:11.75
- 0:11.76 

A simple vista logramos notar que no va a haber un speedup a favor (*speedup* <= 1), cada vez que pasamos a una versión con OMP perdemos rendimiento, y por versión me refiero al tipo de llamado de los programas desde *Bash*.

Si fue en secuencial:

```BASH
for i in {1..18}
do
    INPUT_JPG="images/image${i}.jpg"
    TEMP_FILE="images/image${i}.bin"
    
    python3 fromPNG2Bin.py "${INPUT_JPG}"
    ./main "${TEMP_FILE}"
    python3 fromBin2PNG.py "${TEMP_FILE}.new"
```

O si fue en paralelo:

```BASH
for i in {1..18}
do
    (
        python3 fromPNG2Bin.py "images/image${i}.jpg"
        ./main "images/image${i}.bin"
        python3 fromBin2PNG.py "images/image${i}.bin.new"
    ) &
    if (( $(jobs | wc -l) >= $(nproc) )); then
        wait -n
    fi
done
wait
```

En ambos casos, al implementar los mismas directivas de OMP con 32 hilos, perdemos rendimiento. Sigue habiendo un beneficio notorio al hacer los llamados de manera asíncrona desde *Bash*.

# Procesamiento de imágenes

Este directorio contiene un conjunto de códigos que permiten aplicar un conjunto de filtros básicos a una imagen usando el lenguaje C.
Los filtros que se pueden aplicar son:

- Filtro de desenfoque.
- Filtro de detección de bordes.
- Filtro de Realce. 

El programa `main.c` contiene el código para aplicar estos filtros.
Este programa lee la información de los bits que representan una imagen y hace las transformaciones necesarias.
Se han desarrollado dos scripts en Python que permiten extraer los bits de información de una imagen en formato PNG (`fromPNG2Bin.py`) y toma un conjunto de bits y los almacena de regreso en una imagen en formato PNG (`fromBin2PNG.py`).

Se ha desarrollado un script en Bash llamado `all.sh` y el cual integra los códigos descritos anteriormente para aplicar un filtro a una imagen. 

El `Makefile` permite la compilación y ejecución de los códigos/archivos descritos anteriormente.

- `make all` permite la compilación y la ejecución del programa.
- `make compile` permite la compilación del programa `main.c`.
- `make clean` borra archivos creados durante la compilación y la ejecución del programa.

## Descripción 

El script `all.sh` usa tres programas le aplican un filtro a una imagen PNG de 1024x1024.

El script `all.sh` contiene lo siguiente:

```
#!/usr/bin/env bash
INPUT_PNG="image.png"
TEMP_FILE="image.bin"
python3 fromPNG2Bin.py ${INPUT_PNG}
./main ${TEMP_FILE}
python3 fromBin2PNG.py ${TEMP_FILE}.new
```

Los tres programas que se usan son `fromPNG2Bin.py`, `fromBin2PNG.py` y `main`. 
En este caso se asume que `image.png` es una imagen PNG de 1024x1024.
Lo que hace el script `fromPNG2Bin.py` es convertir la imagen de formanto PNG a una secuencia de pixeles.
Esa secuencia de pixeles queda almacenada en `image.bin` (`${TEMP_FILE}`).
Sobre los datos en `image.bin` se aplica un filtro que está en el archivo `./main`. 
Al terminar la ejecución del programa `main` se genera un archivo en este caso llamado `image.bin.new`.
El archivo `image.bin.new` es pasado al script `fromBin2PNG.py` y genera un nuevo archivo llamado `image.bin.PNG`. 
Este archivo es el archivo que contiene el PNG alterado.

## Otros posibles filtros

Para usar los filtros que se presentan a continuación se debe cambiar la función `aplicarFiltro` que se encuentra en el programa `main.c`.

### Filtro de desenfoque (Blur Filter)

```
void aplicarFiltro(int *imagen, int *imagenProcesada, int width, int height) {
    // Recorre cada píxel de la imagen (excepto los bordes)
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            // Promedia los valores de los píxeles vecinos
            int sum = 0;
            sum += imagen[(y - 1) * width + (x - 1)];  // Superior Izquierda
            sum += imagen[(y - 1) * width + x];        // Superior
            sum += imagen[(y - 1) * width + (x + 1)];  // Superior Derecha
            sum += imagen[y * width + (x - 1)];        // Izquierda
            sum += imagen[y * width + x];              // Centro
            sum += imagen[y * width + (x + 1)];        // Derecha
            sum += imagen[(y + 1) * width + (x - 1)];  // Inferior Izquierda
            sum += imagen[(y + 1) * width + x];        // Inferior
            sum += imagen[(y + 1) * width + (x + 1)];  // Inferior Derecha

            imagenProcesada[y * width + x] = sum / 9;  // Asigna el promedio al píxel procesado
        }
    }
}

```

### Filtro de detección de bordes (Edge Detection) - Filtro Sobel

```
void aplicarFiltro(int *imagen, int *imagenProcesada, int width, int height) {
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            int sumX = 0;
            int sumY = 0;

            // Aplicar máscaras de Sobel (Gx y Gy)
            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    sumX += imagen[(y + ky) * width + (x + kx)] * Gx[ky + 1][kx + 1];
                    sumY += imagen[(y + ky) * width + (x + kx)] * Gy[ky + 1][kx + 1];
                }
            }

            // Calcular magnitud del gradiente
            int magnitude = abs(sumX) + abs(sumY);
            imagenProcesada[y * width + x] = (magnitude > 255) ? 255 : magnitude;  // Normalizar a 8 bits
        }
    }
}

```

### Filtro de Realce (Sharpen Filter)

```
void aplicarFiltro(int *imagen, int *imagenProcesada, int width, int height) {
    int kernel[3][3] = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            int sum = 0;

            // Aplicar kernel de realce
            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    sum += imagen[(y + ky) * width + (x + kx)] * kernel[ky + 1][kx + 1];
                }
            }

            // Clampeo del valor para que esté entre 0 y 255
            imagenProcesada[y * width + x] = (sum > 255) ? 255 : (sum < 0) ? 0 : sum;
        }
    }
}

```

