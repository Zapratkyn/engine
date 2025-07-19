CXX := g++
CXXFLAGS := -Wall -Wextra -Werror -std=c++17 -I./src
SRC_DIR := .
BUILD_DIR := build
SRCS := $(wildcard $(SRC_DIR)/main.cpp) \
        $(wildcard $(SRC_DIR)/*.c) \
        $(wildcard $(SRC_DIR)/Core/*.cpp) \
        $(wildcard $(SRC_DIR)/Input/*.cpp) \
        $(wildcard $(SRC_DIR)/Graphics/*.cpp)

OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(filter %.cpp,$(SRCS)))
OBJS += $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(filter %.c,$(SRCS)))

DEPS := $(OBJS:.o=.d)
TARGET := $(BUILD_DIR)/engine

LINKS := -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LINKS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

-include $(DEPS)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
