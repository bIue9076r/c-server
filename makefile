all: server client
	@echo "All Done"

server: server.c
	@cc server.c -o server.out

client: client.c
	@cc client.c -o client.out

clean:
	@rm ./*.out
