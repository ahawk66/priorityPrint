
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#include <queue>
#include <fstream>

using namespace std;

void runSim();

int main()
{
    runSim();
    return 0;
}


void runSim()
{
    int maxNumOfPages = 1;
    int pagesPerMinute = 1;
    int numberOfPrintJobs = 1;
    int numberOfPrintersBeingUsed = 1;
    int seed = 0;
	int completedJobs = 0;

    cout << "Max number of pages per job: ";
    cin >> maxNumOfPages;
    if (maxNumOfPages < 1)
        maxNumOfPages = 1;
    cout << endl;

    cout << "Pages per minute: ";
    cin >> pagesPerMinute;
    if (pagesPerMinute < 1)
        pagesPerMinute = 1;
    cout << endl;

    cout << "Print jobs: ";
    cin >> numberOfPrintJobs;
    if (numberOfPrintJobs < 1)
        numberOfPrintJobs = 1;
    cout << endl;

    cout << "Number of printers being used: ";
    cin >> numberOfPrintersBeingUsed;
    if (numberOfPrintersBeingUsed < 1)
        numberOfPrintersBeingUsed = 1;
    cout << endl;
    
    cout << "Enter seed, 0 for no seed: ";
    cin >> seed;
    if(seed!=0){
        srand(seed);
    } else {
        srand(time(NULL));
    }
    cout << endl;

	PrintJobQueue queueA = new PrintJobQueue();
	PrintJobQueue queueB = new PrintJobQueue();
	PrintJobQueue queueC = new printJobQueue();
	
	PrinterList printerList = new PrinterList(numberOfPrintersBeingUsed);
	
    queue<int> printJobs; 
    int randPageNum;
    for (int i = 0; i < numberOfPrintJobs; ++i)
    {
        randPageNum = rand() % maxNumOfPages +1;
        printJobs.push(randPageNum);

        cout << "Print Job Added [" << randPageNum << "]" << endl;
    }
	
	for(int clock = 1; completedJobs<numberOfPrintJobs; clock++)
	{
	 int pages = printJobs.pop();
		if(pages < 10){
		queueA.push(new PrintJob(pages, clock));
		} else if (pages<20){
		queueB.push(new PrintJob(pages, clock));
		} else {
		queueC.push(new PrintJob(pages, clock));
		}
		printerList.decrementPages();
		while(printerList.isPrinterOpen != false){
			Printer openPrinter = printerList.getOpenPrinter();
			completedJobs++;
			if(queueA.empty() != false){
				openPrinter.newJob(queueA.push());
			} else if (queueB.empty()!= false){
				openPrinter.newJob(queueB.push());
			} else if (queueC.empty() != false){
				openPrinter.newJob(queueC.push());
			}
		}
			
	}
}
