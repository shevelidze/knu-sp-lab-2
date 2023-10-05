all:
	mkdir -p build
	g++ -std=c++11 -o build/lab-2 src/main.cpp


clean:
	rm -rf build