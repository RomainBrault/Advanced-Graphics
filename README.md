Advanced-Graphics
=================

Coursework

## Requirement

gcc-4.6 or higher (c++11 support) or icpc / icc
openmp

## Installation

$ ./install.sh -cmifp --compiler-cxx=g++ --compiler-c=gcc

c : configure
m : make
i : install
f : force (force to remove build folder and reconfigure)
p : proper (remove build folder at the end)

# Current Features

class image (~50%)
loadPNM (4/6 implemented)
savePNM (2/6 implementer)
normalise : change each pixel value from [0 ; m_max_pixel_chanel] to [ 0 ; n ]

# TODO

loadLFM
saveLFM

rest of the coursework...