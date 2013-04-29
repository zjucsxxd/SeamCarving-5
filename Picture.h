//author: Lipov A.
#pragma once

#include "Color.h"
#include <vector>

class Picture
{
public:
  Picture();
  virtual int GetHeight()const = 0 ;
  virtual int GetWidth()const = 0;	
  virtual void SetBitmap(std::vector<Color> const &newBitmap, int newWidth, int newHeight) = 0;
  virtual Color GetColor(int x, int y)const = 0;
  virtual std::vector<Color> GetColorMatrix()const = 0;
  virtual int Save(std::string FileName) = 0;
  virtual ~Picture(void);
};
