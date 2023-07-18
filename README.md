# Simply_SZ:

## Introduction:

SZ is a error-bounded lossy compressor based prediction model for HPC data. This project contains only the one-dimensional floating-point data compression procedure of SZ2 to help users understand the basic principles of SZ2.



## Install:



## Fundamental：

The SZ2 first predict current data according to the former data to have been traversed.



| Notation      | Description                                  |
| ------------- | -------------------------------------------- |
| $d_i$         | original data value at position $i$          |
| $p_i$         | predicted value of $d_i$                     |
| $\widehat{d}$ | reconstructed data value after decompression |
| $e$           | specified error bound                        |
| $max\_q$      | maximum quantization range                   |
| $q$           | quantization index                           |





