CC = gcc
CFLAGS = -Wall -pthread

all: orchestrator client

orchestrator: orchestrator.c
	$(CC) $(CFLAGS) -o orchestrator orchestrator.c

client: client.c
	$(CC) $(CFLAGS) -o client client.c

clean:
	rm -f orchestrator client


