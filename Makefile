COMPILER = gcc
CFLAGS = -Wall -g 
CFLAGS2 = -lpthread

all: client bank 
.PHONY: all
.PHONY: clean

tokenizer.o: tokenizer.c tokenizer.h
	$(COMPILER) $(CFLAGS) -c tokenizer.c

sorted-list.o: sorted-list.c sorted-list.h
	$(COMPILER) $(CFLAGS) -c sorted-list.c

client.o: client.c client.h
	$(COMPILER) $(CFLAGS) -c client.c

bank.o: bank.c bank.h
	$(COMPILER) $(CFLAGS) -c bank.c

client: client.o tokenizer.o sorted-list.o 
	$(COMPILER) $(CFLAGS) -o client client.o tokenizer.o sorted-list.o $(CFLAGS2)

bank: bank.o 
	$(COMPILER) $(CFLAGS) -o bank bank.o $(CFLAGS2)

clean: 
	rm -f *.o client bankfork
