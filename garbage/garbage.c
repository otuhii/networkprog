#include <stdio.h>

//simple mark&sweep gc, maybe in future i will try something more sophisticated(maybe tri-color)
//disadvs - during sweeping or scanning in general(with marking stage)(im not sure) whole program 
//gets freezed so it can be bad for online or for time-critical apps  


//https://journal.stuffwithstuff.com/2013/12/08/babys-first-garbage-collector/ - following this article

int main(int argc, char* argv)
{
  printf("Hello world!\n");

  return 0;
}


