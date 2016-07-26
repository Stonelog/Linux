.PHONY:all
all: client service

client:client.c
	gcc -o $@ $^
service:service.c
	gcc -o $@ $^ -lpthread

.PHONY:clean
clean:
	rm -f client service
