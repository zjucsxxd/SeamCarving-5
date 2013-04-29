//author: Lipov A.
#pragma once

#include "Picture.h"

//BMP file 24 bit

#define WORD unsigned short
#define DWORD unsigned long
#define LONG long

//file header for BMPfile format
typedef struct tagBITMAPFILEHEADER 
{ 
  WORD    bfType;        
  DWORD   bfSize;        
  WORD    bfReserved1; 
  WORD    bfReserved2; 
  DWORD   bfOffBits;     
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

//info header for BMPfile format
typedef struct tagBITMAPINFOHEADER{
  DWORD  biSize; 
  LONG   biWidth; 
  LONG   biHeight; 
  WORD   biPlanes; 
  WORD   biBitCount; 
  DWORD  biCompression; 
  DWORD  biSizeImage; 
  LONG   biXPelsPerMeter; 
  LONG   biYPelsPerMeter; 
  DWORD  biClrUsed; 
  DWORD  biClrImportant; 
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

class BMPpicture : public Picture
{
private:
  static const short BMP_SIGNATURE = 0x4D42; //"BM", first two bytes in BMPfile
  int _height;
  int _width;	
  std::vector<Color> _colorData;
  tagBITMAPFILEHEADER _myFileHeader;      
  tagBITMAPINFOHEADER _myInfoHeader;

public:
  BMPpicture(std::string FileName);
  int GetHeight() const;
  int GetWidth() const;	 
  void SetBitmap(std::vector<Color> const &newBitmap, int newWidth, int newHeight);
  Color GetColor(int x, int y) const;
  std::vector<Color> GetColorMatrix() const;
  int Save(std::string FileName);
  virtual ~BMPpicture(void);
};
