//author: Lipov A.
#include "SeamCarver.h"

#include <iostream>

#include <time.h>
SeamCarver::SeamCarver()
{
  //Nothing to do here	
}

SeamCarver::~SeamCarver(void)
{
  //Nothing to do here
}

std::vector<int> SeamCarver :: findSeamForPix(int const width, int const height, std::vector<double> const &energyMatrixPtr) const
{
  if (width < 0 || height < 0)
  {  
    throw std::invalid_argument("Error: invalid arguments for findSeamForPix");
  }

  if (&energyMatrixPtr == NULL)
  {
    throw std::invalid_argument("Energy matrix is NULL");
  }

  double curVal = 0;
  double newDist = 0;

  //to find seam to delete we must find seam with lowest energy sum
  std::vector<double> distMatrix(width * height, -1); //matrix of shortest distances from top to any pixel
  std::vector<int> edgeToMatrix(width * height, -1);  //matrix of pixel numbers, where value in y + 1 is shortest edge to y 
  
  //copy to dist matrix first row of energy matrix - this would be out entry point
  std::copy(&energyMatrixPtr[0], &energyMatrixPtr[width - 1], &distMatrix[0]);
  
  for (int y = 0; y < height - 1; y++)
  {
	//we consider next row and relax edge to every bottom neighbor
	int row = y * width;
	int rowPlus = (y + 1) * width;
	
    for (int x = 0; x < width; x++)
    {
	  //
	  //| |s| |
	  //| | |*|
	  //
      if (distMatrix[x + row] != -1)
      {		
        curVal = distMatrix[x + row];
        if (x != 0)
        {
		  //calculating possible distance to this pixel
          newDist = curVal + energyMatrixPtr[(x - 1) + rowPlus];
		  //if never been to this pixel before - consider this distance as shortest path
          if (distMatrix[(x - 1) + rowPlus] == -1)
          {
            distMatrix[(x - 1)+ rowPlus] = newDist;
            edgeToMatrix[(x - 1) + rowPlus] = x;
          }
          else
	      //if new distance shorter than distance before - consider this distance as shortest path
          {
            if (newDist < distMatrix[(x - 1) + rowPlus])
            {
              distMatrix[(x - 1) + rowPlus] = newDist;
              edgeToMatrix[(x - 1) + rowPlus] = x;
            }                          
          }
        }
		//
		//| |s| |
		//|*| | |
		//
        if (x != width - 1)
        {
          newDist = curVal + energyMatrixPtr[(x + 1) + rowPlus];
          if (distMatrix[(x + 1) + rowPlus] == -1)
          {
            distMatrix[(x + 1) + rowPlus] = newDist;
            edgeToMatrix[(x + 1) + rowPlus] = x;
          }
          else
          {
            if (newDist < distMatrix[(x + 1) + rowPlus])
            {
              distMatrix[(x + 1) + rowPlus] = newDist;
              edgeToMatrix[(x + 1) + rowPlus] = x;
            }                          
          }
        }
		//
		//| |s| |
		//| |*| |
		//
        newDist = curVal + energyMatrixPtr[x + rowPlus];
        if (distMatrix[x + rowPlus] == -1)
        {
          distMatrix[x + rowPlus] = newDist;
          edgeToMatrix[x + rowPlus] = x;
        }
        else
        {
          if (newDist < distMatrix[x + rowPlus])
          {
            distMatrix[x + rowPlus] = newDist;
            edgeToMatrix[x + rowPlus] = x;
          }                          
        }
      }
    }   
  }
  
  //calculated all possible distance
  int lastRow = (height - 2) * width;
  double min = distMatrix[lastRow];
  int minx = 0;
  //find minimum distance at the last row - finish point of shortest path
  for (int x = 1; x < width; x++)
  {	
    if (distMatrix[x + lastRow] < min)
	{
	  min = distMatrix[x + lastRow];
	  minx = x;
	}
  }

  std::vector<int> path(height);  
  path[height - 1] = minx;
  //by moving up on edge matrix, we restore shortest path
  for (int y = height - 2; y >= 0; y--)
  {           
    path[y] = edgeToMatrix[path[y + 1] + (y + 1) * width];		
  } 

  //shortest path is seam to delete
  return path;
}

std::vector<int> SeamCarver :: FindVerticalSeam(Picture const &myPic, Energy &myEnergy) const
{  
  if (&myPic == NULL)
  {
    throw std::invalid_argument("Error: pic is null");		  
  }

  if (&myEnergy == NULL)
  {
    throw std::invalid_argument("Error: energy is null");
  }

  if (!myEnergy.IsCounted())
  {  
    myEnergy.CountEnergy(myPic);	  
  }
  std::vector<int> path(myPic.GetHeight());  
  path = findSeamForPix(myPic.GetWidth(), myPic.GetHeight(), myEnergy.GetEnergyMatrix());
  return path;	
}


std::vector<int> SeamCarver :: FindHorizontalSeam(Picture const &myPic, Energy &myEnergy) const 
{
  if (&myPic == NULL)
  {
    throw std::invalid_argument("Error: pic is null");		  
  }

  if (&myEnergy == NULL)
  {
    throw std::invalid_argument("Error: energy is null");
  }

  if (!myEnergy.IsCounted())
  {  
    myEnergy.CountEnergy(myPic);	  
  }
  std::vector<int> path(myPic.GetWidth());  
  //To find horizontal seam we use same function, but we use transposed matrix 
  path = findSeamForPix(myPic.GetHeight(), myPic.GetWidth(), myEnergy.GetTransEnergyMatrix());
  return path;
}

void SeamCarver :: RemoveVerticalSeam(Picture &myPic, Energy &myEnergy, std::vector<int> &seam) const 
{
  if (&myPic == NULL)
  {
 	throw std::invalid_argument("Error: pic is null");		  
  }
  if (&myEnergy == NULL)
  {
  	throw std::invalid_argument("Error: energy is null");
  }
  if (&seam == NULL)
  {
    throw std::invalid_argument("Error: seam is null");
  }
  if (myPic.GetWidth() <= 1)
  {
    throw std::invalid_argument("Error: nothing to delete");		  	 
  }    

  std::vector<Color> newColorData((myPic.GetWidth() - 1) * myPic.GetHeight());
  std::vector<Color> oldColorData = myPic.GetColorMatrix();
  
  std::vector<Color>::iterator myIt = oldColorData.begin();
  std::vector<Color>::iterator myNewIt = newColorData.begin();
  
  //to remove vertical seam in every row
  for (int y = 0; y < myPic.GetHeight(); y++)
  {	   
    //we copy pixels value before pixel to delete (seam[y])
	std::copy(myIt, myIt + seam[y], myNewIt);
	myIt += seam[y];
	myNewIt += seam[y] - 1;
	//skip pixel to delete and copy rest of pixels
	std::copy(myIt, myIt + (myPic.GetWidth() - seam[y]), myNewIt);
	myIt += (myPic.GetWidth() - seam[y]);
	myNewIt += (myPic.GetWidth() - seam[y]);   
  }  

  //don't forget to recalculate energy seam
  myEnergy.RemoveVerticalSeam(seam, myPic);
  //updating picture data
  myPic.SetBitmap(newColorData, myPic.GetWidth() - 1, myPic.GetHeight());    
}

void SeamCarver :: RemoveHorizontalSeam(Picture &myPic, Energy &myEnergy, std::vector<int> &seam) const 
{
  if (&myPic == NULL)
  {
    throw std::invalid_argument("Error: pic is null");		  
  }
  if (&myEnergy == NULL)
  {
  	throw std::invalid_argument("Error: energy is null");
  }
  if (&seam == NULL)
  {
  	throw std::invalid_argument("Error: seam is null");
  }
  if (myPic.GetHeight() <= 1)
  {
  	throw std::invalid_argument("Error: nothing to delete");	  
  }    
  std::vector<Color> newColorData(myPic.GetWidth() * (myPic.GetHeight() - 1));
  std::vector<Color> oldColorData = myPic.GetColorMatrix();
  //same as removing vertical seam, only we move throw columns
  for (int x = 0; x < myPic.GetWidth(); x++)
  {		
  	for (int y = 0; y < seam[x]; y++)
  	{     
  	  int row = (myPic.GetWidth() * y);
	  newColorData[x + row] = oldColorData[x + row];
	}
	for (int y = seam[x] + 1; y < myPic.GetHeight(); y++)
	{	 
  	  newColorData[x + (myPic.GetWidth() * (y - 1))] = oldColorData[x + (myPic.GetWidth() * y)];
	}
  }
  myEnergy.RemoveHorizontalSeam(seam, myPic);
  myPic.SetBitmap(newColorData, myPic.GetWidth(), myPic.GetHeight() - 1);    
}
