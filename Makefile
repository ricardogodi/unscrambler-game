all:
	g++ -std=c++11 -o program main.cpp ContiguousSequence.cpp

run:
	./program

clean:
	rm program
