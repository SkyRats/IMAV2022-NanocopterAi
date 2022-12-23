
import sys
sys.path.append("../../controllers/")

import PIL
import cv2 as cv
import torch
import torchvision.transforms as transforms
import dronet_v2_nemo_dory as nm


#Initialize model
model = nm.dronet_nemo()

#load the parameters into the model
model.load_state_dict(torch.load("../../controllers/Pulp-Dronet/dronet_v2_nemo_dory_original.pth"), "CUDA")

im = PIL.Image.open("Gate2.png")
grayim = PIL.ImageOps.grayscale(im)
width, height = grayim.size
left = width//2 - 100
top = height - 200
right = width//2 + 100
bottom = height


image= cv.imread("Gate2.png")
image_gray = cv.cvtColor(image, cv.COLOR_BGR2GRAY)
image = image_gray[left:right, top:bottom]

image = PIL.Image.fromarray(image)



#grayim = grayim.crop((left, top, right, bottom))


transform = transforms.ToTensor()
Tensor1 = transform(image)




Tensor1 = Tensor1.unsqueeze(0)
Result = model.forward(Tensor1)
print(Result[1][0].item())


