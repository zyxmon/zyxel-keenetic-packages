helloworld: hello.o
	$(CC) $(LDFLAGS) hello.o -o helloworld

hello.o: hello.c
	$(CC) $(CFLAGS) -c hello.c

clean:
	rm *.o helloworld

