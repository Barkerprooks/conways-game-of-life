#!/usr/bin/env bash

BIN="game-of-life_$(uname -a).bin"

function build() {
	mkdir -p bin
	gcc -o bin/$BIN src/main.c -l curses
	ln -sf $PWD/bin/$BIN $PWD/run
}

if [[ $1 == '-c' || $1 == '--clean' ]];
then
	rm -rf bin
	rm -rf run
	exit	
fi

if [ -f '/usr/include/curses.h' ]; 
then
	build

elif [[ $1 == '-f' || $1 == '--force' ]];
then
	echo "curses not detected on this system, trying to build anyway"
	build
else
	echo "curses not detected on this system, use -f to force"
fi
