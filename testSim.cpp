
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
	
	//Get input paramaters
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
    
	//If user provides seed use it, otherwise use time.
    cout << "Enter seed, 0 for no seed: ";
    cin >> seed;
    if(seed!=0){
        srand(seed);
    } else {
		seed = time(NULL);
        srand(seed);
    }
    cout << endl;

	//Create our jobQueueManager and printerList that hold job queues and printers respectively
	JobQueueManager * jobQueueManager = new JobQueueManager();
	PrinterList * printerList = new PrinterList(numberOfPrintersBeingUsed, pagesPerMinute);
	
	//Our entire simulation is run in this for loop that keeps going until the specified number of jobs is finished
	int clock;
	for( clock = 1; completedJobs<numberOfPrintJobs; clock++)
	{
		//Description for how things are at the start of the clock tick
		cout<<  endl << "--- Clock: "<< clock<< " -- Completed Jobs: "<<completedJobs<<" ----- # of Jobs in JobQueues: "<< (*jobQueueManager).getNumJobs()<<" --------" <<endl;
		
		//if we need to create more jobs:
		if(clock <= numberOfPrintJobs){
			//get a random number of pages below the user specified max
			int pages = rand() % maxNumOfPages +1;
			cout<<"New Job. ID:"<<clock<<" # of pages: "<< pages<< endl;
			//ASSUMES ONE JOB PER MINUTE, THUS CLOCK IS AN IDENTIFYING JOB NUMBER
			PrintJob * newJob = new PrintJob(clock,pages,clock);
			(*jobQueueManager).addJob(*newJob);
		}
		//update printer list returns the number of printers that COMPLETE JOBS this clock tick, it assigns and process jobs... updates the printers.
	completedJobs+=(*printerList).updatePrinters(clock, (*jobQueueManager));
	
	}	
	
	//Final Printout Summary
	cout<<endl<< "------------------------SUMMARY--------------------------"<<endl;
	cout<<"Seed: "<< seed<<endl;
	cout<<"The simulation took " << (clock)<< " minutes to complete all jobs"<<endl;
	cout<<"The average print-time was "<< ((double) clock/completedJobs)<<endl<<endl;
	
	
	
	
}
