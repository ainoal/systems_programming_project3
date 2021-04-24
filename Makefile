wish: wishlib.o wish.o
	gcc wish.o wishlib.o -o reverse -Wall -pedantic
wish.o: wish.c wish.h
	gcc wish.c -c -Wall -pedantic
wishlib.o: wishlib.c wish.h
	gcc wishlib.c -c -Wall -pedantic
