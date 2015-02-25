#include <itpp/itcomm.h>
#include <iostream>
#include <bitset>
using namespace itpp;
using namespace std;
//These lines are needed for use of cout and endl
using std::cout;
using std::endl;
int bvec2int(bvec &bv, int pos, bool is_smaller);
void write_file(bvec &bv, string s);
int read_file(bvec &bv, string s);
void write_file(bvec &bv, string s)
{
	ofstream f1;
	f1.open(s.c_str(), ios::binary);
	
	int vector_length = bv.length();
	int int_length = vector_length / 32;
	//cout << bv << endl;
	//cout << int_length << endl;
	for(int j = 0; j < int_length; j++)
	{
		int t = bvec2int(bv, j * 32, true);
		//cout << "1. " << t << endl;
		f1.write((char *)&t, sizeof(int));
	}
	
	if(vector_length % 32 > 0)
	{
		//cout << "vector_length. " << vector_length << endl;
		int t = bvec2int(bv, int_length * 32, false);
		//cout << "2. " << t << endl;
		f1.write((char *)&t, (vector_length % 32) / 8);
	}
	
	f1.close();
	return;
}

/*int read_file(bvec &bv, string s)
{
	FILE * pFile;
	pFile = fopen (s.c_str(), "rb");
	
	ifstream f(s.c_str(), ios::binary);
	bvec a;
	int i, i_read;
	i_read = f.read((char*) &i, sizeof(int));
	while(i_read)
	{
		cout << i << endl;
		a = itpp::dec2bin(i_read, i);
		bv.ins(bv.size(), a);
		if(i_read < 32)
		{
			f.close();
			return i_read;
		}
		i_read = f.read((char*) &i, sizeof(int));
	}
	f.close();
	return 0;
}*/


int read_file(bvec &bv, string s)
{
	FILE* pFile;
	pFile = fopen (s.c_str(), "rb");
	
	bvec a;
	int i, i_read;
	i_read = fread((char*) &i, 1, sizeof(int), pFile);
	//cout << "i: " << i << "        i_read: " << i_read << endl;
	while(i_read)
	{
		a = itpp::dec2bin(i_read * 8, i);
		bv.ins(bv.size(), a);
		if(i_read < 4)
		{
			fclose(pFile);
			return i_read;
		}
		i_read = fread((char*) &i, 1, sizeof(int), pFile);
		//cout << "i: " << i << "        i_read: " << i_read << endl;
	}
	fclose(pFile);
	return 0;
}

void init()
{

}

int bvec2int(bvec &bv, int pos, bool is_smaller)
{
	int ret = 0;
	int a[] = {0x00000001, 0x00000002, 0x00000004, 0x00000008, 0x00000010, 0x00000020, 0x00000040, 0x00000080, 0x00000100, 	0x00000200, 0x00000400, 0x00000800, 0x00001000, 0x00002000, 0x00004000, 0x00008000, 0x00010000, 0x00020000, 0x00040000, 	0x00080000, 0x00100000, 0x00200000, 0x00400000, 0x00800000, 0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 	0x20000000, 0x40000000, 0x80000000};
	if(is_smaller)
	{
		for(int i = pos; i < pos + 32; i++)
		{
			//cout << bv[i] << " ";
			if(bv[i] == 1)
				ret = ret | a[31 - i + pos];
		}
	}
	else
	{
		for(int i = pos; i < bv.length(); i++)
		{
			//cout << bv[i] << " ";
			if(bv[i] == 1)
			{
				ret = ret | a[31 - i + pos];
				
			}
		}
		ret = ret >> (32 - bv.length() + pos);
		//cout << "ret 1: " << ret << endl; 
	}
	//cout << endl;
	//cout << "c = " << (bitset<32>) ret << endl; 
	return ret;
}

/*BPSK bpsk;
Punctured_Convolutional_Code code;
ivec generator(3);
generator(0)=0133;
generator(1)=0165;
generator(2)=0171;
code.set_generator_polynomials(generator, 7);
bmat puncture_matrix = "1 1;0 1";
code.set_puncture_matrix(puncture_matrix);
code.set_truncation_length(30);
bvec bits=randb(100), encoded_bits, decoded_bits;

vec tx_signal, rx_signal;
code.encode(bits, encoded_bits);
tx_signal = bpsk.modulate_bits(encoded_bits);
rx_signal = tx_signal + sqrt(0.5)*randn(tx_signal.size());
code.decode(rx_signal, decoded_bits);*/

 
int main()
{
	bvec bv;
	read_file(bv, "Data/a1.amr");

	
	//Scalars
	int constraint_length, MaxNrofErrors, Nobits, MaxIterations, p, i;
	double Ec, Eb;
	//Vectors
	ivec generators;
	vec EbN0dB, EbN0, N0, ber, trans_symbols, rec_symbols;
	bvec uncoded_bits, coded_bits, decoded_bits;
	//Classes
	BPSK bpsk;
	BERC berc;
	AWGN_Channel channel;
	/*
	Set up the convolutional encoder/decoder class:
	The generators are given in octal form by adding a zero in front of the numbers.
	In this example we will simulate a rate 1/3 code that is listed in J. G. Proakis,
	"Digital communications". The encoder has constraint length 7.
	*/
	generators.set_size(3, false);
	generators(0) = 0133;
	generators(1) = 0145;
	generators(2) = 0175;
	constraint_length = 7;
	
	Punctured_Convolutional_Code code;
	//Convolutional_Code code;
	code.set_truncation_length(30);
	code.set_generator_polynomials(generators, constraint_length);
	bmat puncture_matrix = "1 1; 0 1; 1 1";
	code.set_puncture_matrix(puncture_matrix);
	
	/*Turbo_Codec turbo;
	ivec gen(2);
	gen(0) = 013; gen(1) = 015;
	int constraint_length = 4;
	ivec interleaver_sequence = wcdma_turbo_interleaver_sequence(320);
	turbo.set_parameters(gen, gen, constraint_length, interleaver_sequence);*/

	
	//Init: Calculate some simulation specific parameters:
	Ec = 1.0;
	EbN0dB = linspace(-3, 1, 1);
	EbN0 = inv_dB(EbN0dB);
	Eb = Ec / code.get_rate();
	N0 = Eb * pow(EbN0, -1);
	MaxNrofErrors = 100;
	Nobits = 32;
	MaxIterations = 1;
	ber.set_size(EbN0dB.length(), false);
	ber.clear();
	//Randomize the random number generators.
	RNG_randomize();
	for (p = 0; p < EbN0dB.length(); p++) 
	{
		cout << "Now simulating point " << p + 1 << " out of " << EbN0dB.length() << endl;
		berc.clear(); //Clear the bit error rate counter.
		channel.set_noise(N0(p) / 2.0); //Set the noise value of the AWGN channel.
	
		for (i = 0; i < MaxIterations; i++) 
		{
			coded_bits = code.encode(bv); //The convolutional encoder function.
			//turbo(bv, coded_bits)


			bpsk.modulate_bits(coded_bits, trans_symbols); //The BPSK modulator.
			rec_symbols = channel(trans_symbols); //The AWGN channel.
			
			
			decoded_bits = code.decode(rec_symbols); //The Viterbi decoder function.
			
			cout << "The result is: " << endl;
			//cout << decoded_bits << endl;
			//cout << endl;
			berc.count(uncoded_bits, decoded_bits); //Count the errors.
			ber(p) = berc.get_errorrate();
			//Break the simulation on this point if sufficient number of bit errors were observed:
			if (berc.get_errors() > MaxNrofErrors) 
			{
				cout << "Breaking on point " << p + 1 << " with " << berc.get_errors() << " errors." << endl;
				break;
			}
		}
	}
	//Print the results:
	cout << "BER = " << ber << endl;
	cout << "EbN0dB = " << EbN0dB << endl;
	write_file(decoded_bits, "_Data/_a1.amr");
	return 0;
}

int main__cpp()
{
	BPSK bpsk;
	Punctured_Convolutional_Code code;
	ivec generator(3);
	generator(0)=0133;
	generator(1)=0165;
	generator(2)=0171;
	code.set_generator_polynomials(generator, 7);
	bmat puncture_matrix = "1 1; 0 1; 1 1";//"1 0 1; 1 1 1";
	cout << puncture_matrix.rows() << endl;
	cout << puncture_matrix.cols() << endl;
	code.set_puncture_matrix(puncture_matrix);
	//code.set_truncation_length(30);
	bvec bits=randb(100), encoded_bits, decoded_bits;

	cout << bits << endl;

	vec tx_signal, rx_signal;
	code.encode(bits, encoded_bits);
	tx_signal = bpsk.modulate_bits(encoded_bits);
	rx_signal = tx_signal + sqrt(0.5)*randn(tx_signal.size());
	code.decode(rx_signal, decoded_bits);
	
	cout << decoded_bits << endl;
	return 0;
}
