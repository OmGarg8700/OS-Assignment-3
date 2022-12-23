all:
	gcc client.c -o client
	gcc server.c -o server

	gcc writer.c -o writer -lrt
	gcc reader.c -o reader -lrt

	gcc shared_read.c -o shared_read -lrt
	gcc shared_write.c -o shared_write -lrt

clean:
	rm ./client
	rm ./server
	rm ./writer
	rm ./reader
	rm ./shared_read
	rm ./shared_write
