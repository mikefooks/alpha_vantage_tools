CC = g++
CFLAGS = -g -Wall -Wextra -std=c++17

INCLUDES = -I./include/ `pkg-config --cflags libcurl jsoncpp`
LIBS = `pkg-config --libs libcurl jsoncpp gsl`

SRCS = src/compute_stats.cpp

all:
	$(CC) $(CFLAGS) $(INCLUDES) -o av_tools $(SRCS) $(LIBS)
