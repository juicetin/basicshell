# Justin Ting, 430203826 - Operating Systems Internals Assignment 1
# Monday 10am-12pm lab - Tutor: Jeshua
#TODO: Echo messages in colour!

myshell: main.c internal_commands.c internal_commands.h external_commands.c external_commands.h helpers.c helpers.h
	@echo Compiling myshell...
	@clang -Wall internal_commands.c external_commands.c helpers.c main.c -o myshell
	@echo Compiled!

clean:
	@rm -f *.o

run:
	@./myshell
