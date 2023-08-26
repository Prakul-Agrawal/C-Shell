all:
	gcc -g main.c prompt/prompt.c utils/**/*.c commands/**/*.c -o main