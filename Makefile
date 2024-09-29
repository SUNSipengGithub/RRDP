# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -O3

# Source files
RRDP_LIB = Src/RRDP/*.h
RRDP_SRCS = Src/RRDP/*.cpp
GENERATE_SRCS = Src/Generate_Instance/*.cpp

all: RRDP GENERATE

RRDP: $(RRDP_LIB) $(RRDP_SRCS)
	$(CXX) $(CXXFLAGS) -o RRDP $(RRDP_SRCS)

GENERATE: $(GENERATE_SRCS)
	$(CXX) $(CXXFLAGS) -o GENERATE $(GENERATE_SRCS)

clean:
	rm -f RRDP GENERATE

.PHONY: all clean