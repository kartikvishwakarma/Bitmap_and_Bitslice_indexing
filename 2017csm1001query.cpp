
#include "header.h"

int main(){
	string Bf;
	long cost=0;
	int width = 5;
	val ptr;
	cin>>Bf;
	//Bf = "11101111";
	cout<<(Bf.length())<<' '<<records_size<<'\n';
	if(Bf.length() != records_size){
		cout<<"ERROR: Bf length should be exactly equal to total number of records in file.\n";
		_Exit(1);
	}
	
	// formatting output
	
	cout<<setw(width)<<"indexing type"<<setw(width*1.4)<<"sum"<<' '<<setw(width*1.4)<<"cost "<<' '<<setw(width)<<"exec. time \n";
	cout<<setw(width)<<"-------------"<<setw(width*1.4)<<"---"<<' '<<setw(width*1.4)<<"-----"<<' '<<setw(width)<<"-----------\n";
	
	auto start = high_resolution_clock::now();
	ptr = no_indexing(Bf, cost);
    auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);

	cout<<setw(width)<<"no_indexing:   "<<setw(width)<<(ptr.sum)<<' '<<setw(width)<<(ptr.cost)<<' '<<setw(width)<<(duration.count())<<'\n';
	
	start = high_resolution_clock::now();
	ptr = rowid_rep(Bf, cost);
	stop = high_resolution_clock::now();
	duration = duration_cast<microseconds>(stop - start);
	cout<<setw(width)<<"row_id_rep:    "<<setw(width)<<(ptr.sum)<<' '<<setw(width)<<(ptr.cost)<<' '<<setw(width)<<(duration.count())<<'\n';
	
	start = high_resolution_clock::now();
	ptr = bitarray_rep(Bf, cost);
	stop = high_resolution_clock::now();
	duration = duration_cast<microseconds>(stop - start);
	cout<<setw(width)<<"bit_array_rep: "<<setw(width)<<(ptr.sum)<<' '<<setw(width)<<(ptr.cost)<<' '<<setw(width)<<(duration.count())<<'\n';
	
	start = high_resolution_clock::now();
	ptr = bitslice_rep(Bf, cost);
	stop = high_resolution_clock::now();
	duration = duration_cast<microseconds>(stop - start);
	cout<<setw(width)<<"bit_slice_rep: "<<setw(width)<<(ptr.sum)<<' '<<setw(width)<<(ptr.cost)<<' '<<setw(width)<<(duration.count())<<'\n';
	
	cout<<endl;	

}

val bitarray_rep(string Bf, long cost){
	set<string> file;
	//ifstream infile;
	string path, data, attr, name;
	string row_id, tras_id, tras_amnt;
	string dir = "./dataset/disk_block_";
	string page = "./bit_array_rep/";

	int file_no, index, res, pos;
	long sum=0;

	//cout<<"mask bit: "<<Bf<<'\n';
	for(int row = 0; row < Bf.length(); row++){
		file_no = row/file_size;
		pos = row%file_size;
		name = dir+to_string(file_no);

		//if(Bf[row] == '1'){
		/* look up  if bit is set or not to avoid unnecessay disk I/O */

			ifstream infile(name);
			if(!infile.is_open()){
				cout<<"ERROR:(bit_array_rep)could not open disk_block file\n";
				_Exit(1);
			}
			int line=0;

			while(infile.good()){
				//cout<<"Ok here\n";
				getline(infile, data, '\n');
				if(pos == line)
					break;
				line++;
			}

			stringstream s(data);
			s >> row_id >> tras_id >> tras_amnt;
			res = 0;
			infile.close();
			if(file.find(tras_amnt) == file.end()){
				file.insert(tras_amnt);
				for(int i=0; i<=split; i++){
					bitset<bit_array_file_limit> mask_bit(Bf.substr(i*bit_array_file_limit, bit_array_file_limit));

					if(mask_bit.count()){
						path = page+tras_amnt+'_'+to_string(i);
						ifstream infile(path);
						cost += 1;
						if(!infile.is_open()){
							cout<<"ERROR:(bit_array_rep in for) could not open disk_block file\n";
							_Exit(1);
						}
						infile >> data;
						bitset<bit_array_file_limit> data_bit(data);
						
						res += (data_bit&mask_bit).count();
						//cout<<path<<'\t'<<data<<'\n';
						//cout<<tras_amnt<<": "<<mask_bit<<' '<<data_bit<<'\n';
					}
				}
				//cout<<tras_amnt<<": "<<res<<'\n';
				sum += stoi(tras_amnt)*res;
			}
		//}
	}
	
	val tmp;
	tmp.cost = cost;
	tmp.sum = sum;

	return tmp;
}

val rowid_rep(string Bf, long cost){
	set<string> file;
	//ifstream infile;
	string path, data, attr, name;
	string row_id, tras_id, tras_amnt;
	string dir = "./dataset/disk_block_";
	string page = "./row_id_rep/";

	int file_no, index, count, pos;
	long sum=0;

	//cout<<"mask bit: "<<Bf<<'\n';
	for(int row = 0; row < Bf.length(); row++){
		file_no = row/file_size;
		pos = row%file_size;
		name = dir+to_string(file_no);
		
		if(Bf[row] == '1'){

			ifstream infile(name);
			cost += 1; 
			if(!infile.is_open()){
				cout<<"ERROR:(row_id_rep) could not open disk_block file\n";
				_Exit(1);
			}
			int line=0;

			while(infile.good()){
				//cout<<"Ok here\n";
				getline(infile, data, '\n');
				if(pos == line)
					break;
				line++;
			}

			stringstream s(data);
			s >> row_id >> tras_id >> tras_amnt;

			infile.close();
			if(file.find(tras_amnt) == file.end()){
				file.insert(tras_amnt);
				path = page+tras_amnt+"_0";
				count=0;	
				while(path.length()){
					ifstream infile(path);
					cost += 1;
					if(!infile.is_open()){
						cout<<"ERROR:(row_id_rep in while) could not open row_id_rep file\n";
						_Exit(1);
					}
					path="";
					getline(infile, data, '\n');
					getline(infile, path, '\n');
					stringstream ss(data);
					//cout<<(path)<<'\n';
					while(ss >> attr){
						index = stoi(attr);
						if(Bf[index] == '1'){
							Bf[index] = '0';
							count++;
							//cout << index<<' ';
						}
					}
							
				}
				sum += stoi(tras_amnt)*count;
				//cout<<'\n';
				//cout<<tras_amnt<<": "<<count<<'\n';
			}

		}
	}
	file.clear();

	val tmp;
	tmp.cost = cost;
	tmp.sum = sum;

	return tmp;
}

val bitslice_rep(string Bf, long cost){
	ifstream infile;
	string dir = "./bit_slice_rep/";
	string file;
	//cout<<"in bit_slice_rep\n";
	int split_file = ceil(records_size/bit_slice_file_limit);
	//cout<<split_file<<'\n';
	int res;
	long sum=0;
	for(int i=0; i<bit_len; i++){
		string data;
		//cout<<i<<":\n";
		for(int j=0; j<=split_file; j++){
			bitset<bit_slice_file_limit> mask_bit(Bf.substr(j*bit_slice_file_limit, bit_slice_file_limit));
			//cout<<mask_bit<<'\n';
			if(mask_bit.count()){
				file = dir+"bit_"+to_string(i)+'_'+to_string(j);
				infile.open(file);
				cost += 1;
				if(!infile.is_open()){
					cout<<"ERROR:(bit_slice_rep) File Not Open"<<"\n";
				_Exit(1);
				}
				//cout<<data<<'\n';
				infile >> data;
				infile.close();
				
				bitset<bit_slice_file_limit> data_bit(data);
				res = (data_bit & mask_bit).count();
				sum += (1<<i) * res;
			}
			//cout<<"res: "<<res<<'\n';
		}
	}

	val tmp;
	tmp.cost = cost;
	tmp.sum = sum;

	return tmp;
}

val no_indexing(string Bf, long cost){
	//cout<<"no_indexing..\n";
	int n = Bf.length();
	string data, file, row;
	string row_id, tras_id, tras_amnt;
	string cust_name, item_catg;

	string dir = "./dataset/";
	ifstream infile;
	int file_no;
	long sum=0;
	int i=0, index;
	cout<<"n: "<<n<<'\n';
	while(i<n){
		//cout<<Bf<<'\n';
		if(Bf[i]=='1'){
			//cout<<"condition match: "<<i<<'\n';
			file_no = i/file_size;
			index = i%file_size;

			file = dir+"disk_block_"+to_string(file_no);
			//cout<<file<<'\n';
			infile.open(file);
			cost += 1;
			if(!infile.is_open()){
				cout<<"ERROR:(no_indexing) File Not Open"<<"\n";
			_Exit(1);
			}
			int k=0;
			//cout<<index<<' '<<(infile.good())<<'\n';
			while(infile.good()){
				getline(infile, row_id, ' ');
				getline(infile, tras_id, ' ');
				getline(infile, tras_amnt, ' ');
				getline(infile, cust_name, ' ');
				getline(infile, item_catg, '\n');
				if(k==index)
					break;
				//cout<<row_id<<" "<<tras_id<<" "<<tras_amnt<<'\n';
				k++;
			}
			infile.close();
			sum += stoi(tras_amnt);
			//i += (file_no+1)*records_size;
			
		}
		i += 1;
	}

	val tmp;
	tmp.cost = cost;
	tmp.sum = sum;

	return tmp;
}
