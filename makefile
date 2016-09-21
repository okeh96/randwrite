randwrite: randwrite.o list.o
	g++ -o randwrite list.o randwrite.o 

list.o: list.cc list.h
	g++ -c list.cc

randwrite.o: randwrite.cc list.h
	g++ -c randwrite.cc

clean:
	rm -rf list.o randwrite.o randwrite