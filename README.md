# mobile-network-simulation
Simulation for mobile network with: (a) Channel coding - Punctured Convolution code (b) Transfer over AWGN channel (c) Decode by Viterbi algorithm (d) Quality estimation
You need itpp to compile it.
I recommend you run it in Linux environment.

Command to build: 
g++ `pkg-config --cflags itpp` -o your_file_name file_name.cpp `pkg-config --libs itpp`

Copyleft: MinhQuang NGUYEN, Telecom SudParis
