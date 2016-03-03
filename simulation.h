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
	PrintJobQueue(int upperCutoff);
	PrintJobQueue();
	int getUpperCutoff();
	

private: 
	int upperCutoff;
	
};

/////////////////////////////////////////
//  JobQueueManager will hold and manage the job queues
/////////////////////////////////////////
class JobQueueManager
{
	public:
	JobQueueManager(int); // Constructor will ] generate the queues
	int addJob(int queueIndex,int jobIndex, int time); //newJob takes a printjob, sends it to a print queue, and returns queue id
	PrintJob getJob();
	bool hasJob();
	int getNumJobs();
	void addQueue(int cutoff, int index);
	private:
	PrintJobQueue *jobQueueArray;
	int numOfQueues;
	
	
};

/////////////////////////////////////////
//  Printer gets jobs, has a certain speed, and holds jobs.
/////////////////////////////////////////
class Printer
{
public: 
	Printer(int identifier = -1, int speed=-1, int cost=-1, int degradeRat=-1);

	void doJob(PrintJob newJob);

	bool isOpen();

	void decrementPages();

	int getId();

	int getPrinterSpeed();
	PrintJob getCurrentPrintJob();
	
	void setPagesTillDegrade(int pages);
	int getPagesTillDegrade();
	int getTimeTillRecharge();
	void setTimeTillRecharge(int time);
	double getTotalCost();
	int getTotalTimeSpent();
	int getTotalPages();
	int getTotalJobs();
	

private:
	int printerSpeed;
	int id;
	int cost;
	int pagesPrinted;
	int pagesTillDegrade;
	int timeTillRecharge;
	PrintJob currentJob;
	double totalCost;
	int totalTimeSpent;
	int totalPages;
	int totalJobs;

};

/////////////////////////////////////////
//  The printer list holds and generates all the printers and will tell you if they are open and how many we have.
//  It also holds the key function to update the printers(fetch and process jobs)
/////////////////////////////////////////
class PrinterManager
{
public:
	PrinterManager(int nPrinters, int degradeRate, double failRate, int rechargeRate);
	
	int updatePrinters(int clock, JobQueueManager queueManager);

	int getNumPrinters();

	bool isPrinterOpen();

	Printer getOpenPrinter();

	void addPrinter(int id, int speed, int cost);
	
	void printerSummary(int time);
	
private:
	int numPrinters;
	Printer * printerArray;
	int degradeRate;
	double failRate;
	int rechargeRate;
	// to allocate in constructor do printerArray= new printerType[];
};


/////////////////////////////////////////








