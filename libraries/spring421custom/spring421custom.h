/*
	spring421custom.h
	Created by Tim Lykins Spring 2016.
	Released into the public domain.
*/
#ifndef spring421custom_h
#define spring421custom_h

#include "Arduino.h"

class spring421custom
{
  public:
    homeflagpin(int pin);
    void zeroX();
    void checkPos();
};

#endif