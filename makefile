all: server client
	@echo "[Build]: All Done"

server: server.c
	@cc server.c -o server.out
	@echo "[Build]: Server Done"

client: client.c
	@cc client.c -o client.out
	@echo "[Build]: Client Done"

clean:
	@rm ./*.out
	@echo "[Clean]: Clean Done"
