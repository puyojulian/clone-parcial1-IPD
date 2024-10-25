# fromBin2PNG.py
from PIL import Image
import numpy as np
import sys
import struct

def convert_to_image(input_path):
    # Leer el archivo binario
    with open(input_path, 'rb') as f:
        # Leer las dimensiones del encabezado (8 bytes)
        width, height = struct.unpack('II', f.read(8))
        
        # Leer los datos de la imagen
        img_array = np.fromfile(f, dtype=np.int32)
        
        # Reshape el array según las dimensiones
        img_array = img_array.reshape((height, width))
        
        # Normalizar valores si es necesario (0-255)
        img_array = np.clip(img_array, 0, 255).astype(np.uint8)
        
        # Crear imagen
        img = Image.fromarray(img_array)
        
        # Guardar imagen
        output_path = input_path.rsplit('.', 1)[0] + '.png'
        img.save(output_path)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Uso: python3 fromBin2PNG.py <imagen.bin>")
        sys.exit(1)
    
    convert_to_image(sys.argv[1])