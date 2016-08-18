CLI=client
SER=server
CC=gcc

.PHONY:all
all: $(CLI) $(SER)

$(CLI):client.c comm.c
	gcc -o $@ $^
$(SER):server.c comm.c
	gcc -o $@ $^

.PHONY:clean
clean:
	rm -f client server 
