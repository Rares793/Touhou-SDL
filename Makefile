CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g -Iinclude $(shell pkg-config --cflags sdl3)
LDFLAGS := $(shell pkg-config --libs sdl3) -lSDL3_image

# Directories
SRC_DIR := sources
INC_DIR := include

# Sources — all .cpp in sources/ plus main.cpp
GAME_SOURCES := main.cpp \
                $(SRC_DIR)/AI.cpp \
                $(SRC_DIR)/bullet.cpp \
                $(SRC_DIR)/collisionBox.cpp \
                $(SRC_DIR)/enemy.cpp \
                $(SRC_DIR)/game.cpp \
                $(SRC_DIR)/movementGeometry.cpp \
                $(SRC_DIR)/player.cpp \
                $(SRC_DIR)/vec2.cpp

PREVIEW_SOURCES := previewMovement.cpp \
                   $(SRC_DIR)/movementGeometry.cpp \
                   $(SRC_DIR)/vec2.cpp

# Targets
GAME_TARGET    := game
PREVIEW_TARGET := previewMovement

# Default — build the game
all: $(GAME_TARGET)

$(GAME_TARGET): $(GAME_SOURCES)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(PREVIEW_TARGET): $(PREVIEW_SOURCES)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(GAME_TARGET) $(PREVIEW_TARGET)

.PHONY: all clean