from itertools import product
import numpy as np
from PIL import Image
import os

def tile(filename, dir_in, dir_out, d):
    name, ext = os.path.splitext(filename)
    x = Image.open(os.path.join(dir_in, filename))
    x=x.convert('L') #makes it greyscale
    y = np.asarray(x.getdata(),dtype=np.float64).reshape((x.size[1],x.size[0]))
    y = np.asarray(y,dtype=np.uint8) #if values still in range 0-255! 
    img = Image.fromarray(y,mode='L')

    w, h = img.size
    
    grid = list(product(range(0, h-h%d, d), range(0, w-w%d, d)))
    for i, j in grid:
        box = (j, i, j+d, i+d)
        out = os.path.join(dir_out, f'{name}_{i}_{j}{ext}')
        img.crop(box).save(out)

# usage:
# tile(img_name, "imput_folder", "output_folder", resolution)
tile("61to72.png", "imput_folder", "output_folder", 600)