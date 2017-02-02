# Z80 module assembler

[![Build Status](https://travis-ci.org/z88dk/z80asm.svg?branch=master)](https://travis-ci.org/z88dk/z80asm)

z80asm is part of the [z88dk](http://www.z88dk.org/) project and is used as the back-end of the z88dk C compilers. It has a long history of development by different people and in different platforms, resulting in a complex code base and causing changes to be difficult to implement. 

Two examples were the implementation of sections and link-time resolved expressions that were difficult to get right.

This repository holds the next generation z80asm under development.

## Copyright

The original z80asm module assembler was written by Gunther Strube. 
It was converted from QL SuperBASIC version 0.956, initially ported to Lattice C,
and then to C68 on QDOS.

It has been maintained since 2011 by Paulo Custodio.

Copyright (C) Gunther Strube, InterLogic 1993-99  
Copyright (C) Paulo Custodio, 2011-2017

## Repository

The original C source code is in the [z88dk GitHub](https://github.com/z88dk/z88dk/tree/master/src/z80asm).
This C++ port is in the [z80asm GitHub](https://github.com/z88dk/z80asm).

## License

Artistic License 2.0 <http://www.perlfoundation.org/artistic_license_2_0>

## External contributions

This package uses Open Source Software developed and distributed by others.  
These external packages keep their own copyright notices and licences.

## Project Objectives

The current project has the following objectives:

1. Be as much as reasonable compatible with the previous version;
1. Replace the previous version as C-compiler back-end;
1. Developed in C++ to simplify coding;
1. Portable to at least Windows, Linux and OS-X;

# Wiki for contributions

See https://github.com/z88dk/z80asm/wiki
