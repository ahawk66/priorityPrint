//yea theres nothing atm but this is the header file 
#ifndef simulation_hpp
#define simulation_hpp

#include <stdio.h>
#include <iostream>
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#include <queue>
#include <fstream>


class printerListType
{
public:
    
private:
    int numOfPages;
    
};




class printerType
{
public:
    printerType();
    
private:
    int printSpeed;
    bool busy;
    
};






class printJobType
{
public:
	printJobType(int numPages);

	int getNumPages();

	void setNumPages(int n);
private:
	int numPages;
};




#endif /* simulation_hpp */

