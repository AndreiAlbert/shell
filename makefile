output: main.o commands.o
	gcc main.o commands.o -o output

main.o: main.c
	gcc -c main.c

commands.o: commands.c
	gcc -c commands.c

clean: 
	rm output *.o