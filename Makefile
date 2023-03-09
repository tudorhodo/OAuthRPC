RPC = rpcgen
CC = g++
CFLAGS += -g -I/usr/include/tirpc
LDLIBS += -lnsl -ltirpc
PROGRAM = tema
SRC_CLIENT = $(PROGRAM)_clnt.c $(PROGRAM)_xdr.c

.PHONY: build clean

build: server client

server: tema_rpc_server.cpp tema_xdr.c utils.cpp protocols_server.cpp 
	$(CC) $(CFLAGS) $^ -o $@

client: tema_clnt.c tema_xdr.c tema_rpc_client.cpp
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f client server
