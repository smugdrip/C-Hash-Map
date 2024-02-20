CFLAGS += -Wall -std=c99 -g
LDLIBS +=

driver: driver.o input.o map.o value.o
	gcc driver.o input.o map.o value.o -o driver $(LDLIBS)

driver.o: driver.c input.h map.h
	gcc $(CFLAGS) -c driver.c

input.o: input.c input.h
	gcc $(CFLAGS) -c input.c

map.o: map.c map.h value.h
	gcc $(CFLAGS) -c map.c

value.o: value.c value.h
	gcc $(CFLAGS) -c value.c

stringTest: stringTest.o value.o
	gcc stringTest.o value.o -o stringTest $(LDLIBS)

mapTest: mapTest.o map.o value.o
	gcc mapTest.o map.o value.o -o mapTest $(LDLIBS)

clean:
	rm -f driver stringTest mapTest *.o *.gcda *.gcno *.gcov

stringTest.o: stringTest.c value.h
	gcc $(CFLAGS) -c stringTest.c

mapTest.o: mapTest.c map.h value.h
	gcc $(CFLAGS) -c mapTest.c
