#!/usr/bin/env bash

if [ -f "/usr/include/curses.h" ] || ([ $# -gt 1 ] && [ $1 == "-f" ]); 
then
	mkdir -p bin
	gcc -o bin/game-of-life src/main.c -l curses
	ln -sf $PWD/bin/game-of-life $PWD/life
else
	echo "curses not detected on this system, use -f to force build"
fi
