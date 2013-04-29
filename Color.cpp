//author: Lipov A.
#include "Color.h"


Color::Color()
{
  _r = 0;  
  _g = 0;
  _b = 0;
}
Color :: Color(unsigned char r, unsigned char g, unsigned char b)
{
  _r = r;
  _g = g;
  _b = b;
}

void Color :: SetR(unsigned char r)
{
  _r = r;
}

void Color :: SetG(unsigned char g)
{
  _g = g;
}

void Color :: SetB(unsigned char b)
{
  _b = b;
}

void Color :: SetColor(Color newColor)
{
 _r = newColor.GetR();
 _g = newColor.GetG();
 _b = newColor.GetB();
}

unsigned char Color :: GetR() const
{
  return _r;
}

unsigned char Color :: GetG() const
{
  return _g;
}

unsigned char Color :: GetB() const
{
  return _b;
}

Color::~Color(void)
{
}
