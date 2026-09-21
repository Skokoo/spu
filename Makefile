CC = clang++
FLAGS = -nostdlib -fno-stack-protector -march=native -O3 -ffast-math -static -s -fno-builtin
OUT = spu
SRC = spu.cpp

all: dep $(OUT)

dep:
	@command -v clang++ >/dev/null 2>&1 || (echo "Error: clang++ missing"; exit 1)

$(OUT): $(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)

.PHONY: all clean dep