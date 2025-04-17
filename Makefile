
CXX := g++


CXXFLAGS := -std=c++17

# build the final program
calculator: main.o calculator.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# compile main.cpp
main.o: main.cpp calculator.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

# compile calculator.cpp
calculator.o: calculator.cpp calculator.hpp
	$(CXX) $(CXXFLAGS) -c calculator.cpp

# remove build artifacts
clean:
	rm -f *.o calculator
