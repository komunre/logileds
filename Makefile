CXX=g++
CXX_STANDARD=c++20
CXX_FLAGS=-Wall -Wextra -pedantic -Weffc++ -Wconversion -Wsign-conversion
CXX_DEBUG=-g
LIBS=-lhidapi-hidraw -lhidapi-libusb
INCLUDE=-I/usr/include/hidapi -I./include

OBJ=src/main.o src/hid.o src/outgauge.o

all: logileds

logileds: $(OBJ)
	$(CXX) -o $@ $^ $(LIBS)

$(OBJ): %.o: %.cpp
	$(CXX) -o $@ $(CXX_FLAGS) $(CXX_DEBUG) $(INCLUDE) -c $^ $(LIBS)

clean:
	rm $(OBJ)