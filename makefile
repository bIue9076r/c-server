all: server client
	@echo "[Build]: All Done"

lib: lib.c
	@cc lib.c -c -o lib.out
	@echo "[Build]: Lib Done"

server: server.c lib
	@cc server.c lib.out -o server.out
	@echo "[Build]: Server Done"

client: client.c lib
	@cc client.c lib.out -o client.out
	@echo "[Build]: Client Done"

all2: server2 client2
	@echo "[Build]: All(2) Done"

server2: server2.c lib
	@cc server2.c lib.out -o server.out
	@echo "[Build]: Server(2) Done"

client2: client2.c lib
	@cc client2.c lib.out -o client.out
	@echo "[Build]: Client(2) Done"

clean:
	@rm ./*.out
	@echo "[Clean]: Clean Done"
