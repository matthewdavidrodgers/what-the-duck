# The Ugly Duckling: Look Who's Ugly Now

A video game created for a class about what happened after the famous children's tale "The Ugly Duckling".

This game, and the engine underneath, was written by me in c++ with the SDL framework.

## Installation

Clone the repo and navigate to the directory
```
git clone https://github.com/matthewdavidrodgers/what-the-duck.git
cd what-the-duck
```
Build the project using one of the provided makefiles. Due to compiler differences, specify whether you are building on a mac or linux machine.
For OSX:
```
make -f makefile.mac
```
For linux:
```
make -f makefile.linux
```

## Running and playing the game

To play, run the executable creating from the build.
```
./run
```

## Cleanup

To remove object files and executables, run the make clean command, specifying your OS again.
For OSX:
```
make clean -f makefile.mac
```
For linux:
```
make clean -f makefile.linux
```
