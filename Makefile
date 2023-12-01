hello:
	gcc sender.c -g -o send

all:
	gcc semaphore.c -g -o sem
	gcc  receiver.c -g -o rec
	gcc client.c -g -o cli
	
	
	./sem
	./rec
	./cli


clean:
	rm send sem rec cli *.txt

