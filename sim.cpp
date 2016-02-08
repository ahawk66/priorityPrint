#include <iostream>
#include <string>
#include <cstdlib>

#include "simulation.h"

using namespace std;

printJobType::printJobType(int nPages, int aTime)
{
	setNumPages(nPages);
	setArrivalTime(aTime);
}

int getNumPages()
{
	return numPages;
}

void setNumPages(int n)
{
	numPages = n;
}

int getArrivalTime()
{
	return arrivalTime;
}

void setArrivalTime(int t)
{
	arrivalTime = t;
}
