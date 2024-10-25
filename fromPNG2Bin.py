# fromPNG2Bin.py
from PIL import Image
import numpy as np
import sys
import struct

def convert_to_binary(input_path):
    # Abrir y convertir la imagen a escala de grises
    img = Image.open(input_path).convert('L')
    # Obtener dimensiones
    width, height = img.size
    
    # Convertir a array de numpy
    img_array = np.array(img, dtype=np.int32)
    
    # Crear el archivo binario
    output_path = input_path.rsplit('.', 1)[0] + '.bin'
    with open(output_path, 'wb') as f:
        # Escribir las dimensiones al inicio del archivo (8 bytes en total)
        f.write(struct.pack('II', width, height))
        # Escribir los datos de la imagen
        img_array.tofile(f)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Uso: python3 fromPNG2Bin.py <imagen.jpg/png>")
        sys.exit(1)
    
    convert_to_binary(sys.argv[1])