.PHONY: all clean

CC = g++
TARGET = main.exe
SOURCE = $(wildcard ./src/*.cpp)
OBJECT = $(patsubst ./src/%.cpp, ./build/%.o, $(SOURCE))
CFLAGS = -I ./include -DSDL_MAIN_HANDLED -std=c++11 -O2 -DDEBUG
LIBS = -L ./lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

all : $(TARGET)
	$(TARGET)

$(TARGET) : $(OBJECT)
	$(CC) $(OBJECT) -o $(TARGET) $(LIBS)

./build/%.o : ./src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f ./build/*.o
