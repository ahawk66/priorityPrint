#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#include <queue>
#include <fstream>

class printJobType
{
public:
	printJobType(int nPages, int aTime);

	int getNumPages();

	void setNumPages(int n);

	int getArrivalTime();

	void setArrivalTime(int t);


private:
	int numPages;
	int arrivalTime;
};

////////



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

