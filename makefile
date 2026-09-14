# NOTE : This makefile is a supplementary file for demonstration purposes, hence it only consists of simple rules.
#        When you write the makefile for your assignment, please make sure it has complete rules,
#        prerequisites, all the necessary variables, and clean rules to get full mark on makefile category.
#	     (Depending on the assignment requirement, you might even have to write CONDITIONAL COMPILATION)

demo: demoAnimation.o newSleep.o color.o
	gcc demoAnimation.o newSleep.o color.o -o demo

demoAnimation.o: demoAnimation.c newSleep.h color.h
	gcc -Wall -ansi -pedantic demoAnimation.c -c

newSleep.o: newSleep.c newSleep.h
	gcc -Wall -ansi -pedantic newSleep.c -c

color.o: color.c color.h
	gcc -Wall -ansi -pedantic color.c -c

clean:
	rm demo demoAnimation.o newSleep.o color.o	