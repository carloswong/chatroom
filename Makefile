LDFLAGS = -lpthread

main:service.o parser.o http_handler.o main.o
	gcc -o chatroom service.o parser.o http_handler.o main.o
clean:
	rm ./*.o
