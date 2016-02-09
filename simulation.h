#include <fstream>
#include <string>
#include <queue>


using namespace std;


//  JobQueueManager will hold and manage the job queues


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

class JobQueueManager
{
	public:
	JobQueueManager(); // Constructor will ] generate the queues
	int addJob(PrintJob job); //newJob takes a printjob, sends it to a print queue, and returns queue id
	PrintJob getJob();
	bool hasJob();
	int getNumJobs();
	private:
	PrintJobQueue jobQueuesArray  [3]; // array for jobqueues
	
	
};

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








