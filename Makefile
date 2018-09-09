TARGET=phaser
CFLAGS=-Wall

all: html

phaser.o: phaser.c
	cc -c phaser.c -o phaser.o

phaser: phaser.o
	cc phaser.o -o phaser

html: phaser
	./phaser src

clean:
	rm -f phaser.o
	rm -f phaser
	rm -f *.html
	rm -rf blog
	rm -rf projects
