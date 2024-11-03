all: server client
	@echo "[Build]: All Done"

server: server.c
	@cc server.c -o server.out
	@echo "[Build]: Server Done"

client: client.c
	@cc client.c -o client.out
	@echo "[Build]: Client Done"

all2: server2 client2
	@echo "[Build]: All(2) Done"

server2: server2.c
	@cc server2.c -o server.out
	@echo "[Build]: Server(2) Done"

client2: client2.c
	@cc client2.c -o client.out
	@echo "[Build]: Client(2) Done"

clean:
	@rm ./*.out
	@echo "[Clean]: Clean Done"
