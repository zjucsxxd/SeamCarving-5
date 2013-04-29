//author: Lipov A.
#pragma once
#include "energy.h"

//Simple energy class calculates pixel energy as dual gradient energy function
class SimpleEnergy :
	public Energy
{
  int _height;
  int _width;
  std::vector<double> _energyMatrix;
  std::vector<double> _energyMatrixTrans;	//transponed _energyMatrix
  static const double MAXIMUM_ENERGY_VALUE;
  double energyForPixel(int x, int y);
  double energyForTwoPixel(Color pix1, Color pix2);
  void transpone();
  bool _isCounted;
public:
  SimpleEnergy();
  void CountEnergy(Picture const &pic);
  double GetEnergy(int x, int y) const;
  std::vector<double> GetEnergyMatrix() const;
  std::vector<double> GetTransEnergyMatrix() const;	
  void RemoveVerticalSeam(std::vector<int> const &seam, Picture &pic);
  void RemoveHorizontalSeam(std::vector<int> const &seam, Picture &Pic);		
  bool IsCounted() const;
  virtual ~SimpleEnergy(void);
};
