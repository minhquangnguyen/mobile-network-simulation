#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>  
#include <vector>
using namespace std;
#define NN 45

typedef struct rates
{
	double com_rate;
	double cha_rate;
	double MOS;
} rates;

string channel_r[] = {"8/9", "7/8", "5/6", "4/5", "3/4", "2/3", "4/7", "1/2", "4/9", "2/5", "4/11", "1/3", "1/4", "1/5"};

using namespace std;
double get_compression(string& s, bool b);
double get_compression_id(string& s, bool b);
double get_channel(string& s, bool b);
void conv(rates& r);

void read3d(const char* s, const char* _s1);
void _read3d(const char* s, const char* _s1);
void _read3d(const char* s, const char* _s1)
{
	ifstream f(s);
	string str, s1, s2, s3, s4, s5, s6;
	double d;
	
	double channel_rate[1000];
	double compres_rate[1000];
	double MOS[1000];
	
	for(int i = 0; i < 1000; i++)
		channel_rate[i] = compres_rate[i] = MOS[i] = 0;
		
	int _deb = 0;
	int index = 0;
	
	while(!f.eof())
	{
		f >> s1;
		f >> s2;
		compres_rate[index] = get_compression(s2, true);
		channel_rate[index] = get_channel(s2, true);
		f >> MOS[index];
		index++;
		f >> s4;
		f >> s5;
		f >> s6;
	}
	ofstream f1(_s1);
	for(int i = 0; i < 1000; i++)
	{
		if(MOS[i] > 2 && MOS[i] < 2.5)
		{
			f1 << compres_rate[i] << "\t";
			f1 << channel_rate[i] << "\t" << MOS[i] << endl;
		}
	}
	cout << endl;	
	f1.close();
	f.close();
	return;
}

void read3d(const char* s, const char* _s1)
{
	ifstream f(s);
	std::vector<rates> *v;
	v = new vector<rates>[NN];
	
	string str, s1, s2, s3, s4, s5, s6;
	double d;
	double th[14][8];
	int sl[14][8];
	
	for(int i = 0; i < 14; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			th[i][j] = sl[i][j] = 0;
		}
	}	
	
	int _deb = 0;
	int index = 0;
	int _i, _j;
	double temp1;
	cout << "Here" << endl;
	while(!f.eof())
	{
		f >> s1;
		f >> s2;
		_i = get_channel(s2, false);
		cout << _i << endl;
		_j = get_compression(s2, false);
		cout << _j << endl;
		f >> s4;
		cout << s4 << endl;
		f >> temp1;
		cout << temp1 << endl;
		th[_i][_j] += temp1;
		sl[_i][_j] += 1;
		f >> s5;
		f >> s6;
	
	}
	cout << "Here1" << endl;
	ofstream f1(_s1);
	rates r;
	for(int i = 0; i < 14; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			
			if(sl[i][j] != 0)
				th[i][j] = th[i][j]/sl[i][j];
			else
			{
				th[i][j] = -1;
			}	
			r.com_rate = j;
			r.cha_rate = i;
			r.MOS = th[i][j];
			
			//cout << r.cha_rate << "\t" << r.com_rate << "\t" << th[i][j] << "\t" << endl;
			conv(r);
			
			//k: 0 0.0 0.1
			//k: 1 0.1 0.2
			//...k/10 k+1/10
			for(int k = 0; k < NN; k++)
			{
				
				if(th[i][j] >= (double)(k * 0.1) && th[i][j] < (double)((k+1) * 0.1))
				{
					v[k].push_back(r);
					break;
				}
			}

			f1 << r.cha_rate << "\t" << r.com_rate << "\t" << th[i][j] << "\t" ;
			//th[i][j] = th[i][j]/sl[i][j];
			f1 << "\t\t\t" << sl[i][j] << endl;
			
			//f1 << "\t" << sl[i][j] << endl;
		}
	}
	f1 << endl;
	std::vector<rates>::iterator it;
	for(int i = 0; i < NN; i++)
	{
		f1 << i * 0.1 << " : " << (i + 1) * 0.1 << endl;
		for(it = v[i].begin(); it != v[i].end(); it++)
		{
			f1 << "\t" << it->com_rate << "\t" << it->cha_rate << "\t" << it->MOS << "\t" << it->com_rate*it->cha_rate << endl;
		}
		f1 << endl;
	}
	
	
	cout << endl;	
	f1.close();
	f.close();
	return;
}

double get_compression(string& s, bool b)
{
	if(b)
	{
		if(s.find("475") != -1)
			return 4.75;
		else if(s.find("515") != -1)
			return 5.15;
		else if(s.find("59") != -1)
			return 5.9;
		else if(s.find("67") != -1)
			return 6.7;
		else if(s.find("74") != -1)
			return 7.4;
		else if(s.find("795") != -1)
			return 7.95;
		else if(s.find("102") != -1)
			return 10.2;
		else if(s.find("122") != -1)
			return 12.2;
	
	}
	else
	{
		if(s.find("475") != -1)
			return 0;
		else if(s.find("515") != -1)
			return 1;
		else if(s.find("59") != -1)
			return 2;
		else if(s.find("67") != -1)
			return 3;
		else if(s.find("74") != -1)
			return 4;
		else if(s.find("795") != -1)
			return 5;
		else if(s.find("102") != -1)
			return 6;
		else if(s.find("122") != -1)
			return 7;
	}
	return -1;
}

double get_channel(string& s, bool b)
{
	//cout << s.find("1_2") << endl;
	if(b)
	{
		if(s.find("8_9") != -1)
			return 1.125;
		else if(s.find("7_8") != -1)
			return 1.142;
		else if(s.find("5_6") != -1)
			return 1.2;
		else if(s.find("4_5") != -1)
			return 1.250;
		else if(s.find("3_4") != -1)
			return 1.333333;
		else if(s.find("2_3") != -1)
			return 1.5;
		else if(s.find("4_7") != -1)
			return 1.750;
		else if(s.find("1_2") != -1)
			return 2.000;
		else if(s.find("4_9") != -1)
			return 2.250;
		else if(s.find("2_5") != -1)
			return 2.500;
		else if(s.find("4_11") != -1)
			return 2.750;
		else if(s.find("1_3") != -1)
			return 3.000;
		else if(s.find("1_4") != -1)
			return 4.000;
		else if(s.find("1_5") != -1)
			return 5.000;
	}
	else
	{
		if(s.find("8_9") != -1)
			return 0;
		else if(s.find("7_8") != -1)
			return 1;
		else if(s.find("5_6") != -1)
			return 2;
		else if(s.find("4_5") != -1)
			return 3;
		else if(s.find("3_4") != -1)
			return 4;
		else if(s.find("2_3") != -1)
			return 5;
		else if(s.find("4_7") != -1)
			return 6;
		else if(s.find("1_2") != -1)
			return 7;
		else if(s.find("4_9") != -1)
			return 8;
		else if(s.find("2_5") != -1)
			return 9;
		else if(s.find("4_11") != -1)
			return 10;
		else if(s.find("1_3") != -1)
			return 11;
		else if(s.find("1_4") != -1)
			return 12;
		else if(s.find("1_5") != -1)
			return 13;

	}
	return -1;
}

void MOS1(const char* s, const char* _s1)
{
	ifstream f(s);
	string str, s1, s2, s3, s4, s5, s6;
	double d;
	double th[8];
	int sl[8];
	
	for(int i = 0; i < 8; i++)
	{
		th[i] = sl[i] = 0;
	}	
	
	int _deb = 0;
	int index = 0;
	int _i, _j;
	double temp1;
	
	while(!f.eof())
	{
		f >> s1;
		f >> s2;
		_j = get_compression(s2, false);
		f >> s3;
		f >> temp1;
		th[_j] += temp1;
		sl[_j] += 1;
		f >> s5;
		f >> s6;
		cout << _j << endl;
	}
	ofstream f1(_s1);
	for(int i = 0; i < 8; i++)
	{
		th[i] = th[i]/sl[i];
		f1 << i << ": " <<  th[i] << endl;
	}
	f1.close();
	
	f.close();
	return;
}

void conv(rates& r)
{
	if(r.com_rate == 0)
		r.com_rate = 4.75;
	else if(r.com_rate == 1)
		r.com_rate = 5.15;
	else if(r.com_rate == 2)
		r.com_rate = 5.9;
	else if(r.com_rate == 3)
		r.com_rate = 6.7;
	else if(r.com_rate == 4)
		r.com_rate = 7.4;
	else if(r.com_rate == 5)
		r.com_rate = 7.95;
	else if(r.com_rate == 6)
		r.com_rate = 10.2;
	else if(r.com_rate == 7)
		r.com_rate = 12.2;
 	
	
	if(r.cha_rate == 0)
		r.cha_rate = 1.125;
	else if(r.cha_rate == 1)
		r.cha_rate = 1.142;
	else if(r.cha_rate == 2)
		r.cha_rate = 1.200;
	else if(r.cha_rate == 3)
		r.cha_rate = 1.250;
	else if(r.cha_rate == 4)
		r.cha_rate = 1.333333;
	else if(r.cha_rate == 5)
		r.cha_rate = 1.500;
	else if(r.cha_rate == 6)
		r.cha_rate = 1.750;
	else if(r.cha_rate == 7)
		r.cha_rate = 2.000;
	else if(r.cha_rate == 8)
		r.cha_rate = 2.250;
	else if(r.cha_rate == 9)
		r.cha_rate = 2.500;
	else if(r.cha_rate == 10)
		r.cha_rate = 2.750;
	else if(r.cha_rate == 11)
		r.cha_rate = 3.000;
	else if(r.cha_rate == 12)
		r.cha_rate = 4.000;
	else if(r.cha_rate == 13)
		r.cha_rate = 5.000;
	return;
}

void ber(const char* s, const char* _s1)
{
	cout << "Here" << endl;
	ifstream f(s);
	string s1, s2, s3, s4, s5;
	double d;
	double ch[14];
	unsigned int error_b[14];
	unsigned int total_b[14];
	int ch_number[14];
	
	for(int i = 0; i < 14; i++)
	{
		ch[i] = ch_number[i] = 0;
	}

	cout << "Here" << endl;	

	char* pEnd;
	int c, tb, eb;
	
	while(!f.eof())
	{
		f >> s1;
		f >> s2;
		f >> s3;
		f >> s4;
		f >> s5;
		/*if(s3.find("inf") == -1)
			d = strtod(s3.c_str(), &pEnd);
		else 
			d = 0;*/
		eb = atoi(s4.c_str());
		tb = atoi(s5.c_str());
		
		s2.replace(1, 1, "_");
		c = get_channel(s2, false);
		//cout << "s2: " << s2 << " s3: " << s3 << endl;
		//ch[c] += d;
		ch_number[c]++;
		error_b[c] += eb;
		total_b[c] += tb;
		
		cout << "d: " << d;
		cout << "    c: " << c << endl;
	}
	f.close();
	ofstream f1(_s1);
	for(int i = 0; i < 14; i++)
	{
		ch[i] = error_b[i]/total_b[i];
		f1 << channel_r[i] << ": " << ch[i] << " total error: " << error_b[i] << " total bit: " << total_b[i] << " total records: " << ch_number[i]<< endl;
	}
	f1.close();
	
	return;
}

int main(int argc, char* argv[])
{
	/*char* pEnd;
	string s = "1.02153";
	double d = strtod(s.c_str(), &pEnd);
	cout << d << endl;
	return 0;*/
		
	if(strncmp(argv[1], "-m1", 3) == 0)
		MOS1(argv[2], argv[3]);
	else if(strncmp(argv[1], "-m", 2) == 0)
		read3d(argv[2], argv[3]);
	else if(strncmp(argv[1], "-b", 2) == 0)
		ber(argv[2], argv[3]);

	return 0;
}
