#include <stdio.h>

#include <iostream>

int counter = 100;

int count() {
  counter += 1;
  std::cout << counter;
  printf("kek\n");
  return counter;
}