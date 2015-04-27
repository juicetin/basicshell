# Justin Ting, 430203826 - Operating Systems Internals Assignment 1
# Monday 10am-12pm lab - Tutor: Jeshua
# Makefile

myshell: main.c commands.c commands.h helpers.c helpers.h
	@clang -Wall commands.c helpers.c main.c -o myshell
	@echo

clean:
	@rm -f *.o
	@rm myshell

run:
	@./myshell
