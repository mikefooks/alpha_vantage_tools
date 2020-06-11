CC = g++
CFLAGS = -Wall -Wextra -std=c++17 -O3

INCLUDES = -I./include/ `pkg-config --cflags libcurl jsoncpp`
LIBS = `pkg-config --libs libcurl jsoncpp gsl`

ENTRY = src/cli_entry.cpp

all:
	$(CC) $(CFLAGS) $(INCLUDES) $(ENTRY) $(LIBS) -lstdc++fs -o av_tools
