#include <fstream>
#include <string>
#include <queue>

using namespace std;

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






/////////////////////////////////////////


class printListType
{
public:
	printListType(int nPrinters, int printerSpeed);

	int setNumPrinters(int n);

	void getNumPrinters();

	bool isPrinterOpen();

	PrinterType getOpenPrinter();

	void decrementPages();
	
private:
	int numPrinters;
	PrinterType * printerArray;
	// to allocate in constructor do printerArray= new printerType[];
};


/////////////////////////////////////////

class PrinterType
{
public: 
	PrinterType(int id, int printerSpeed);
	void addJob(printJobType newJob);
	bool isEmpty();
	void decrementPages();

private:
	int printerSpeed;

}




