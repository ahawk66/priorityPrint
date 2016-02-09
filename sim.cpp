#include <iostream>
#include <string>
#include <cstdlib>

#include "simulation.h"

using namespace std;

printJobType::printJobType(int id, int nPages, int aTime)
{
	setNumPages(nPages);
	setArrivalTime(aTime);
}

int printJobType::getNumPages()
{
	return numPages;
}

void printJobType::setNumPages(int n)
{
	numPages = n;
}

int printJobType::getArrivalTime()
{
	return arrivalTime;
}

void printJobType::setArrivalTime(int t)
{
	arrivalTime = t;
}
int printJobType::getId()
{
	return id;
}
void printJobType::setId(int i)
{
	id = i;
}

////////////////////////////////////////

printerListType::printerListType(int nPrinters)
{
	setNumPrinters(nPrinters);
	printerArray= new printerType[];
}

int printerListType::setNumPrinters(int n)
{
	numPrinters = n;
}

void printerListType::getNumPrinters()
{
	return numPrinters;
}

bool printerListType::isPrinterOpen(PrinterType p)
{
	if (p.isEmpty() == true)
	{
		return true;
	}
	else
		return false;
}

PrinterType printerListType::getOpenPrinter()
{
	//unsure what to do here
}

///////////////////////////////////////

printerType::printerType(int i, int printerSpeed)
{
	setId(i);
	setPrinterSpeed(printerSpeed);
}

void printerType::addJob(printJobType newJob)
{
	//unsure how to implement
}

bool printerType::isEmpty()
{
	if (isEmpty == true)
	{
		return true;
	}
	else
		return false;
}

void printerType::setIsEmpty(bool isE)
{
	isEmpty = isE;
}

void printerType::decrementPages()
{
	//unsure how to implement
}

int printerType::getId()
{
	return id;
}
	
void printerType::setId(int i)
{
	id = i;
}

void printerType::setPrinterSpeed(int ps)
{
	printerSpeed = ps;
}
	
int printerType::getPrinterSpeed()
{
	return printerSpeed;
}






