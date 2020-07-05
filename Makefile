objects = search.o stats.o main.o
search-cli: $(objects)
	cc -o search-cli $(objects)
main.o: main.c search.c stats.c
	cc -c main.c
stats.o: stats.c search.c
	cc -c stats.c
search.o: search.c
	cc -c search.c
.PHONY: clean
clean:
	-rm search-cli $(objects)

