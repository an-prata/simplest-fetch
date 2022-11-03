default: simplest-fetch

all: simplest-fetch

simplest-fetch: src/main.c
	gcc src/main.c -o simplest-fetch

