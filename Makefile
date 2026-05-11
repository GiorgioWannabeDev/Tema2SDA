build:
	gcc -Wall -Wextra main.c treeFunctions.c -o search_index
run:
	./search_index
clean:
	rm search_index