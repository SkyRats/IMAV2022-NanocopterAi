import PIL
import torch
import torchvision.transforms as transforms
import sys
sys.path.append('../../../controllers/')
import dronet_v2_nemo_dory as nm
import numpy as np
import cv2 as cv

def obsDetector(rawImage):
    #Initialize model
    model = nm.dronet_nemo()
    #load the parameters into the model
    model.load_state_dict(torch.load("../../../controllers/Pulp-Dronet/dronet_v2_nemo_dory_original_himax.pth"), "CUDA")
    im = np.frombuffer(rawImage, np.uint8).reshape((200, 200, 4))
    im = cv.cvtColor(im, cv.COLOR_BGR2GRAY)
    imP = PIL.Image.fromarray(im) #Transforms image into Pillow image
    imP = imP.filter(PIL.ImageFilter.GaussianBlur(2))
    imP = imP.convert("L")
    transform = transforms.ToTensor()
    Tensor = transform(imP) #Transforms image into Tensor
    Tensor = Tensor.unsqueeze(0)
    Result = model.forward(Tensor)
    
    return [Result[1][0].item(), Result[0][0].item()]
