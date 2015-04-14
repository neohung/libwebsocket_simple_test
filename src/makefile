ifeq ($(OS),Windows_NT)
	LIBS=-Llib/win32 -lwebsockets -lz
	INCS=-Iinclude/linux
	OBJS= server.exe client.exe
else
UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        LIBS=-Llib/win32 -lwebsockets -lz
		INCS=-Iinclude/linux
		OBJS= server client
    endif
endif	
CC=gcc

all:
	$(CC) server.c $(LIBS) $(INCS) -o server
	$(CC) client.c $(LIBS) $(INCS) -o client
clean:
	rm $(OBJS)
