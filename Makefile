CC = gcc
CFLAGS = -Wall -g

build: 
	$(CC) $(CFLAGS) main.c -o main

run: build
	./main

checker:
	./checker.py

check: build
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         ./main

clean:
	rm -f valgrind-out.txt
	rm -f main

.PHONY: clean check checker run build
