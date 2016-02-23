#include <fstream>
#include <string>
#include <queue>


using namespace std;


/////////////////////////////////////////
//  PrintJob stores and/or modifies information about what needs to be printed, how much still needs to be printed, and what time it got to a printer,
/////////////////////////////////////////
class PrintJob
{
public:
	PrintJob(int identifier =-1,int nPages = -1, int aTime= -1);

	int getNumPages();
	int getArrivalTime();
	int getRemainingPages();
	int getId();
	void decrementPages(int pagesToPrint);

private:
	int numPages;
	int arrivalTime;
	int remainingPages;
	int id;
};

class PrintJobQueue: public queue<PrintJob>
{
public:
	PrintJobQueue();
	
};

/////////////////////////////////////////
//  JobQueueManager will hold and manage the job queues
/////////////////////////////////////////
class JobQueueManager
{
	public:
	JobQueueManager(); // Constructor will ] generate the queues
	int addJob(PrintJob job); //newJob takes a printjob, sends it to a print queue, and returns queue id
	PrintJob getJob();
	bool hasJob();
	int getNumJobs();
	private:
	PrintJobQueue* jobQueueOne; // array for jobqueues
	PrintJobQueue* jobQueueTwo;
	PrintJobQueue* jobQueueThree;
	
};

/////////////////////////////////////////
//  Printer gets jobs, has a certain speed, and holds jobs.
/////////////////////////////////////////
class Printer
{
public: 
	Printer(int identifier = -1, int speed=-1);

	void sendJob(PrintJob newJob);

	bool isOpen();

	void decrementPages();

	int getId();

	int getPrinterSpeed();
	PrintJob getCurrentPrintJob();


private:
	int printerSpeed;
	int id;
	PrintJob currentJob;

};

/////////////////////////////////////////
//  The printer list holds and generates all the printers and will tell you if they are open and how many we have.
//  It also holds the key function to update the printers(fetch and process jobs)
/////////////////////////////////////////
class PrinterList
{
public:
	PrinterList(int nPrinters, int speed);
	
	int updatePrinters(int clock, JobQueueManager queueManager);

	int getNumPrinters();

	bool isPrinterOpen();

	Printer getOpenPrinter();

	
private:
	int numPrinters;
	Printer * printerArray;
	// to allocate in constructor do printerArray= new printerType[];
};


/////////////////////////////////////////








