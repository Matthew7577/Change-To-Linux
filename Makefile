# Define variables
CXX = g++
WINDRES = windres
SRC_DIR = .
RES_DIR = ./res
BIN_DIR = ./bin
TARGET = $(BIN_DIR)/ChangeToLinux.exe

# Define the source files
SOURCES = $(SRC_DIR)/ChangeToLinux.cpp
RESOURCES = $(RES_DIR)/tux.rc

# Define the output resource file
RES_OBJ = $(RES_DIR)/tux.res

# Default target
all: $(TARGET)

# Compile the program
$(TARGET): $(SOURCES) $(RES_OBJ)
	$(CXX) $(SOURCES) $(RES_OBJ) -o $(TARGET)

# Convert the .rc file to .res file
$(RES_OBJ): $(RESOURCES)
	$(WINDRES) "$(RESOURCES)" -O coff -o "$(RES_OBJ)"

# Clean the build files
clean:
	@rm -f $(RES_OBJ) $(TARGET)
