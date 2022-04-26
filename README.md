# Matroid Intersection

**Version 1.0 - 23/04/2022**

## Information about this project

This repository contains implementations of some algorithms to solve the matroid intersection problem based on finding augmenting paths on the exchange graph.
Currently, you can find implementations of:

* Lawler, Eugene L. "Matroid intersection algorithms." Mathematical programming 9.1 (1975): 31-56.
* Aigner, Martin, and Thomas A. Dowling. "Matching theory for combinatorial geometries." Transactions of the American Mathematical Society 158.1 (1971): 231-245.
* Cunningham, William H. "Improved bounds for matroid partition and intersection algorithms." SIAM Journal on Computing 15.4 (1986): 948-957.	
* Chakrabarty, Deeparnab, et al. "Faster matroid intersection." 2019 IEEE 60th Annual Symposium on Foundations of Computer Science (FOCS). IEEE, 2019.

**Remark:** The first and the second papers essentially give the same algorithm.

## How to use

The repository contains a Makefile. To compile use the `make` command. To run use the `make run` command or type `./exec` in the command line.
The `main.cpp` file currently has an example on how to make use of some parts of the code.

To use this code for your own projects, you should go to the `oracles` directory and follow the example of the oracle implementation for the uniform matroid.


## Authors
This project was developed by Francisco Sena from Instituto Superior Técnico - University of Lisbon, Portugal.
For *any* questions: f.miguel.sena@tecnico.ulisboa.pt