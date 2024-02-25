all: src/main.c src/lex.c src/parse.c src/compile.c
	gcc src/main.c src/lex.c src/parse.c src/compile.c \
	-o build/main -ansi -Wpedantic
	cp build/main test/main
