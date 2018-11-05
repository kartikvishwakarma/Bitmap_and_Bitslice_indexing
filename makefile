all: 2017csm1001dataset.cpp 2017csm1001bitmap.cpp 2017csm1001bitslice.cpp 2017csm1001query.cpp 2017csm1001condition.cpp
	g++ 2017csm1001bitmap.cpp -o bitmap -std=c++11
	g++ 2017csm1001bitslice.cpp -o bitslice -std=c++11
	g++ 2017csm1001dataset.cpp -o data -std=c++11
	g++ 2017csm1001query.cpp -o query -std=c++11
	g++ 2017csm1001condition.cpp -o condition -std=c++11
	
run: bitmap bitslice data condition query
	./data
	./bitmap
	./bitslice
	./condition

clean:
	rm -r ./dataset
	rm -r ./bit_array_rep 
	rm -r ./bit_slice_rep 
	rm -r ./row_id_rep
	rm clause
	find . -type f -executable -delete
	
