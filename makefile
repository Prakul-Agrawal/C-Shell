all:
	# gcc -g main.c prompt/prompt.c utils/**/*.c commands/**/*.c -fsanitize=address -fsanitize=undefined -o main
	gcc -g main.c prompt/prompt.c utils/**/*.c commands/**/*.c -o main
