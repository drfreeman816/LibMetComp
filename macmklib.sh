#!/usr/bin/env bash

# Colors
echo_black() { echo "$(tput setaf 0)$*$(tput setaf 7)"; }
echo_darkred() { echo "$(tput setaf 1)$*$(tput setaf 7)"; }
echo_green() { echo "$(tput setaf 2)$*$(tput setaf 7)"; }
echo_yellow() { echo "$(tput setaf 3)$*$(tput setaf 7)"; }
echo_purple() { echo "$(tput setaf 4)$*$(tput setaf 7)"; }
echo_pink() { echo "$(tput setaf 5)$*$(tput setaf 7)"; }
echo_cyan() { echo "$(tput setaf 6)$*$(tput setaf 7)"; }
echo_white() { echo "$(tput setaf 7)$*$(tput setaf 7)"; }
echo_grey() { echo "$(tput setaf 8)$*$(tput setaf 7)"; }
echo_red() { echo "$(tput setaf 9)$*$(tput setaf 7)"; }

clear

# My cpp lib make script
echo_green "Cleaning folder..."
rm -Rf bin lib obj
mkdir bin lib obj

# Compile library to object code
printf '\n'
echo_green "Compiling library to object file..."
time clang ./src/MetComp.cpp -std=c++1z -c -I ./inc -o ./obj/MetComp.o

# Generate library file
printf '\n'
echo_green "Generating library archive file..."
time ar rcsv ./lib/libMetComp.a ./obj/MetComp.o

# Use library in test program
printf '\n'
echo_green "Compiling main program to object file..."
time clang ./src/main.cpp -std=c++1z -c -I ./inc -o ./obj/main.o

printf '\n'
echo_green "Linking object files..."
time ld ./obj/main.o -macosx_version_min 10.11.6 -framework CoreFoundation -lSystem -L ./lib -lMetComp -o ./bin/main

#$ ld main.o -framework CoreFoundation -lSystem -L. -lfoo_static -o test_static
#clang main.cpp -v -std=c++1z -I ./inc -L ./lib -static -lMetComp -o ./bin/main

# Run test program
printf '\n'
echo_green "Running test program..."
time ./bin/main
