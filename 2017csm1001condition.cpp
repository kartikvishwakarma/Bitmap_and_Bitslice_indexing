#include "header.h"


int main(){
	ofstream out;
	srand(time(NULL));
	int r;
	string str = "";
	char bits[records_size];
	
	int k=0;
	int ones = (int)1e5;
	memset(bits, '0', sizeof(bits));

	while(k<ones){
		r =  rand() % ones;
		while(trans.find(r) != trans.end())
			r = rand() % records_size;
		trans.insert(r);
		bits[r] = '1';
		k++;
	}
	int count=0;

	out.open("clause", ios_base::app);
	for(int i=0; i<records_size; i++)
		out << bits[i];
	out.close();
	
	/*
	for(int i=0; i<records_size; i++)
		if(bits[i]=='1')
			count += 1;
	cout<<count<<'\n';
*/
}
