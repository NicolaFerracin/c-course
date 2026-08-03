all: points

points: lesson.c
	$(CC) -O2 lesson.c `sdl2-config --cflags` `sdl2-config --libs` -lm -o lesson30 -Wall -W

clean:
	rm -f lesson30