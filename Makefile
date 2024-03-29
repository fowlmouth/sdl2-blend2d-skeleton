SRC := $(wildcard src/*.cpp src/*/*.cpp)
OBJ := $(SRC:.cpp=.o)

BIN := game

CXXFLAGS := $(shell pkg-config --cflags sdl2) -std=c++20
LDFLAGS := $(shell pkg-config --libs sdl2) -lblend2d

ALL: $(BIN)
.PHONY: clean debug release

%.o: %.cpp entt
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ) $(LDFLAGS)

debug: CXXFLAGS += -g -O0 -DDEBUG
debug: $(BIN)

release: CXXFLAGS += -O3 -DRELEASE
release: $(BIN)

clean:
	rm -f $(OBJ) $(BIN)
