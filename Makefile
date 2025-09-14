# Makefile for CUDA Boids Simulation

# Compiler and flags
NVCC = nvcc
CXX = g++
NVCC_FLAGS = -arch=$(CUDA_ARCH) -O3 -std=c++17
CXX_FLAGS = -O3 -std=c++23 -Wno-missing-braces -Wno-deprecated-gpu-targets

# CUDA architecture (adjust based on your GPU)
CUDA_ARCH = sm_86

# Include paths
RAYLIB_INCLUDE = -I/usr/local/include
CUDA_INCLUDE = -I/usr/local/cuda/include
INCLUDE_PATHS = -I./ $(RAYLIB_INCLUDE) $(CUDA_INCLUDE)

# Library paths
RAYLIB_LIB = -L/usr/local/lib
CUDA_LIB = -L/usr/local/cuda/lib64
LIB_PATHS = $(RAYLIB_LIB) $(CUDA_LIB)

# Libraries to link
LIBS = -lraylib -lcudart -lm -lGL -lpthread -ldl -lrt -lX11

# Source files
# (Assuming your other files are .cpp, change if they are .c)
CUDA_SOURCES = simulation_kernels.cu main_cuda.cu cuda_management.cu
CPP_SOURCES = $(wildcard *.cpp)

# Object files
CUDA_OBJS = $(CUDA_SOURCES:.cu=.o)
CPP_OBJS = $(CPP_SOURCES:.cpp=.o)
OBJS = $(CUDA_OBJS) $(CPP_OBJS)

# Executable name
TARGET = cuda_boids

# Default target
all: $(TARGET)

# Link the target
$(TARGET): $(OBJS)
	$(NVCC) $(LIB_PATHS) $(OBJS) -o $@ $(LIBS)

# Compile CUDA source files
%.o: %.cu
	$(NVCC) $(NVCC_FLAGS) $(INCLUDE_PATHS) -c $< -o $@

# Compile C++ source files
%.o: %.cpp
	$(CXX) $(CXX_FLAGS) $(INCLUDE_PATHS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
