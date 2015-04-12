CC=gcc
LIBS=-Llib/linux/64-bit -lwebsockets -lz
INCS=-Iinclude/linux
all:
	$(CC) server.c $(LIBS) $(INCS) -o server
	$(CC) client.c $(LIBS) $(INCS) -o client
clean:
	rm server
	rm client
