CXX = clang++
FLAGS = -nostdlib -fno-stack-protector -march=native -O3 -ffast-math -static -s -fno-builtin
TARGET = spu
SRC = spu.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(FLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean