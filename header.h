#include <sys/stat.h>
#include <unistd.h>

#include <iostream>
#include <iomanip>
#include <algorithm> 
#include <chrono> 

#include <regex>
#include <dirent.h>

#include <map>
#include <set>
#include <bitset>

#include <fstream>
#include <sstream>  

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;
using namespace std::chrono; 

const int bit_len = 16;
const int theta = (int)3000;
const int lower_limit = (int)0;

const int file_size = 300;
const int records_size = (int)1e6;
const int disk_page_size = 300; 
const int trans_id_size = (int)5e6;

const int row_file_limit = (int)1e3;
const int bit_array_file_limit = (int)32e3;
const int bit_slice_file_limit = (int)32e3;

const int split = (int)ceil(records_size/bit_array_file_limit);

const char file_name[] = "./dataset/disk_block_";
const char next_file[] = "next_file: ";
const char dir_path[] = "./dataset/";

const string row_id_dir = "./row_id_rep/";
const string bit_array_dir = "./bit_array_rep/";
const string prefix = "disk_block_(.*)";


typedef struct{
	long cost;
	long sum;
} val;

val no_indexing(string Bf, long cost);
val rowid_rep(string Bf, long cost);
val bitarray_rep(string Bf, long cost);
val bitslice_rep(string Bf, long cost);


set<int> trans;
map<string, string> row_id_rep;
map<string, string> bit_array_rep;
map<string, int> row_file_entry;
map<string, bool> bit_file_entry;
map<int, string> bit_entry;

int fract=0;
int record_count;

string create_bitmap(string file);

char* random_string(int n);
int random_number(int n);
void generating_function(); 
int generate_trans_id();
void initlize_entry();
void write_to_file();
void create_row_rep(string trans_sale_amnt, string record_id);
void create_bit_array(string trans_sale_amnt, string record_id);
void create_bit_slice(string trans_sale_amnt, string record_id);
