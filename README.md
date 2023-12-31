# Simply_SZ2:

## Introduction:

SZ2 is an error-bounded lossy compressor based prediction model for HPC data. This project involves only the prediction and quantization procedure of  one-dimensional floating-point data in compression to help users understand the fundamental of SZ2.



## Install:

make



## Testing Examples

Example data file can be found in the [INSTALL_DIR]/examples

./sz -i ../examples/testfloat_8_8_128.dat  -1 8192 -M REL -R 0.001 -p output



## Details：

### Prediction:

SZ2 first predicts current data according to the former data to have been traversed. The predicted value $p_i$ corresponds to original data value at position $i$. The predicters in SZ2 contain both Lorenzo and linear regression predictions, where one-dimensional Lorenzo prediction simply treats the previous data as the predicted value of the current data, i.e., $p_i=d_{i-1}$.

### Quantization:

The main function of quantizer is that turns floating-point errors between predicted values and original values into integers called quantization codes, and the calculating formula can be simplified as


$$
\acute{q_i}=\frac{d_i-p_i}{2 \times e}
$$


Furthermore, in order to convert quantization codes to unsigned integers, a more strict formula is 


$$
q_i=\frac{int(\frac{d_i-p_i}{e})+1\times sign}{2}+max\underline{~}q,\qquad {\rm where\ usually}\ max\underline{~}q=2^m-1, sign\ {\rm is\ the\ sign\ bit\ of}\ d_i-p_i
$$


The actual range of quantization code is as follows:

<img src=".\quantization.png" alt="quantizaiton" style="width: 40%; height: auto;" />



|      Notation       | Description                                                  |
| :-----------------: | ------------------------------------------------------------ |
|        $d_i$        | original data value at position $i$                          |
|        $p_i$        | predicted value of $d_i$                                     |
|   $\widehat{d_i}$   | reconstructed data value after decompression                 |
|         $e$         | specified error bound                                        |
| $max\underline{~}q$ | maximum quantization range (should be always set to an "even" number) |
|        $q_i$        | quantization index                                           |



If the $q_i$ is better than $max\underline{}q$, the original data is marked as unpredictable data. Additionally, the $q_i$ is limited to $\[1,max \underline{} q\)$ by processing, and the quantization code $q_i=0$ represents the unpredictable data. Usually, the total number of quantization codes is $max\underline{~}q$. 

The unpredictable data only remained the significant bits by analysis of binary representation.

#### Analysis of binary representation:

First, map all of the unpredictable data to a smaller range by letting all the values minus the median value of the range (denote by $med$, i.e., $med=(max(d_i)+min(d_i))/2$. Such a step is motivated by the fact that the closer the number is to zero, the less mantissa bits are required to meet the specified precision.

Second, SZ2 truncates the value by disregarding the insignificant mantissa part based on the user-required error bound. For instance, supposing the absolute error bound $e=1.22 \times 10^{-4}(i.e.,2^{-12})$, if there are both
$1.1001000_{(2)}... \times 2^6$ and $1.00001100_{(2)}... \times 2^{−10}$ numbers,  then the leading mantissa bits for the two numbers are supposed to be no less than $6−(−12)=18$ bits and $−10−(−12)=2$ bits respectively. Obviously, the latter requires much less bits to meet the required precision.

Although using $d_i-med$ for each value can obtain more accurate minimum number of required mantissa bits, the final sequence of the compressed bits for different normalized values $d_i-med$ will likely have different lengths, such that we cannot recover the normalized values based on the mixed sequence of bits. In SZ, using $radius$  computes the required mantissa bits for each value to fix the number of bits required. $(radius=(max(d_i)-min(d_i))/2 \ge d_i-med)$

Unpredictable values that are truncated certain tails, called binary processed values, will be further compressed to minimize the sizes. The main measure is to byte-match the truncated tails with the previous truncated tails, which generates three arrays representing the number of bytes that are the same as the previous binary processed value, the bytes that are different from the previous binary left over, and the remaining bits (the number of bits in the binary processed value is not always divisible by one byte or eight bits). 

### Tips

In order to ensure error consistency, the current value should be predicted by the decompressed previous value, because the $\widehat{d_i}\ne d_i$. Therefore, the $prediction—quantization$ complete procedure is  as follows:

<img src=".\procedure.png" alt="procedure" style="width: 45%; height: auto;" />

The outcome after binary representation will be further compression by squeezing the bit. Specifically, the recorder of the number of same bytes is stored in Byte format(8 bits). However, Up to 4 bytes per floating point number, this same number of bytes requires only 2 bits,  which means a byte can represent 4 numbers. Additionally, the remaining bits also not required a full byte.

## Code Output

### Predictable values

1. type: the quantization codes of every data.

### Unpredictable values:

1. exactLeadNumArray->array: the numbers of same bytes of each unpredictable value as the previous unpredictable value.
2. exactMidByteArray->array: the different bytes.
3. resiBitArray->array: the remaining bits array of each unpredictable value.
