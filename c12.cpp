#include <itpp/itcomm.h>
#include <iostream>
#include <bitset>
using namespace itpp;
using namespace std;
#define BS 1024

using std::cout;
using std::endl;
int bvec2int(bvec &bv, int pos, bool is_smaller);
void write_file(bvec &bv, string s);
int read_file(bvec &bv, string s);
int read_file(bvec &bv, FILE* f, int i);
void write_file(bvec &bv, FILE* f, int i);

char bvec2char(bvec &bv, int pos)
{
	char ret = 0;
	int a[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
	if(pos > bv.length() - 8)
	{
		cout << "Can sua loi nay";
		return 0;
	}
	for(int i = pos; i < pos + 8; i++)
	{
		if(bv[i] == 1)
			ret = ret | a[7 - i + pos];
	}
	return ret;
}


void char2vec(char t, bvec &v)
{
	//can xem lai
	int i = (int) t;
	v = itpp::dec2bin(8, i);
	return;
}

// doc tu byte thu i, dua ra file bv. So luong bit doc: 1024
int read_file(bvec &bv, FILE* f, int i)
{
	bvec a;
	
	int total = 0, i_read, pos = 0;
	
	char c;

	int temp = i/8;
	
	fseek(f, temp, SEEK_SET );
	
	i_read = fread(&c, 1, 1, f);
	
	while(i_read)
	{
		total += i_read * 8;
		
		a = itpp::dec2bin(i_read * 8, (int) c);
		
		bv.replace_mid(pos, a);
		
		pos = pos + a.length();
		
		//cout << "pos: " << pos << endl;
		
		i_read = fread(&c, 1, 1, f);
		
		//cout << i_read << endl;
		
		if(total == BS)
			break;		
	};
	return total;
}

void write_file(bvec &bv, FILE* f, int t)
{
	//cout << "Lenthg: " << t << endl;

	char b;
	
	for(int j = 0; j < t/8; j++)
	{
		b = bvec2char(bv, j * 8);
		fwrite(&b, 1, 1, f);
	}
	
	return;
}

void transfer(const char* s1, const char* s2)
{
	int constraint_length, error = 0;
	double Ec, Eb, error_rate, _BER;
	
	ivec generators;
	vec EbN0dB, EbN0, N0, ber, trans_symbols, rec_symbols;
	bvec coded_bits, decoded_bits;
	BPSK bpsk;
	BERC berc;
	AWGN_Channel channel;
	
	generators.set_size(2, false);
	generators(0) = 0133;
	generators(1) = 0145;
	constraint_length = 7;
	Convolutional_Code code;

	code.set_generator_polynomials(generators, constraint_length);
	
	Ec = 1.0;
	EbN0dB = linspace(-2, 6, 5);
	EbN0 = inv_dB(EbN0dB);
	Eb = Ec / code.get_rate();
	N0 = Eb * pow(EbN0, -1);
	
	berc.clear();
	
//cout << " " << N0(3) / 2.0 << endl;
//cout << " " << N0(2) / 2.0 << endl;
	
	channel.set_noise(N0(2) / 2.0);
	
	
	bvec bv;
	
	bv.set_size(BS);
	
	FILE* fin = fopen (s1, "rb");
	
	FILE* fout = fopen (s2, "wb");

	int i = 0, t = 0;
	
	bv.clear();
	
	t = read_file(bv, fin, i);
	
	coded_bits = code.encode(bv); //The convolutional encoder function.
	
	bpsk.modulate_bits(coded_bits, trans_symbols); //The BPSK modulator.
	
	rec_symbols = channel(trans_symbols); //The AWGN channel.
		
	decoded_bits = code.decode(rec_symbols);
	
	i = i + t;
	
	while(t > 0)
	{
		write_file(decoded_bits, fout, t);
		
		t = read_file(bv, fin, i);
		
		i += t;
		
		coded_bits = code.encode(bv); //The convolutional encoder function.
	
		bpsk.modulate_bits(coded_bits, trans_symbols); //The BPSK modulator.
	
		rec_symbols = channel(trans_symbols); //The AWGN channel.
		
		decoded_bits = code.decode(rec_symbols);
		
		berc.clear();
		
		berc.count(decoded_bits, bv);
		
		error += berc.get_errors();
	}
	
	error_rate = (double)error / i;
	
	_BER = -log10(error_rate);
	
	ofstream fBER("./BER.txt", std::ofstream::out | std::ofstream::app);
	
	fBER << "Rate 1/2: " << _BER << "\t" << error << "\t" << i << endl;
	
	fBER.close();
	
	fclose(fin);
	fclose(fout);
	return;
}

int main(int argc, char* argv[])
{
	cout << "Sending " << argv[1] << " to " << argv[2] << "...";
	
	transfer(argv[1], argv[2]);
	
	cout << "Finished." << endl;
	
	return 0;
}

