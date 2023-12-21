all:
	gcc -g raytracer.c objects.c background.c types.c main.c -lm
