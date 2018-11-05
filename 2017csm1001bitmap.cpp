#include "header.h"


int main(int argc, char const *argv[])
{
	DIR *dir;
	struct dirent *ent;
	string file = "./dataset/disk_block_0";
	regex b(prefix);
	
	/*	create directory for row id and bit array representation,
		where respective block file will be stored.
	*/
	system("mkdir ./row_id_rep");
	system("mkdir ./bit_array_rep");

	/* opening directory path and get first file of records.  
	*/
	
	/* read all file of records */
		while(file != ""){
			file = create_bitmap(file);
	}

	return 0;
}

string create_bitmap(string name){
	ifstream file(name);
	string record_id, trans_id, trans_sale_amnt;
	string cust_name, item_catg;

	if(!file.is_open()){
		cout<<"ERROR: File Not Open"<<"\n";
		_Exit(1);
	}

	/*	parse file value line by line and 
		create row id and bit array representation.
	*/
	
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
		
		//cout<<trans_sale_amnt<<" "<<record_id<<'\n';
		create_row_rep(trans_sale_amnt, record_id);
		create_bit_array(trans_sale_amnt, record_id);

	}
	// all file parsed then return null 
	return "";
}

void create_row_rep(string trans_sale_amnt, string record_id){
	ifstream infile;
	ofstream outfile;
	

	/* 	create a map on sale amount, if sale_amount
		does not exist in map, create file for this 
		sale amount and put in row id  fasion.
	*/

	if(row_id_rep.find(trans_sale_amnt) == row_id_rep.end()){
		string file = row_id_dir+trans_sale_amnt+"_"+
					to_string(row_file_entry[trans_sale_amnt]/row_file_limit);
		/******************************************/
		//write_to_file(file, record_id);
		outfile.open(file, ios_base::app);
		if(!outfile.is_open()){
			cout<<"Row representation ERROR!! \n"
			<<"Unable to open file "<<file<<'\n';
			_Exit(1);
		}
		outfile<<record_id<<" ";
		outfile.close();

		/*******************************************/
		row_file_entry[trans_sale_amnt] += 1;
		row_id_rep[trans_sale_amnt] = file;
	}
	// sale amount already exist in file.
	else{

		// mapping record which file it should go. 

		string file = row_id_dir+trans_sale_amnt+"_"+
					to_string(row_file_entry[trans_sale_amnt]/row_file_limit);


		outfile.open(file, ios_base::app);
		if(!outfile.is_open()){
			cout<<"Row representation ERROR!! \n"
			<<"Unable to open file "<<file<<'\n';
			_Exit(1);
		}
		
		
		/*	if file capacity excced create new file and link current
			file to created new file.
		*/

		
		// check file size less than limit or not and put into respective file.
		if((row_file_entry[trans_sale_amnt]+1) % row_file_limit == 0){
			outfile<<record_id<<" ";
			row_file_entry[trans_sale_amnt] += 1;
			string next_file = row_id_dir+trans_sale_amnt+'_'+to_string(row_file_entry[trans_sale_amnt]/row_file_limit);
			outfile<<('\n'+ next_file);
			//outfile.close();
		}
		else{
			outfile<<record_id<<" ";
			row_file_entry[trans_sale_amnt] += 1;
			//outfile.close();
		}

		outfile.close();
	}
}

void create_bit_array(string trans_sale_amnt, string record_id){
	ifstream infile;
	ofstream outfile;

	if(bit_array_rep.find(trans_sale_amnt) == bit_array_rep.end()){
		string file = bit_array_dir+trans_sale_amnt;
		bit_array_rep[trans_sale_amnt] = file;
		string str_data="";
		for(int i=0; i<records_size;i++){
			str_data += '0';
		}
		str_data[stoi(record_id)]='1';
		
		bit_file_entry[trans_sale_amnt] = true;
		
		/*	if file capacity excced create new file and link current
			file to created new file.
		*/

		string out;
		int k = (int)ceil(records_size/bit_array_file_limit);

		for(int i=0; i<=k; i++){
			out = file+"_"+to_string(i);
			outfile.open(out, ofstream::out | ofstream::trunc);
			outfile<<(str_data.substr(bit_array_file_limit*i, bit_array_file_limit));
			outfile.close();
		}

		/*****************************************/

		str_data.clear();
	} 

	else{
		//	find file index to which file bit will set and read that file only
		//	set that index and write back to that file only. 
		int id = stoi(record_id);
		int file_no = id/bit_array_file_limit;
		
		string data;

		string file = bit_array_dir+trans_sale_amnt+"_"+to_string(file_no);
		infile.open(file);
		// store file in string update and write back //
		infile >> data;
		infile.close();
		//cout<<file<<" index: "<<index<<" "<<(data.length())<<'\n';
		int index = id%bit_array_file_limit;
		//cout<<record_id<<" "<<id<<" file_no: "<<file_no<<" index: "<<index<<"\n";
		data[index] = '1';
		outfile.open(file, ofstream::out | ofstream::trunc);
		outfile << data;
		outfile.close();
		data.clear();
	}
}
