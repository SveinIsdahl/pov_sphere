from PIL import Image
import numpy as np


#This code is not effective at all, but is sufficient for single images and a good starting point for continuous image translation (video)
im = Image.open('mario.png')
pixels = list(im.getdata())

image_width = 16

#Turn single array into 2D array
temp_hor_line = []
img = []
for i, pixel in enumerate(pixels):
    temp_hor_line.append(pixel)
    if(((i + 1) % image_width) == 0):
        img.append(temp_hor_line)
        temp_hor_line = []
img = [[img[j][i] for j in range(len(img))] for i in range(len(img[0]))]
for line in img:
    line.reverse()
    
# Write img to .h file
f = open("img.h", "w")
f.close()
f = open("img.h", "a")
f.write("#include \"pixel.h\"\n")
f.write("Pixel image[IMG_W][IMG_H] = {\n")
for hor_line in img:
    f.write("{")
    for pixel in hor_line:
        f.write("{")
        for val in pixel:
            f.write(hex(val))
            f.write(", ")
        f.write("},")
    f.write("},\n")
f.write("};")
f.close()