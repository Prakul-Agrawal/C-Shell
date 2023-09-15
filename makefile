all:
	# gcc -g main.c prompt/prompt.c utils/**/*.c commands/**/*.c -fsanitize=address -fsanitize=undefined
	gcc -g main.c prompt/prompt.c utils/**/*.c commands/**/*.c
