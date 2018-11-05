#include "header.h"

int main(int argc, char *argv[]){
	system("mkdir dataset");
	generating_function();

	return 0;
}

int random_number(int n){
	return rand()%n + lower_limit+1;
}

char* random_string(int n){
	char *cust_name = (char*)malloc(n*sizeof(char));

	for(int i=0; i<n; i++)
		cust_name[i] = rand()%26 + 65;
	return (char*)cust_name;
}

void generating_function(){
	ofstream file;
	int row_id, trans_id, trans_sale_id, item_catg;
	char *cust_name, buffer[100];
	int no_of_disk_block;

	no_of_disk_block = (int)ceil(records_size/disk_page_size);	
	
	srand(time(0));
	for(int disk_block = 0; disk_block <= no_of_disk_block; disk_block++){
		sprintf(buffer, "%s%d", file_name,disk_block);
		file.open(buffer, ios_base::app);
		int start_line = disk_block*disk_page_size;
		int end_line =  start_line + disk_page_size;
		
		for(int row = start_line; row < end_line; row++){
			if(row==records_size)
				return;
			row_id = row;

			trans_id = generate_trans_id();
			
			trans_sale_id = random_number(theta);
			cust_name = random_string(3);
			item_catg = random_number(1500);
			file<<row_id<<" "<<trans_id<<" "<<trans_sale_id<<" "<<cust_name<<" "<<item_catg<<"\n";
		}
		sprintf(buffer, "%s", next_file);
		file<<buffer;
		sprintf(buffer, "%s%d", file_name,disk_block+1);
		file<<buffer<<"\n";
		file.close();
	}

}

int generate_trans_id(){
	int id = random_number(trans_id_size);
	if(trans.find(id) != trans.end())
		id = random_number(trans_id_size);

	return id;
}