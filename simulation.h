#include <fstream>
#include <string>
#include <queue>

using namespace std;

class printJobType
{
public:
	printJobType(int i,int nPages, int aTime);

	int getNumPages();

	void setNumPages(int n);

	int getArrivalTime();

	void setArrivalTime(int t);

	int getId();

	void setId(int i);

private:
	int numPages;
	int arrivalTime;
	int id;
};

/////////////////////////////////////////


class printerListType
{
public:
	printerListType(int nPrinters);

	int setNumPrinters(int n);

	void getNumPrinters();

	bool isPrinterOpen(PrinterType p);

	PrinterType getOpenPrinter();

	//void decrementPages();
	
private:
	int numPrinters;
	PrinterType * printerArray;
	// to allocate in constructor do printerArray= new printerType[];
};


/////////////////////////////////////////

class printerType
{
public: 
	printerType(int i, int printerSpeed);

	void addJob(printJobType newJob);

	bool isEmpty();
	void setIsEmpty(bool isE);

	void decrementPages();

	int getId();
	void setId(int i);

	void setPrinterSpeed(int ps);
	int getPrinterSpeed();


private:
	int printerSpeed;
	int id;
	bool isEmpty;

}




