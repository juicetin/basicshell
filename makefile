# Justin Ting, 430203826 - Operating Systems Internals Assignment 1
# Monday 10am-12pm lab - Tutor: Jeshua
#TODO: Echo messages in colour!

myshell: main.c internal_commands.c internal_commands.h external_commands.c external_commands.h execute.c execute.h helpers.c helpers.h
	@echo Compiling myshell...
	@clang -c internal_commands.c -o internal_commands.o
	@clang -c external_commands.c -o external_commands.o
	@clang -c execute.c -o execute.o
	@clang -c helpers.c -o helpers.o
	@clang -c main.c -o main.o
	@clang internal_commands.o external_commands.o execute.o helpers.o main.o -o myshell
	@echo Compiled!

clean:
	@rm -f *.o

run:
	@./myshell
