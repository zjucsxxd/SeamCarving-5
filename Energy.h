//author: Lipov A.
#pragma once

#include "Color.h"
#include "Picture.h"
#include <vector>

class Energy
{
public:
	Energy(void);
	virtual void CountEnergy(Picture const &pic) = 0;
	virtual double GetEnergy(int x, int y) const = 0;
	virtual std::vector<double> GetEnergyMatrix() const = 0;
	virtual std::vector<double> GetTransEnergyMatrix() const = 0;	
	virtual bool IsCounted() const = 0;
	virtual void RemoveVerticalSeam(std::vector<int> const &seam, Picture &myPic) = 0;
	virtual void RemoveHorizontalSeam(std::vector<int> const &seam, Picture &myPic) = 0;
	virtual ~Energy(void);	
};
