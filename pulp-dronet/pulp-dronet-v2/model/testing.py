

import PIL
import torch
import torchvision.transforms as transforms
import dronet_v2_nemo_dory as nm
import os
dirPath = os.path.dirname(os.path.abspath(__file__))
#Initialize model
model = nm.dronet_nemo()
#../checkpoints/pulp_dronet_v2/pulp_dronet_v2_71.pth
#load the parameters into the model
model.load_state_dict(torch.load("dronet_v2_nemo_dory_original_himax.pth"))
files = os.listdir("dataset_test")
results = open("results.txt", "w")


for i in files:
    im = PIL.Image.open(dirPath + "/dataset_test/" + i)
    #grayim = PIL.ImageOps.grayscale(im)
    #grayim = grayim.resize([200,200])
    width = im.width
    height = im.height
    left = width//2 - 100
    top = height - 200
    right = width//2 + 100
    bottom = height
    cutIm = im.crop((left, top, right, bottom))
    transform = transforms.ToTensor()
    Tensor = transform(cutIm)
    Tensor = Tensor.unsqueeze(0)
    results.write(i + " = " + str(model.forward(Tensor)) + "\n")
    #print(model.forward(Tensor))
    im.close()
results.close()
"""
im = PIL.Image.open(dirPath + "/dataset_test/img_000024.png")
width = im.width
height = im.height
left = width//2 - 100
top = height - 200
right = width//2 + 100
bottom = height
cutIm = im.crop((left, top, right, bottom))
transform = transforms.ToTensor()
Tensor = transform(cutIm)
Tensor = Tensor.unsqueeze(0)
print(model.forward(Tensor))
im.close()
"""