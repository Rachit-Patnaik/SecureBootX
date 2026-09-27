CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Wall

SRC = src/crypto_engine.cpp src/tpm_module.cpp src/boot_manager.cpp src/main.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = build/secureboot

all: $(EXEC)

$(EXEC): $(OBJ)
	mkdir -p build
	$(CXX) $(OBJ) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf src/*.o build/
