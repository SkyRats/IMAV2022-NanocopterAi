from PIL import Image
from PIL import ImageFilter
import glob
import sys
import os

#input_folder = input("Qual o path absoluto da pasta de entrada?")
#output_folder = input("Qual o path absoluto da pasta de saída?")

#input_files = glob.glob(input_folder + "/*.png")

#for file in input_files:

input_image = Image.open(sys.argv[1])

width, height = input_image.size[0], input_image.size[1]

crop_area = (width//2 - 100, height - 200,  width//2 + 100, height)

output_image = input_image.crop(crop_area)

output_image = output_image.filter(ImageFilter.GaussianBlur(2))

output_image = output_image.convert("L")

output_filename = sys.argv[1][0:-3] + "jpeg"
output_image.save(output_filename, "JPEG")

