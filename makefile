# Justin Ting, 430203826 - Operating Systems Internals Assignment 1
# Monday 10am-12pm lab - Tutor: Jeshua
#TODO: Echo messages in colour!

myshell: main.c commands.c commands.h
	@echo Compiling myshell...
	@clang -c commands.c -o commands.o
	@clang -c main.c -o main.o
	@clang commands.o main.o -o myshell
	@echo Compiled!

run:
	@./myshell
