

import PIL
import torch
import torchvision.transforms as transforms
import dronet_v2_nemo_dory as nm


#Initialize model
model = nm.dronet_nemo()
#../checkpoints/pulp_dronet_v2/pulp_dronet_v2_71.pth
#load the parameters into the model
model.load_state_dict(torch.load("../checkpoints/pulp_dronet_v2/pulp_dronet_v2_71.pth"))

im = PIL.Image.open("Gate2.png")
grayim = PIL.ImageOps.grayscale(im)
grayim = grayim.resize([200,200])
transform = transforms.ToTensor()
Tensor = transform(grayim)
Tensor = Tensor.unsqueeze(0)
print(model.forward(Tensor))


