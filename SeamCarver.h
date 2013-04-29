//author: Lipov A.
#pragma once

#include "Picture.h"
#include "Energy.h"

class SeamCarver
{
private:
  std::vector<int>  findSeamForPix(int const width, int const height, std::vector<double> const &energyMatrixPtr) const;	
public:
  SeamCarver();
  ~SeamCarver(void);
  std::vector<int> FindHorizontalSeam(Picture const &myPic, Energy &myEnergy) const;
  std::vector<int> FindVerticalSeam(Picture const &myPic, Energy &myEnergy) const;
  void RemoveVerticalSeam(Picture &myPic, Energy &myEnergy, std::vector<int> &seam) const;
  void RemoveHorizontalSeam(Picture &myPic, Energy &myEnergy, std::vector<int> &seam) const;
};
