unscrambler: main.o
	g++ -o unscrambler main.o

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm -f unscrambler main.o
