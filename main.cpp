//author: Lipov A.


#include <iostream>
#include "Picture.h"
#include "BMPpicture.h"
#include "SimpleEnergy.h"
#include "SeamCarver.h"
#include <memory>
#include <stdexcept>
#include <string>

#define DEFAULT_INPUT_FILE_NAME "input.bmp"
#define DEFAULT_OUTPUT_FILE_NAME "output.bmp"

int main()
{  
  //demonstation of seam carving
  try
  {
    std::cout<<"Seam Carving"<<std::endl;
	std::cout<<"Input filename(default:"<<DEFAULT_INPUT_FILE_NAME<<") >>";
	std::string FileName;	
	std::getline(std::cin, FileName, '\n');
	if (FileName.size() == 0)
	  FileName = DEFAULT_INPUT_FILE_NAME;
    std::auto_ptr<Picture> myPic(new BMPpicture(FileName));  
    std::cout<<"Image size = "<<myPic->GetWidth()<<"x"<<myPic->GetHeight()<<std::endl;  
    std::auto_ptr<Energy> myEnergy(new SimpleEnergy);  
    SeamCarver mySeamCarver;          
    int vertN = 0; 
	int horN = 0;	
	do
	{
	  std::cout<<"Horizontal lines to remove(0-"<<myPic->GetHeight() - 1<<") >>";
      std::cin>>horN;
	  if (horN >= myPic->GetHeight() || horN < 0)
	  {
	    std::cout<<"Input number should be between 0 and "<<myPic->GetHeight() - 1<<std::endl;
	  }
	}
	while(horN >= myPic->GetHeight() || horN < 0);
	do 
	{
      std::cout<<"Vertical lines to remove(1-"<<myPic->GetWidth() - 1<<") >>";
	  std::cin>>vertN;
	  if (vertN >= myPic->GetWidth() || vertN < 0)
	  {
	    std::cout<<"Input number should be between 0 and "<<myPic->GetWidth() - 1<<std::endl;
	  }
	}
	while  (vertN >= myPic->GetWidth() || vertN < 0);
	std::cout<<"Processing...Please wait"<<std::endl;
    for (int i = 0; i < vertN; i++)
	{
	  mySeamCarver.RemoveVerticalSeam(*myPic, *myEnergy, mySeamCarver.FindVerticalSeam(*myPic, *myEnergy));
	}
    for (int i = 0; i < horN; i++)
    {
	  mySeamCarver.RemoveHorizontalSeam(*myPic, *myEnergy, mySeamCarver.FindHorizontalSeam(*myPic, *myEnergy));	  
    } 	  
	std::cout<<"Done!"<<std::endl;    
	std::cout<<"Output filename(default: "<<DEFAULT_OUTPUT_FILE_NAME<<") >>";
	std::cin.ignore();
	std::getline(std::cin, FileName, '\n');
	if (FileName.size() == 0)
		FileName = DEFAULT_OUTPUT_FILE_NAME;	
    myPic->Save(FileName);
    std::cout<<"Saved!"<<std::endl;  
  }
  catch (std::exception& e)
  {
	  std::cout << e.what() << std::endl;
	  return -1;
  }
  return 0;
}