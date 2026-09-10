CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -pedantic

TARGET = cs509_labtest1
SRC = src/csr.cpp src/algorithms.cpp driver.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -I./src -o $(TARGET)

clean:
	rm -f $(TARGET)
