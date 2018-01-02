#!/bin/bash

# My cpp lib make scrip

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

# Select parameters
program="$1"
compiler="$2"
options="$3"
library="metcomp"
libs="-fopenmp"

clear

echo_green "Compiling lib$library with $compiler $options"

printf '\n'
echo_purple "Creating directories..."
mkdir -p obj lib bin

# Compile headers
printf '\n'
echo_purple "Precompiling headers to object files and creating library archive..."
headers=./inc/*.h
for header in $headers
do
  file=${header##*/}
  file=${file%.*}

  printf '\n'
  echo_cyan "$file"

  time icpc -Wall -c $options ./src/$file.cpp -I ./inc -o ./obj/$file.o
  ar rcs ./lib/libMetComp.a ./obj/$file.o

  if [ $compiler = "gcc" ]; then
    time g++ -Wall -c $options -std=c++17 $libs ./src/$file.cpp -I ./inc -o ./obj/$file.o
    ar rcs ./lib/lib$library.a ./obj/$file.o
  elif [ $compiler = "clang" ]; then
    time clang -v -c $options -std=c++17 $libs ./src/$file.cpp -I ./inc -o ./obj/$file.o
    libtool -static ./obj/$file.o -o ./lib/lib$library.a
  elif [ $compiler = "intel" ]; then
    time icpc -Wall -c $options $libs ./src/$file.cpp -I ./inc -o ./obj/$file.o
    ar rcs ./lib/lib$library.a ./obj/$file.o
  fi

done

# Compile program and link
printf '\n'
if [ $compiler = "gcc" ]; then
  echo_purple "Compiling $program and linking to $library..."
  time g++ -Wall $options -std=c++17 $libs ./src/$program.cpp -I ./inc -L ./lib -static -l$library -o ./bin/$program
elif [ $compiler = "intel" ]; then
  echo_purple "Compiling $program and linking to $library..."
  time icpc -Wall $options $libs -L ./lib -I ./inc ./src/$program.cpp ./lib/lib$library.a -o ./bin/$program
elif [ $compiler = "clang" ]; then
  echo_purple "Compiling $program to object file..."
  time clang -v -c $options -std=c++17 $libs -I ./inc ./src/$program.cpp -o ./obj/$program.o
  echo_purple "Linking $program and $library..."
  time ld -v ./obj/$program.o -framework CoreFoundation -lSystem -L. -l$library -o ./bin/$program
fi

# Run test program
printf '\n'
echo_purple "Running $program..."
printf '\n'
  time ./bin/$program

printf '\n'
