//author: Lipov A.
#pragma once

class Color
{
private:
  unsigned char _r;  //RED
  unsigned char _g;  //GREEN
  unsigned char _b;  //BLUE
public:
  Color();
  Color(unsigned char r, unsigned char g, unsigned char b);
  unsigned char GetR() const;
  unsigned char GetG() const;
  unsigned char GetB() const;
  void SetR(unsigned char r);
  void SetG(unsigned char g);
  void SetB(unsigned char b);
  void SetColor(Color newColor);
  ~Color(void);
};
