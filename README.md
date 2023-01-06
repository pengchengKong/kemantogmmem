# KemansToGmm

Firstly, the Kmeans algorithm is iterated to calculate the center of gravity of the class, and then the Gaussian mixture model is iterated to classify the samples

## Installation

You can install the development version of KemansToGmm like so:

``` r
# FILL THIS IN! HOW CAN PEOPLE INSTALL YOUR DEV PACKAGE?
```

## Example

This is a basic example which shows you how to solve a common problem:

``` r
library(KemansToGmm)
library(ggplot2)
```

What is special about using `README.Rmd` instead of just `README.md`?
You can include R chunks like so:

``` r
samp = matrix(c(8.6338,8.8988,3.604,14.1556,19.2995,18.2959,31.7925,32.8074,32.0885,148.6553,141.5995,147.4205,7.0472,1.1754,7.5018,12.3305,17.0575,17.2132,33.5934,37.5266,36.9793,153.7012,151.2712,152.8625), nrow = 12, ncol = 2)

> samp
          [,1]     [,2]
 [1,]   8.6338   7.0472
 [2,]   8.8988   1.1754
 [3,]   3.6040   7.5018
 [4,]  14.1556  12.3305
 [5,]  19.2995  17.0575
 [6,]  18.2959  17.2132
 [7,]  31.7925  33.5934
 [8,]  32.8074  37.5266
 [9,]  32.0885  36.9793
[10,] 148.6553 153.7012
[11,] 141.5995 151.2712
[12,] 147.4205 152.8625

> kmtogmm(samp,4)
$classprob
          [,1]
[1,] 0.2489685
[2,] 0.2510315
[3,] 0.2500000
[4,] 0.2500000

$mean
         [,1]     [,2]     [,3]     [,4]
[1,] 7.038953 17.21493 32.22947 145.8918
[2,] 5.233986 15.49886 36.03310 152.6116

$sigma
$sigma[[1]]
          [,1]      [,2]
[1,]  5.947869 -4.177963
[2,] -4.177963  8.321718

$sigma[[2]]
         [,1]     [,2]
[1,] 5.239998 5.209499
[2,] 5.209499 5.408006

$sigma[[3]]
          [,1]      [,2]
[1,] 0.1816061 0.5986095
[2,] 0.5986095 3.0259909

$sigma[[4]]
         [,1]     [,2]
[1,] 9.465898 3.049353
[2,] 3.049353 1.015617


$loglikelihood
[1] -51.91775

$BIC
[1] 151.0487

$sampleprob
               [,1]         [,2]          [,3] [,4]
 [1,]  9.876225e-01 1.237748e-02  0.000000e+00    0
 [2,]  1.000000e+00 3.041187e-37  0.000000e+00    0
 [3,]  1.000000e+00 2.514240e-36  0.000000e+00    0
 [4,]  8.714458e-09 1.000000e+00  0.000000e+00    0
 [5,]  2.468128e-23 1.000000e+00 4.210657e-315    0
 [6,]  1.421700e-21 1.000000e+00  0.000000e+00    0
 [7,] 3.926886e-108 8.284102e-22  1.000000e+00    0
 [8,] 5.215811e-128 3.515718e-51  1.000000e+00    0
 [9,] 1.911279e-122 2.258519e-52  1.000000e+00    0
[10,]  0.000000e+00 0.000000e+00  0.000000e+00    1
[11,]  0.000000e+00 0.000000e+00  0.000000e+00    1
[12,]  0.000000e+00 0.000000e+00  0.000000e+00    1

$Classify
      [,1]
 [1,]    0
 [2,]    0
 [3,]    0
 [4,]    1
 [5,]    1
 [6,]    1
 [7,]    2
 [8,]    2
 [9,]    2
[10,]    3
[11,]    3
[12,]    3

$sampleplot
```

You’ll still need to render `README.Rmd` regularly, to keep `README.md`
up-to-date. `devtools::build_readme()` is handy for this. You could also
use GitHub Actions to re-render `README.Rmd` every time you push. An
example workflow can be found here:
<https://github.com/r-lib/actions/tree/v1/examples>.

You can also embed plots, for example:

![image-20230106162921015](C:\Users\16598\AppData\Roaming\Typora\typora-user-images\image-20230106162921015.png)

In that case, don’t forget to commit and push the resulting figure
files, so they display on GitHub and CRAN.
