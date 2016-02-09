
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#include <queue>
#include <fstream>
#include "simulation.h"
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

	JobQueueManager * jobQueueManager = new JobQueueManager();
	PrinterList * printerList = new PrinterList(numberOfPrintersBeingUsed, pagesPerMinute);
	
	for(int clock = 1; completedJobs<numberOfPrintJobs; clock++)
	{
		cout<<  endl << "--- Clock: "<< clock<< " --Completed Jobs: "<<completedJobs<<" ----- #Jobs in JobQueues"<< (*jobQueueManager).getNumJobs()<<"--------" <<endl;
		if(clock != numberOfPrintJobs){
			int pages = rand() % maxNumOfPages +1;
			cout<<"Pages for job: "<< pages<< endl;
			//ASSUMES ONE JOB PER MINUTE, THUS CLOCK IS AN IDENTIFYING JOB NUMBER
			PrintJob * newJob = new PrintJob(clock,pages,clock);
			int queueNum = (*jobQueueManager).addJob(*newJob);
			
			cout<<"NewJob ID: "<< (*newJob).getId();
		}
	completedJobs+=(*printerList).updatePrinters(clock, (*jobQueueManager));
	
	}
	cout << " completed jobs: "<< completedJobs<< endl;		
}
