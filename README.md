# mobile-network-simulation
------ Cach bien dich -------
Simulation for mobile network with: (a) Channel coding - Punctured Convolution code (b) Transfer over AWGN channel (c) Decode by Viterbi algorithm (d) Quality estimation
You need itpp to compile it.
I recommend you run it in Linux environment.

Command to build: 
    g++ `pkg-config --cflags itpp` -o your_file_name file_name.cpp `pkg-config --libs itpp`

About raw file, usually used as input of source coding:
Play raw file:
    aplay -t raw -c 1 -r 8000 -f S16_LE ./_data/wtest0.raw

Convert raw to mp3:
    sox -r 8000 -b 16 -c 1 -e signed-integer ./test.raw ./test.wav
    
Cut a file
    sox test.wav 60secs.wav --show-progress trim 0 01:00

Copyleft: MinhQuang NGUYEN, Telecom SudParis

----- Cach su dung -----
# Channel_source_coding

quality_test [-f/-d] x y z

-f
	compare 2 files: x and y, the result is printed to file z
-d
	compare 2 folders: x and y, the result is printed to file z

default
	compare file x with all files in folder y, the result is printed to file z
	

	
codeX_Y x y
	transfer file x over AWGN channel with convolutional code, (code rate: X/Y). Receive file: y


