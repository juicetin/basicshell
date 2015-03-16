# Justin Ting, 430203826 - Operating Systems Internals Assignment 1
# Monday 10am-12pm lab - Tutor: Jeshua
#TODO: Echo messages in colour!

myshell: myshell.c
	@clear
	@echo Compiling myshell...
	@clang myshell.c
	@echo Starting myshell...
	@echo Started!
	@./a.out