COMPILER = gcc
CFLAGS = -Wall -g
CFLAGS2 = -g

all: bank 
.PHONY: all
.PHONY: clean

sorted-list.o: sorted-list.c sorted-list.h
	$(COMPILER) $(CFLAGS) -c sorted-list.c

tokenizer.o: tokenizer.c tokenizer.h
	$(COMPILER) $(CFLAGS) -c tokenizer.c

bank.o: bank.c bank.h
	$(COMPILER) $(CFLAGS) -c bank.c

main.o: main.c main.h
	$(COMPILER) $(CFLAGS) -c main.c

bank: main.o bank.o tokenizer.o sorted-list.o
	$(COMPILER) $(CFLAGS) -o bank main.o tokenizer.o bank.o sorted-list.o

clean: 
	rm -f *.o bank