#Pour installer la sdl : sudo apt install libsdl1.2-dev
# Makefile.c
#

TARGET = all
C = gcc
CFLAGS = -std=c99 -Wall

#Version materielle
INCLDIRS =
LIBDIRS =

LIBS64 = `sdl-config --cflags --libs`

SRCS = $(wildcard *.c)


OBJS = ${SRCS:.c=.o}

$(TARGET): $(OBJS)
	$(C) $(CFLAGS) $(INCLDIRS) -o $@ $(OBJS) $(LIBS64)

$(OBJS):
	$(C) $(CFLAGS) $(INCLDIRS) -c $*.c


clean:
	rm -f $(OBJS) core

veryclean: clean
	rm -f $(TARGET) a.out *.*~
	rm -rf $(TARGET).dSYM
