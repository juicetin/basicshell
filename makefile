# Justin Ting, 430203826 - Operating Systems Internals Assignment 1
# Monday 10am-12pm lab - Tutor: Jeshua
#TODO: Echo messages in colour!

myshell: main.c commands.c commands.h helpers.c helpers.h
	@echo Compiling myshell...
	@clang -Wall commands.c helpers.c main.c -o myshell
	@echo Compiled!
	@echo

clean:
	@rm -f *.o

run:
	@./myshell
