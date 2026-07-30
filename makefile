# =========================================
# Date: 23.02.2026
# CROSS-PLATFORM MAKEFILE (C11 & C23)
# Written by: Gemini 3.1 Pro & Abdulkadir 
# =========================================

# --- 1. COMPILER SELECTION ---
CC := gcc
CXX := g++

# --- 2. DIRECTORY CONFIGURATION ---
SRC_DIR := src
INC_DIR := inc
OBJ_DIR := lib
BIN_DIR := bin
APP_NAME := app

# --- 3. FLAGS ---
# -Wall -Wextra: Allows all potential warnings (for clean code).
# -pedantic: Forces to fit C/C++ standards.
# -g: Buries the debug symbols to code.
# -I$(INC_DIR): Tells compiler regarding where to find header files.
CFLAGS := -I$(INC_DIR) -Wall -Wextra -pedantic -std=c11 -g
CXXFLAGS := -I$(INC_DIR) -Wall -Wextra -pedantic -std=c++23 -O3 -g
LDFLAGS := -lstdc++exp

# --- 4. FILE DETECTION ---
# Find all .c and .cpp files in given directories.
C_SRCS := $(wildcard $(SRC_DIR)/*.c)
CXX_SRCS := $(wildcard $(SRC_DIR)/*.cpp)

# Change all found source code file names into .o format and direct to obj folder. 
C_OBJS := $(C_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
CXX_OBJS := $(CXX_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
OBJS := $(C_OBJS) $(CXX_OBJS)

# --- 5. OS CONTROL AND COMMANDS ---
ifeq ($(OS),Windows_NT)
	TARGET := $(BIN_DIR)/$(APP_NAME).exe
	CLEAN_CMD := if exist $(OBJ_DIR) rd /s /q $(OBJ_DIR) & if exist $(BIN_DIR) rd /s /q $(BIN_DIR)
	MKDIR_CMD := if not exist $(OBJ_DIR) mkdir $(OBJ_DIR) & if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	SCREEN_CLEAR := cls
	SLEEP_CMD := timeout /t 1 /nobreak > NUL
else
	TARGET := $(BIN_DIR)/$(APP_NAME)
	CLEAN_CMD := rm -rf $(OBJ_DIR) $(BIN_DIR)
	MKDIR_CMD := mkdir -p $(OBJ_DIR) $(BIN_DIR)
	SCREEN_CLEAR := clear
	SLEEP_CMD := sleep 1
endif

# --- 6. COMPILING RULES ---

# The default. Works when written "make" or "mingw32-make". First folders, then compile.
compile: prepare $(TARGET)
	@if "$(WAS_REBUILT)"=="" @echo --- [INFO] Project Is Up To Date. Compile Stopped. ---

# Ensure folders exist.
prepare:
	@echo --- [PREPARE] Checking Folders. ---
	@$(MKDIR_CMD)

# Linking process: Take all .o files and create the executable.
$(TARGET): $(OBJS)
	@echo --- [LINK] Linking Program: $@ ---
	@$(CXX) $(OBJS) -o $@ $(LDFLAGS)
	@echo --- [SUCCESS] Compiling Completed ---
	$(eval WAS_REBUILT := 1)

# Compiling C++ files.
# $<: source file, $@: target object file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | prepare
	@echo [C++] Compile: $<
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Compiling C files.
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | prepare
	@echo [C]   Compile: $<
	@$(CC) $(CFLAGS) -c $< -o $@


run: compile
	@echo --- [RUN] Executing Program ---
	@$(TARGET) $(ARGS)
	@echo --- [SUCCESS] End Execution ---

# --- 7. CLEANING ---
# Deletes the compiled files, resets project.
clean:
	@echo --- [CLEAN] Resetting Project ---
	@$(CLEAN_CMD)
	@echo --- [SUCCESS] Reset Done ---
	@$(SLEEP_CMD) 
	@$(SCREEN_CLEAR)


# Allows even if there are folders named same as those.
.PHONY: compile prepare run clean