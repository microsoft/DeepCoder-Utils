#CC=clang++ -stdlib=libc++ -std=c++11 -Wall
CC=g++ -std=c++11 -O3
# CC=g++ -std=c++11 -g

HOMEDIR=/home/t-mabalo

TARGET=search
CCFILES=$(wildcard *.cc)
OBJFILES=$(CCFILES:.cc=.o)

INCLUDES=
all: $(TARGET)

%.o: %.cc
	$(CC) $(INCLUDES) $(CFLAGS) $< -c -o $@

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) $^ -o $(TARGET)

clean:
	rm *.o $(TARGET)
