CC = gcc


EXEC = 	project.run	\


CFLAGS =   -g  -O3 
HEADER_FILES = $(wildcard *.h)


all: $(EXEC)

%.run: %.o mc48.o 
	$(CC) $(LDFLAGS) -o $@ $^ -lm

%.o: %.c $(HEADER_FILES)
	$(CC) -c $(CONFIG_FLAGS) $(CFLAGS) $< -o $@


clean:
	rm -f $(EXEC) *.o *~

.PHONY: clean
