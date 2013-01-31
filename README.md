Advanced-Graphics
=================

Coursework

## Requirement

- gcc-4.6 or higher (c++11 support) or icpc / icc

- openmp

## Installation

$ ./install.sh -cmifp --compiler-cxx=g++ --compiler-c=gcc

- c : configure

- m : make

- i : install

- f : force (force to remove build folder and reconfigure)

- p : proper (remove build folder at the end)

# Current Features

- class image (~70%)

- loadPNM (4/6 implemented)

- savePNM (2/6 implemented)

- loadPFM

- savePFM

- create HDR

- normalise : change each pixel value from [0 ; m_max_pixel_chanel] to [ 0 ; n ]

- troncate

- gamma

# Use example

- cd /bin/usr/local/bin

- ./HDRimage memorial1.pfm memorial2.pfm memorial3.pfm memorial4.pfm memorial5.pfm memorial6.pfm memorial7.pfm
