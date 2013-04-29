//author: Lipov A.
#include "BMPpicture.h"
#include <iostream>
#include <string>


BMPpicture :: BMPpicture(std::string FileName)
{
  FILE *inData;
  inData = fopen(FileName.c_str(), "rb");
  if (inData == NULL)
  {	
    throw std::invalid_argument("Error opening file for reading");
  }
  else
  {
    std::cout<<"Opened "<<FileName<<std::endl;
  } 
  
  // reading file header
  // sizeof(tagBITMAPFILEHEADER) = 16(any size is multiple of 4), but actually it is 14, so we subtract 2
  fread(&_myFileHeader, sizeof(tagBITMAPFILEHEADER) - 2, 1, inData); 
  
  //Checking for signature, it should be "BM"
  if (_myFileHeader.bfType != BMP_SIGNATURE)
  {
	throw std::invalid_argument("Error: opened file is not BMP");
  }
  
  // reading info header and getting necessary image parametrizes
  fread(&_myInfoHeader, sizeof(tagBITMAPINFOHEADER), 1, inData);

  _width = _myInfoHeader.biWidth;
  _height = _myInfoHeader.biHeight;

  // if line is not multiple of 4, than we fill it with padding bytes until it becomes multiple of 4
  int paddingBytes = 0;
  if ((_width * 3) % 4)
  {
    paddingBytes = 4 - (_width * 3) % 4;
  }
 
  // color order in BMP file: B-G-R
  for (int y = 0; y < _height; y++)
  {	  
    for (int x = 0; x < _width; x++)
	{	    
	  unsigned char nextR;
	  unsigned char nextG;
	  unsigned char nextB;  
	  fread(&nextB, sizeof(nextB), 1, inData);
	  fread(&nextG, sizeof(nextG), 1, inData);
	  fread(&nextR, sizeof(nextR), 1, inData);
	  Color nextColor(nextR, nextG, nextB);		
	  _colorData.push_back(nextColor);		
	}
	// filing with padding bytes (value equals 0)
	for (int x = 0; x < paddingBytes; x++)
	{
	  unsigned char emptyByte;
	  fread(&emptyByte, sizeof(emptyByte), 1, inData);		
	}
  }
  fclose(inData);  
}

int BMPpicture :: GetHeight() const
{
  return _height;	
}

int BMPpicture :: GetWidth() const
{
  return _width;  
}

void BMPpicture :: SetBitmap(std::vector<Color> const &newBitmap, int newWidth, int newHeight)
{   
  _width = newWidth;
  _height = newHeight; 
  _colorData = newBitmap;	
}

Color BMPpicture :: GetColor(int x, int y) const
{  
  if (x <0 || x > _width || y < 0 || y > _height)
  {    
    throw std::invalid_argument("Error: invalid arguments for color");
  }
  
  return _colorData[x + _width * y];		
}

int BMPpicture :: Save(std::string FileName)
{
  //similarly to opening BMP
  FILE *outData;
  outData = fopen(FileName.c_str(), "wb");
  if (outData == NULL)
  {
	throw std::invalid_argument("Error opening file for writing");
  }
  else
  {
    std::cout<<"Saving to "<<FileName<<std::endl;
  }  
  _myInfoHeader.biHeight = _height;
  _myInfoHeader.biWidth = _width;  
  _myInfoHeader.biSizeImage = _height * _width * 3;

  fwrite(&_myFileHeader, sizeof(tagBITMAPFILEHEADER) - 2, 1, outData); 
  fwrite(&_myInfoHeader, sizeof(tagBITMAPINFOHEADER), 1, outData);
  
  int paddingBytes = 0;
  if ((_width* 3) % 4)
  {
    paddingBytes = 4 - (_width * 3) % 4;
  }

  for (int y = 0; y < _height; y++)
  {	  
    for (int x = 0; x < _width; x++)
    {		  
      Color nextColor;
	  unsigned char nextR;
	  unsigned char nextG;
	  unsigned char nextB;		  
	  nextColor = GetColor(x, y);
	  nextR = nextColor.GetR();
	  nextG = nextColor.GetG();
	  nextB = nextColor.GetB();
	  fwrite(&nextB, sizeof(nextB), 1, outData);
	  fwrite(&nextG, sizeof(nextG), 1, outData);
	  fwrite(&nextR, sizeof(nextR), 1, outData);
    }	
	
	for (int x = 0; x < paddingBytes; x++)
	{
	  unsigned char emptyByte = 0;
	  fwrite(&emptyByte, sizeof(emptyByte), 1, outData);
	}
  }
  fclose(outData);
  return 0;
}


std::vector<Color> BMPpicture :: GetColorMatrix() const
{
  return _colorData;
}
BMPpicture :: ~BMPpicture(void)
{  
  //nothing to do here
}

