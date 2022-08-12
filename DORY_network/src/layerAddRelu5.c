/*
 * add_layer_template.c
 * Alessio Burrello <alessio.burrello@unibo.it>
 * Thorir Mar Ingolfsson <thoriri@iis.ee.ethz.ch>
 *
 * Copyright (C) 2019-2020 University of Bologna
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
 */
// sdk                            gap_sdk
// dma_parallelization            8-cores
// optional_type                  8bit
// func_name                      layerAddRelu5
// flag_DW                        0
// optional                       add
// FLAG_BATCHNORM                 0
// has_bias                       1
// FLAG_RELU                      1
// test_location                  L3
// platform                       GAP8
// chip                           GAP8v3
// type                           char
// nof                            32
// factor                         1
// g                              1
// nif                            32
// conv_overlap1                  0
// conv_overlap2                  0
// padding_top                    0
// padding_bottom                 0
// padding_left                   0
// padding_right                  0
// stride                         1
// x_h                            25
// x_w                            25
// x_data_size_byte               8
// x_tile_size_nif                32
// x_tile_size_h                  14
// x_tile_size_w                  14
// x_tile_size_byte               6272
// x_tile_size_nif_byte           32
// x_stride_w_byte                800
// x_stride_c_byte                32
// y_h                            25
// y_w                            25
// y_data_size_byte               8
// act_dim_bit                    0
// y_tile_size_nof                32
// y_tile_size_h                  14
// y_tile_size_w                  14
// y_tile_size_byte               6272
// y_stride_w_byte                800
// y_stride_c_byte                32
// y_tile_size_nof_byte           32
// tile_dim_h                     2
// tile_dim_w                     2
// tile_dim_nof                   1
// tile_dim_nif                   1
// fs1                            1
// fs2                            1
// W_data_size_byte               0
// W_tile_size_nof                32
// b_size_byte                    32
// W_tile_size_nif                32
// W_tile_size_byte               0
// W_stride_nof_byte              0
// W_stride_hw_byte               0
// W_tile_nif_byte                0
// l2_off_bias                    0
// k_tile_size_byte               0
// lambda_tile_size_byte          0
// k_size_byte                    0
// lambda_size_byte               0
// bias_tile_size_byte            32
// l1_x_offset                    0
// l1_y_offset                    12548
// l1_x2_offset                   25096
// x_tile_size_nif_last           32
// x_tile_size_nif_byte_last      32
// x_tile_size_h_last             11
// x_tile_size_w_last             11
// y_tile_size_nof_last           32
// y_tile_size_h_last             11
// y_tile_size_w_last             11
// y_length_nof_byte_last         32


#include "layerAddRelu5.h"
#define VERBOSE_PRINT(...) printf(__VA_ARGS__)




void layerAddRelu5(
  void *args
) {
  unsigned int *real_arg = (unsigned int *) args;
  unsigned int l3_x =(unsigned int)  real_arg[0];
  unsigned int l3_y =(unsigned int)  real_arg[1];
  unsigned int l3_W =(unsigned int)  real_arg[2];
  unsigned int l2_x =(unsigned int)  real_arg[3];
  unsigned int l2_x2 =(unsigned int)  real_arg[4];
  unsigned int l2_y =(unsigned int)  real_arg[5];
  unsigned int l2_W =(unsigned int)  real_arg[6];
  unsigned int l1_buffer =(unsigned int)  real_arg[7];
  unsigned int hyperram =(unsigned int)  real_arg[8];
  unsigned int out_mult_in =(unsigned int)  real_arg[9];
  unsigned int inmul1_in = (unsigned int) real_arg[10];
  unsigned int inmul2_in = (unsigned int) real_arg[11];
  unsigned int out_shift_in = (unsigned int) real_arg[12];
  unsigned int dma_evt;

//DMA events
  unsigned int dma_read_evt_x;
  unsigned int dma_read_evt_x2;
  unsigned int dma_write_evt_y;

  int last_nof_exec;
  int last_nif_exec;
  int last_h_exec;
  int last_w_exec;

  unsigned short x_tile_size_nif;
  unsigned short  x_tile_size_h;
  unsigned short  x_tile_size_w;
  unsigned short  x_tile_size_byte;
  unsigned short  x_length_h_px;
  unsigned short  x_length_nif_byte;
  int pad_offset_h, pad_offset_w;

  char *x;
  char *x2;
  char *y;
  int x_tile_size_nif_exec;
  int x_tile_size_h_exec;
  int x_tile_size_w_exec;
  int y_tile_size_nof;
  int y_tile_size_h;
  int y_tile_size_w;
  int y_tile_size_byte;
  int y_length_h_px;
  int y_length_nof_byte;
// compute double buffering offsets and update db state
  int db_x;
  int db_y;

  int exec_db_x;
  int exec_db_W;
  dma_evt = mchan_alloc();
  // copy first tiles
  //l2_x has input activations
  dory_dma_memcpy_3d_custom(
  l2_x, // ext
  (l1_buffer + 0) + 0, // loc
  6272, // size: dimension of the buffer
  800, // stride_1: stride for the 3d copy: if we have to copy on n_features axis, this is the stride to change from first 2D space to the next ones.
  32, // stride_0: stride to be passed to 2d_copy: the dimension w of the in image
  14,// length_2: how many 2_d copies we need -> the dimension of the tile in n_features direction
  32, // length_0: legnth of the 1_d copy, the length of tile in w direction
  1, // dir
  &dma_evt // copy
  );

  dory_dma_memcpy_3d_custom(
  l2_x2, // ext
  (l1_buffer + 25096) + 0, // loc
  6272, // size: dimension of the buffer
  800, // stride_1: stride for the 3d copy: if we have to copy on n_features axis, this is the stride to change from first 2D space to the next ones.
  32, // stride_0: stride to be passed to 2d_copy: the dimension w of the in image
  14,// length_2: how many 2_d copies we need -> the dimension of the tile in n_features direction
  32, // length_0: legnth of the 1_d copy, the length of tile in w direction
  1, // dir
  &dma_evt // copy
  );
  mchan_barrier(dma_evt);
  pi_cl_team_barrier(0);
  // tile loop indeces
  int _i_nof_load=0, _i_nif_load=0, _i_h_load=0, _i_w_load=0;
  int _i_nof_exec=0, _i_nif_exec=0, _i_h_exec=0, _i_w_exec=0;

  // double buffering state
  int db_state_x=0;
  int db_state_y=1;
  int db_state_acc_out=1;
  int flag_first_ch_out;

  // last-tile flags
  int last_nof_load = (1 == 1) ? 1 : 0;
  int last_nif_load = (1 == 1) ? 1 : 0;
  int last_h_load = (2 == 1) ? 1 : 0;
  int last_w_load = (2 == 1) ? 1 : 0;
  int iter;
  uint16_t out_mult1 = inmul2_in;
  uint16_t out_mult2 = inmul1_in;
  uint16_t out_shift = out_shift_in;
  // tile loop nest
  for(iter=0; iter<1*2*2; iter++) {
    // loop nest is nof,h,w,(nif=0)
    _i_w_load += 1;
    if(_i_w_load==2) 
    {
      _i_w_load = 0;
      _i_h_load += 1;
      if(_i_h_load==2) 
      {
        _i_h_load = 0;
        _i_nif_load += 1;
        _i_nof_load += 1;
      }
    }
    // check if last in any dimension
    last_nof_exec = last_nof_load;
    last_nif_exec = last_nif_load;
    last_h_exec = last_h_load;
    last_w_exec = last_w_load;
    last_nof_load = (_i_nof_load+1 == 1) ? 1 : 0;
    last_nif_load = (_i_nof_load+1 == 1) ? 1 : 0;
    last_h_load = (_i_h_load+1 == 2) ? 1 : 0;
    last_w_load = (_i_w_load+1 == 2) ? 1 : 0;

    // compute double buffering offsets and update db state
    db_x = !db_state_x ? 6272 : 0;
    db_y = !db_state_y ? 6272 : 0;
    exec_db_x = db_state_x ? 6272 : 0;
    db_state_x = ! db_state_x;

    //switch all double buffering offset and y only after that all n_input_features have been analyzed: we need to pass all n_in to produce a single filter_out
    db_state_y = ! db_state_y;

    if(iter<1*2*2-1) 
    {
      x_tile_size_nif = (last_nif_load) ? 32 : 32;
      x_tile_size_h   = (last_h_load)   ? 11 : 14;
      x_tile_size_w   = (last_w_load)   ? 11 : 14;
      x_tile_size_byte = x_tile_size_nif*x_tile_size_h*x_tile_size_w*8/8;
      x_length_nif_byte = (last_nif_load)   ? 32 : 32;
      // additionally overlap by padding for the first tile after a border one
      //this because in the first tile we use less pixels from x_buffer, since we have the ones of padding
      dory_dma_memcpy_3d_custom(
        dory_get_tile_3d(l2_x, _i_h_load, _i_w_load, _i_nif_load, 14, 14, 32, 25, 32,  0, 0,0, 0, 0, 0, 8), // extern
        (l1_buffer + 0) + db_x, // loc
        x_tile_size_byte, // size: dimension of the buffer
        800, // stride_1: stride for the 3d copy: if we have to copy on n_features axis, this is the stride to change from first 2D space to the next ones.
        32, // stride_0: stride to be passed to 2d_copy: the dimension w of the in image
        x_tile_size_h,// length_2: how many 2_d copies we need -> the dimension of the tile in n_features direction
        x_length_nif_byte, // length_0: legnth of the 1_d copy, the length of tile in w direction
        1, // dir
        &dma_evt // copy
        );
      dory_dma_memcpy_3d_custom(
        dory_get_tile_3d(l2_x2, _i_h_load, _i_w_load, _i_nif_load, 14, 14, 32, 25, 32,  0, 0,0, 0, 0, 0, 8), // extern
        (l1_buffer + 25096) + db_x, // loc
        x_tile_size_byte, // size: dimension of the buffer
        800, // stride_1: stride for the 3d copy: if we have to copy on n_features axis, this is the stride to change from first 2D space to the next ones.
        32, // stride_0: stride to be passed to 2d_copy: the dimension w of the in image
        x_tile_size_h,// length_2: how many 2_d copies we need -> the dimension of the tile in n_features direction
        x_length_nif_byte, // length_0: legnth of the 1_d copy, the length of tile in w direction
        1, // dir
        &dma_evt // copy
        );
      y_tile_size_h   = (last_h_load)   ? 11 : 14;
      y_tile_size_w   = (last_w_load)   ? 11 : 14;
    }
    x = (char *) (l1_buffer + 0 + exec_db_x);
    x2 = (char *) (l1_buffer + 25096 + exec_db_x);
    y = (char *) (l1_buffer + 12548 + db_y);
    x_tile_size_nif_exec = (last_nif_exec) ? 32 : 32;
    x_tile_size_h_exec   = (last_h_exec)   ? 11 : 14;
    x_tile_size_w_exec   = (last_w_exec)   ? 11 : 14;

    y_tile_size_nof = (last_nof_exec) ? 32 : 32;
    y_tile_size_h   = (last_h_exec)   ? 11 : 14;
    y_tile_size_w   = (last_w_exec)   ? 11 : 14;
    y_tile_size_byte = y_tile_size_nof*y_tile_size_h*y_tile_size_w*8/8;
    y_length_nof_byte = (last_nof_exec)   ? 32 : 32;
    asm volatile("": : :"memory");
    pi_cl_team_barrier(0);
    pulp_nn_add(
      x,
      x2,
      x_tile_size_nif_exec,
      x_tile_size_h_exec,
      x_tile_size_w_exec,
      y,
      out_mult1,
      out_mult2,
      out_shift
      );
    pi_cl_team_barrier(0);
    // wait for DMA write
    mchan_barrier(dma_evt);
    // copying output back to L2
    dory_dma_memcpy_3d_custom_out(
      dory_get_tile_3d(l2_y, _i_h_exec, _i_w_exec, _i_nof_exec, 14, 14, 32, 25, 32, 0, 0, 0, 0, 0, 0, 8), // ext
      (l1_buffer + 12548) + db_y, // loc
      y_tile_size_byte, // size
      800, // stride_1
      32, // stride_0
      y_tile_size_h, // length_2
      y_length_nof_byte, // length_0
      0, // dir
      &dma_evt // copy
    );
    // update prev iterators
    _i_nof_exec = _i_nof_load;
    _i_nif_exec = _i_nif_load;
    _i_h_exec = _i_h_load;
    _i_w_exec = _i_w_load;
  }
  // wait for final write
  mchan_barrier(dma_evt);
  mchan_free(dma_evt);
}