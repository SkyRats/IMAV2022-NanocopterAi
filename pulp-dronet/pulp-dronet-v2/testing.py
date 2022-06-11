#-------------------------------------------------------------------------------#
# Copyright (C) 2020-2021 ETH Zurich, Switzerland, University of Bologna, Italy.#
# All rights reserved.                                                          #
#                                                                               #
# Licensed under the Apache License, Version 2.0 (the "License");               #
# you may not use this file except in compliance with the License.              #
# See LICENSE.apache.md in the top directory for details.                       #
# You may obtain a copy of the License at                                       #
#                                                                               #
#   http://www.apache.org/licenses/LICENSE-2.0                                  #
#                                                                               #
# Unless required by applicable law or agreed to in writing, software           #
# distributed under the License is distributed on an "AS IS" BASIS,             #
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.      #
# See the License for the specific language governing permissions and           #
# limitations under the License.                                                #
#                                                                               #
# File:   testing.py                                                            #
# Author: Daniele Palossi  <dpalossi@iis.ee.ethz.ch> <daniele.palossi@idsia.ch> #
#         Lorenzo Lamberti <lorenzo.lamberti@unibo.it>                          #
#         Vlad Niculescu   <vladn@iis.ee.ethz.ch>                               #
# Date:   18.02.2021                                                            #
#-------------------------------------------------------------------------------#

# Description:
# This script is used to check the PULP-DroNet CNN performance (Accuracy and RMSE) 
# with a set of pre-trained weights. 
# You must specify the CNN architecture (nemo_dory or gapflow) and 
# the path to the pre-trained weights that you want to load ('--model_weights').
# The output will be the testing MSE and Accuracy of such network.

# essentials
import os
import argparse
import numpy as np
from os.path import join
from tqdm import tqdm
# torch
import torch
import torch.nn as nn
from torch.utils.data import DataLoader
from torchvision import transforms
# PULP-dronet
from utility import DronetDataset
from utility import custom_mse, custom_accuracy

# change working directory to the folder of the project
working_dir = os.path.dirname(os.path.realpath(__file__))
os.chdir(working_dir)
print('\nworking directory:', working_dir, "\n")

def create_parser(cfg):
    parser = argparse.ArgumentParser(description='PyTorch PULP-DroNet Testing')
    parser.add_argument('-d', '--data_path', help='path to dataset',
                        default=cfg.data_path)
    parser.add_argument('-s', '--dataset', default=cfg.testing_dataset,
                        choices=['original', 'himax'], 
                        help='train on original or original+himax dataset')
    parser.add_argument('-m', '--model_weights', default=cfg.model_weights, 
                        help='path to the weights of the testing network (.pth file)')
    parser.add_argument('-a', '--flow',metavar='DEPLOYMENT_FLOW',default=cfg.flow,
                        choices=['nemo_dory', 'gapflow'],
                        help='select the NN architecture backbone:')
    parser.add_argument('--gpu', help='which gpu to use. Just one at'
                        'the time is supported', default=cfg.gpu)
    parser.add_argument('-j', '--workers', default=cfg.workers, type=int, metavar='N',
                        help='number of data loading workers (default: 4)')
    parser.add_argument('-b', '--batch_size', default=cfg.testing_batch_size, type=int,
                        metavar='N',
                        help='mini-batch size (default: 32), this is the total '
                            'batch size of all GPUs')
    return parser

def testing(model, testing_loader, device):
    model.eval()
    loss_mse, loss_acc = [], []
    test_mse, test_acc = 0.0, 0.0

    with tqdm(total=len(testing_loader), desc='Test', disable=not True) as t:

        with torch.no_grad():
            for batch_idx, data in enumerate(testing_loader):

                inputs, labels, types, filenames = data[0].to(device), data[1].to(device), data[2].to(device), data[3]

                outputs = model(inputs)
                # we might have batches without steering or collision samples
                loss_mse_value, loss_mse_valid = custom_mse(labels, outputs, types, device)
                loss_acc_value, loss_acc_valid = custom_accuracy(labels, outputs, types, device)
                
                if loss_mse_valid == 1:
                    loss_mse.append(loss_mse_value.item())
                    test_mse = sum(loss_mse)/len(loss_mse)
                if loss_acc_valid == 1:
                    loss_acc.append(loss_acc_value.item())
                    test_acc = sum(loss_acc)/len(loss_acc)

                t.set_postfix({'mse' : test_mse, 'acc' : test_acc})
                t.update(1)

    print("Testing MSE: %.4f" % test_mse, "Acc: %.4f" % test_acc)
    return test_mse, test_acc


def main():
    # parse arguments
    global args
    from config import cfg # load configuration with all default values 
    parser = create_parser(cfg)
    args = parser.parse_args()
    model_weights_path=args.model_weights
    print("Model name:", model_weights_path)
    
    # select device
    os.environ['CUDA_VISIBLE_DEVICES'] = args.gpu
    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
    print("CUDA/CPU device:", device)
    print("pyTorch version:", torch.__version__)

    # import PULP-DroNet CNN architecture
    if args.flow == 'nemo_dory':
        from model.dronet_v2_nemo_dory import dronet
    elif args.flow == 'gapflow':
        from model.dronet_v2_gapflow import dronet
    else: 
        raise ValueError('Doublecheck the deployment flow that you are trying to use.\
                            Select one between nemo_dory and gapflow')

    # define testing dataset path
    if args.dataset == 'original':
        testing_data_path   = join(args.data_path, "testing/")
    elif args.dataset == 'himax':
        testing_data_path   = join(args.data_path, "himax/jpg/testing/")
    else:
        raise ValueError('they only choices for dataset are: original or himax')
    print('You are testing on the', args.dataset ,'dataset\n')

    # load testing set
    test_dataset = DronetDataset(
        root=testing_data_path,
        transform=transforms.ToTensor())
    test_loader = torch.utils.data.DataLoader(
        test_dataset, 
        batch_size=args.batch_size, 
        shuffle=False, 
        num_workers=args.workers)

    # load the CNN model
    net = dronet()
    net.to(device)

    #load weights into the network
    if os.path.isfile(model_weights_path):
        if torch.cuda.is_available():
            checkpoint = torch.load(model_weights_path, map_location=device)
            print('loaded checkpoint on cuda')
        else:
            checkpoint = torch.load(model_weights_path, map_location='cpu')
            print('CUDA not available: loaded checkpoint on cpu')
        if 'state_dict' in checkpoint:
            checkpoint = checkpoint['state_dict']
        else:
            print('Failed to find the [''state_dict''] inside the checkpoint. I will try to open it anyways.')
        net.load_state_dict(checkpoint)
    else: 
        raise RuntimeError('Failed to open checkpoint. provide a checkpoint.pth.tar file')

    #### TESTING ####
    print('model tested:', model_weights_path, 'evaluated on the:', args.dataset, 'dataset')
    testing(net, test_loader, device)


if __name__ == '__main__':
    main()
