#!/usr/bin/env bash
#
# Este script se encarga de invocar los tres programas que permiten la 
# conversion de un PNG a secuencia de pixeles, se procesan esos pixeles y
# posteriormente se convierte esa secuencia de pixeles a un archivo en formato
# PNG
#
# Autor: John Sanabria - john.sanabria@correounivalle.edu.co
# Fecha: 2024-08-22
# Modificado: Julian Ernesto Puyo Mora - julian.puyo@correounivalle.edu.co
#

# # Versión Original
# INPUT_PNG="image.png"
# TEMP_FILE="image.bin"
# python3 fromPNG2Bin.py ${INPUT_PNG}
# ./main ${TEMP_FILE}
# python3 fromBin2PNG.py ${TEMP_FILE}.new

# # Itera sobre las 18 imágenes de manera secuencial.
# for i in {1..18}
# do
#     INPUT_JPG="images/image${i}.jpg"
#     TEMP_FILE="images/image${i}.bin"
    
#     python3 fromPNG2Bin.py "${INPUT_JPG}"
#     ./main "${TEMP_FILE}"
#     python3 fromBin2PNG.py "${TEMP_FILE}.new"
# done

# Itera sobre las 18 imágenes de manera paralela, según la cantidad de núcleos lógicos.
for i in {1..18}
do
    (
        python3 fromPNG2Bin.py "images/image${i}.jpg"
        ./main "images/image${i}.bin"
        python3 fromBin2PNG.py "images/image${i}.bin.new"
    ) &
    # Limita el número de trabajos paralelos
    if (( $(jobs | wc -l) >= $(nproc) )); then
        wait -n  # Espera a que cualquier trabajo termine.
    fi
done
wait