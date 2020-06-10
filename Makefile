CC = g++
CFLAGS = -g -Wall -Wextra -std=c++17

INCLUDES = -I./include/ `pkg-config --cflags libcurl jsoncpp`
LIBS = `pkg-config --libs libcurl jsoncpp gsl`

CLI_ENTRY = src/cli_entry.cpp

SRCS = src/moving_average.cpp

cli_entry:
	$(CC) $(CFLAGS) $(INCLUDES) $(CLI_ENTRY) $(LIBS) -lstdc++fs -o entry 

all:
	$(CC) $(CFLAGS) $(INCLUDES) -o av_tools $(SRCS) $(LIBS)
