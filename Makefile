sources = $(wildcard *.c)
objects = $(addsuffix .o, $(basename $(sources)))
flags = -W -Wall
target = shell


$(target) : $(objects)
	gcc -o $(target) $(objects)

%.o : %.c
	gcc -c $(flags) $< -o $@
clean :
	rm $(target) $(objects)
