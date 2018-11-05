/* Bit representtion is from */

#include "header.h"

int main(){
	DIR *dir;
	struct dirent *ent;

	char dir_path[] = "./dataset";
	string file = "./dataset/disk_block_0";
	string prefix = "disk_block_(.*)";

	system("mkdir ./bit_slice_rep");
	
	regex b(prefix);
	initlize_entry();

	if((dir = opendir(dir_path)) == NULL){
		perror("Error in reading directory\n");
		_Exit(1);
	}
	
	else{
		while(file != ""){
			file = create_bitmap(file);
			//cout<<file<<"\n";
		}

	}
	write_to_file();
}

string create_bitmap(string name){
	ifstream file(name);
	string record_id, trans_id, trans_sale_amnt;
	string cust_name, item_catg;
		
	//cout<<name<<"\n";

	if(!file.is_open()){
		cout<<"ERROR: File Not Open"<<"\n";
		_Exit(1);
	}

	//systme("rm ./bit_slice_rep/*");

	while(file.good()){
		getline(file, record_id, ' ');
		//cout<<record_id<<"\n";
		long amount, tx_id;
		if(record_id == "next_file:"){
			getline(file, name, '\n');
			//cout<<name<<"\n";
			return name;
		}
		else if(record_id == "")
			return "";
		getline(file, trans_id, ' ');
		getline(file, trans_sale_amnt, ' ');
		getline(file, cust_name, ' ');
		getline(file, item_catg, '\n');
		//amount = stoi(trans_sale_amnt);
		//tx_id = stoi(record_id);
		
		create_bit_slice(trans_sale_amnt, record_id);
	}
	//write_to_file();
	return  "";
}

void initlize_entry(){
	for(int i=0; i<bit_len; i++)
		bit_entry[i] = "";
}


void create_bit_slice(string trans_sale_amnt, string record_id){
	
	//cout<<bit_entry[0]<<'\n';
	int amount = stoi(trans_sale_amnt);
	if(bit_entry[0].length()==bit_array_file_limit){
		//cout<<"condition meets:\n";
		write_to_file();
		//cout<<fract<<'\n';
		fract++;
	}
	for(int i=15; i>=0; i--){

		if(amount&(1<<i))
			bit_entry[i]+='1';
		else
			bit_entry[i]+='0';

		//cout<<bit_entry[i]<<'\n';
	}
}

void write_to_file(){
	ifstream infile;
	ofstream outfile;
	string bit_slice_dir = "./bit_slice_rep/";
	
	string file, data="";
	int n;

	for(int i=0; i<bit_len; i++){
		file = bit_slice_dir+"bit_"+to_string(i)+"_"+to_string(fract);
		//cout<<file<<"\n";
		outfile.open(file,  ofstream::out | ofstream::trunc);
		if(!outfile.is_open()){
			cout<<"Bit slice representation ERROR!! \n"
			<<"Unable to open file "<<file<<'\n';
			_Exit(1);
		}
		outfile << bit_entry[i];
		outfile.close();
		bit_entry[i]="";
	}
}