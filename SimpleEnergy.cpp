//author: Lipov A.
#include "SimpleEnergy.h"
#include <iostream>

const double SimpleEnergy::MAXIMUM_ENERGY_VALUE = 195075;

SimpleEnergy::SimpleEnergy()
{	
  _isCounted = false;	
}

SimpleEnergy::~SimpleEnergy(void)
{
  //Nothing to do here	  
}

double SimpleEnergy :: energyForTwoPixel(Color pix1, Color pix2)
{
  double R, G, B;
  R = pix1.GetR() - pix2.GetR();  
  G = pix1.GetG() - pix2.GetG();  
  B = pix1.GetB() - pix2.GetB();  
  return (R*R + G*G + B*B);       
}

//function calculates energyMatrix using dual gradient energy function
void SimpleEnergy :: CountEnergy(Picture const &pic)
{  
  if (&pic == NULL)
  {
    throw std::invalid_argument("Error: pic is null");
  }
  _height = pic.GetHeight();
  _width = pic.GetWidth();      
  _energyMatrix.resize(_height * _width);
  _energyMatrixTrans.resize(_height * _width);
  for (int x = 0; x < _width; x++)
  {    
    int transRow = _height * x;
    for (int y = 0; y < _height; y++)
    {
	  int row = _width * y;
	  //borders pixels have maximum energy
	  if (x == 0 || y == 0 || x == _width - 1 || y == _height - 1)
	  {
	    _energyMatrix[x + row] = MAXIMUM_ENERGY_VALUE;
		_energyMatrixTrans[y + transRow] = _energyMatrix[x + row];
	  }
	  else
	  {
	    //energy calculated as deltaX(x, y) ^ 2 + deltaY(x,y) ^ 2
		//deltaX(x,y) is Rx(x, y) ^ 2 + Gx(x, y) ^ 2 + Bx(x, y) ^ 2
		//similarly for deltaY
	    Color pixX1, pixX2, pixY1, pixY2;		  
		pixX1 = pic.GetColor(x - 1, y);
		pixX2 = pic.GetColor(x + 1, y);
		pixY1 = pic.GetColor(x, y - 1);
		pixY2 = pic.GetColor(x, y + 1);
		_energyMatrix[x + row] = energyForTwoPixel(pixX1, pixX2) + energyForTwoPixel(pixY1, pixY2);   	    
		_energyMatrixTrans[y + transRow] = _energyMatrix[x + row];
	  }
    }
  }  
  _isCounted = true;
}


double SimpleEnergy :: GetEnergy(int x, int y) const
{
  if (x <0 || x >=_height || y < 0 || y >= _width)
  {
    throw std::invalid_argument("Error: invalid arguments for energy");
  }
  if (_isCounted)
  {
    return _energyMatrix[x + y * _width];  
  }
  else
  {
    throw std::invalid_argument("Error: energy is not counted");
  }
}

bool SimpleEnergy :: IsCounted() const
{
  return _isCounted;
}

std::vector<double> SimpleEnergy :: GetEnergyMatrix() const
{
  return _energyMatrix;
}

std::vector<double> SimpleEnergy :: GetTransEnergyMatrix() const
{
  return _energyMatrixTrans;
}

void SimpleEnergy :: RemoveVerticalSeam(std::vector<int> const &seam, Picture &pic)
{ 
  if (&seam == NULL)
  {	  
    throw std::invalid_argument("Error: seam is null");
  }
  if (&pic == NULL)
  {
    throw std::invalid_argument("Error: pic is null"); 
  }
  _height = pic.GetHeight();
  _width = pic.GetWidth() - 1;
  // when we remove seam, we must recalculate enerhyMatrix
  // however, to save time we can recalculate only values near deleted seam 
  std::vector<double> newEnergyMatrix(_width * _height);    
  for (int y = 0; y < _height; y++)
  {  
    int x = 0;
    int smallRow = _width * y;
    int row = (_width + 1) * y;
	//copy values before deleted pixel - 1
	for (x = 0; x < seam[y] - 1; x++)
	{
	  newEnergyMatrix[x + smallRow] = _energyMatrix[x + row];
	}		  	 
	//copy values after deleted pixel + 1
	for (x = seam[y] + 2; x < pic.GetWidth(); x++)
	{
	  newEnergyMatrix[(x - 1) + smallRow] = _energyMatrix[x + row];
	}	 
	if (seam[y] != 0)
	{	  
	  //calculate energy for deleted pixel - 1
	  x = seam[y] - 1;
	  if (x == 0 || y == 0 || x == _width - 1 || y == _height - 1)
	  {
	    newEnergyMatrix[x + smallRow] = MAXIMUM_ENERGY_VALUE;
	  }
	  else
	  {			
	    Color pixX1, pixX2, pixY1, pixY2;
		pixX1 = pic.GetColor(x-1, y);
		pixX2 = pic.GetColor(x+2, y);
		pixY1 = pic.GetColor(x, y-1);
		pixY2 = pic.GetColor(x, y+1);
		newEnergyMatrix[x + smallRow] = energyForTwoPixel(pixX1, pixX2) + energyForTwoPixel(pixY1, pixY2);   
	  }
	}
	if (seam[y] != pic.GetWidth() - 1)
	{	  
	  //calculate energy for deleted pixel + 1
	  x = seam[y] + 1;
	  if (x == 0 || y == 0 || x == _width || y == _height - 1)
	  {
	    newEnergyMatrix[(x - 1) + smallRow] = MAXIMUM_ENERGY_VALUE;
	  }
	  else
	  {		
	    Color pixX1, pixX2, pixY1, pixY2;
		pixX1 = pic.GetColor(x - 2, y);
		pixX2 = pic.GetColor(x + 1, y);
		pixY1 = pic.GetColor(x, y - 1);
		pixY2 = pic.GetColor(x, y + 1);
		newEnergyMatrix[(x - 1) + smallRow] = energyForTwoPixel(pixX1, pixX2) + energyForTwoPixel(pixY1, pixY2);   
	  }
	}	 
  }     	
  _energyMatrix = newEnergyMatrix;
  transpone();
}

void SimpleEnergy :: RemoveHorizontalSeam(std::vector<int> const &seam, Picture &pic)
{
  if (&seam == NULL)
  {	  
    throw std::invalid_argument("Error: seam is null");
  }
  if (&pic == NULL)
  {
    throw std::invalid_argument("Error: pic is null"); 
  }
  _height = pic.GetHeight() - 1;
  _width = pic.GetWidth();
  // when we remove seam, we must recalculate enerhyMatrix
  // however, to save time we can recalculate only values near deleted seam 
  std::vector<double> newEnergyMatrix(_width * _height);	
  for (int x = 0; x < _width; x++)
  {
    int y = 0;
	// copy values before deleted pixel - 1
    for (y = 0; y < seam[x] - 1; y++)
	{
	  newEnergyMatrix[x + y * _width] = _energyMatrix[x + y * pic.GetWidth()];
	}		  	 
	// copy values after deleted pixel + 1
	for (y = seam[x] + 2; y < pic.GetHeight(); y++)
	{
	  newEnergyMatrix[x + (y - 1) * _width] = _energyMatrix[x + y * pic.GetWidth()];
	}
	if (seam[x] != 0)
    {	  
	  //calculate energy for deleted pixel - 1
	  y = seam[x] - 1;
	  if (x == 0 || y == 0 || x == _width - 1 || y == pic.GetHeight() - 2)
	  {
	    newEnergyMatrix[x + y * _width] = MAXIMUM_ENERGY_VALUE;
	  }
	  else
	  {				
	    Color pixX1, pixX2, pixY1, pixY2;
	    pixX1 = pic.GetColor(x - 1, y);
	    pixX2 = pic.GetColor(x + 1, y);
	    pixY1 = pic.GetColor(x, y - 1);
 	    pixY2 = pic.GetColor(x, y + 2);
	    newEnergyMatrix[x + y * _width] = energyForTwoPixel(pixX1, pixX2) + energyForTwoPixel(pixY1, pixY2);   
	  }
    }
    if (seam[x] != pic.GetHeight() - 1)
    {	  
	  //calculate energy for deleted pixel + 1
      y = seam[x] + 1;
      if (x == 0 || y == 0 || x == _width - 1 || y == pic.GetHeight() - 1)
      {
        newEnergyMatrix[x + (y - 1) * _width] = MAXIMUM_ENERGY_VALUE;
      }
      else
      {				
        Color pixX1, pixX2, pixY1, pixY2;
        pixX1 = pic.GetColor(x - 1, y);
        pixX2 = pic.GetColor(x + 1, y);
        pixY1 = pic.GetColor(x, y - 2);
        pixY2 = pic.GetColor(x, y + 1);
        newEnergyMatrix[x + (y - 1) * _width] = energyForTwoPixel(pixX1, pixX2) + energyForTwoPixel(pixY1, pixY2);   
      }
    }	 
  }		
  _energyMatrix = newEnergyMatrix;
  transpone();
}

void SimpleEnergy :: transpone()
{	
  for (int i = 0; i < _width; i++)
    for (int j = 0; j < _height; j++)
      {
        _energyMatrixTrans[j + i * _height] = _energyMatrix[i + j * _width];
      }
}
