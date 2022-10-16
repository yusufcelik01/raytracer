CC= gcc
CXX= g++
CFLAGS = -Wall -g
CXXFLAGS = -std=c++17


all:
	g++ *.cpp -o raytracer $(CFLAGS) $(CXXFLAGS)


%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< 




.PHONY: clean
clean:
	-rm *.o 
	-rm raytracer

