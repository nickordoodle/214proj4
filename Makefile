COMPILER = gcc
CFLAGS = -Wall -g 
CFLAGS2 = -lpthread

all: client bankfork 
.PHONY: all
.PHONY: clean

tokenizer.o: tokenizer.c tokenizer.h
	$(COMPILER) $(CFLAGS) -c tokenizer.c

sorted-list.o: sorted-list.c sorted-list.h
	$(COMPILER) $(CFLAGS) -c sorted-list.c

client.o: client.c client.h
	$(COMPILER) $(CFLAGS) -c client.c

bankfork.o: bankfork.c bankfork.h
	$(COMPILER) $(CFLAGS) -c bankfork.c

client: client.o tokenizer.o sorted-list.o 
	$(COMPILER) $(CFLAGS) -o client client.o tokenizer.o sorted-list.o $(CFLAGS2)

bankfork: bankfork.o sorted-list.o
	$(COMPILER) $(CFLAGS) -o bankfork bankfork.o sorted-list.o $(CFLAGS2)

clean: 
	rm -f *.o client bankfork