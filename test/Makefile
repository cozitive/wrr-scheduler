CC=aarch64-linux-gnu-gcc
CFLAGS=-static -g
TARGETS=test
OBJECTS=wrappers.o

all: $(TARGETS) $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%: %.c $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGETS) $(OBJECTS)