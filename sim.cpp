#include <iostream>
#include <string>
#include <cstdlib>

#include "simulation.h"

using namespace std;

//initializes our job queues
JobQueueManager::JobQueueManager()
{
    jobQueueOne = new PrintJobQueue();
	jobQueueTwo = new PrintJobQueue();
	jobQueueThree = new PrintJobQueue();
}
//Gets number of jobs
int JobQueueManager::getNumJobs(){
	int sum = (*jobQueueOne).size()+(*jobQueueTwo).size()+(*jobQueueThree).size();
	return sum;
}

// GetJob pops a job from the lowest queue and returns it
PrintJob JobQueueManager::getJob()
{
	PrintJob job = *(new PrintJob(-1,-1,-1));
	if(!(*jobQueueOne).empty()){
		job = (*jobQueueOne).front();
		(*jobQueueOne).pop();
		cout << "QueueManager: Transferring Job "<< job.getId()<<" from queue 1. There are  "<< (*jobQueueOne).size()<<" Jobs left in the queue"<<endl;	
	} else if(!(*jobQueueTwo).empty()){
		job = (*jobQueueTwo).front();
		(*jobQueueTwo).pop();
		cout << "QueueManager: Transferring Job "<< job.getId()<<" from queue 2. There are  "<< (*jobQueueTwo).size()<<" Jobs left in the queue"<<endl;
		
	} else if(!(*jobQueueThree).empty()){
		job = (*jobQueueThree).front();
		(*jobQueueThree).pop();
		cout << "QueueManager: Transferring Job "<< job.getId()<<" from queue 3. There are  "<< (*jobQueueThree).size()<<" Jobs left in the queue"<<endl;
		
	}
	return job;
}

//returns if we have a job in any queue
bool JobQueueManager::hasJob()
{
	if((*jobQueueOne).empty()&& (*jobQueueTwo).empty() && (*jobQueueThree).empty()){
		return false;
	}
	return true;
}

//Takes a job and adds itt into the correct queue
int JobQueueManager::addJob(PrintJob job)
{
	int queueNum = 0;
	int numPages = job.getNumPages();
	if(numPages < 10){
		(*jobQueueOne).push(job);
		queueNum=1;
		} else if (numPages<20){
		(*jobQueueTwo).push(job);
		queueNum=2;
		} else {
		(*jobQueueThree).push(job);
		queueNum=3;
		}		
		cout<<endl <<"QueueManager: Added Job "<<job.getId() << " to queue "<<queueNum<<endl;
		return queueNum;
}

PrintJob::PrintJob(int identifier,int nPages, int arrivalT)
{
	id=identifier;
	numPages=nPages;
	arrivalTime=arrivalT;
	remainingPages=nPages;
}
//Override equivalance operator so we can compare jobs
bool operator== (PrintJob &cP1, PrintJob &cP2)
{
    return (cP1.getId() == cP2.getId());
}
bool operator!= (PrintJob &cP1, PrintJob &cP2){
	return !(cP1 == cP2);
}

//Returns number of pages in a printjob
int PrintJob::getNumPages()
{
	return numPages;
}

//returns a print jobs process time
int PrintJob::getArrivalTime()
{
	return arrivalTime;
}

//returns printjob id
int PrintJob::getId()
{
	return id;
}

//returns remaining pages in printjob
int PrintJob::getRemainingPages()
{
	return remainingPages;
}
//decrements the number of pages in the job
void PrintJob::decrementPages(int pagesToPrint){
	remainingPages= remainingPages - pagesToPrint;
}

////////////////////////////////////////

PrinterList::PrinterList(int nPrinters, int speed)
{
	numPrinters=nPrinters;
	printerArray= new Printer[numPrinters];
	for(int i=0; i < numPrinters; i++){
		printerArray[i] = *(new Printer(i,speed));
	}
}

//This is the bread and butter.
//Loops through all the printers, states its opening situation. if it doesnt have a job, gets one and starts printing, if it does, print. If its open at the end give it a new job.
int PrinterList::updatePrinters(int clock, JobQueueManager queueManager)
{
	int jobsCompleted = 0;
	for(int i=0; i < numPrinters; i++){
		 
		 bool startsOpen = printerArray[i].isOpen();
		 //Print opening situation
		 if(!startsOpen){
			 PrintJob print = printerArray[i].getCurrentPrintJob();
			 cout << "Printer "<<(i+1)<< " has Job "<< (int) print.getId()<< " Start # of Pages:"<< print.getNumPages()<<" Pages Remaining: "<< print.getRemainingPages()<<"" <<endl;
		 } else {
			 cout<<"Printer "<<(i+1)<< " is empty! "<<endl;
		 }
		 //If you dont have a job and we have a job to give
		if(startsOpen && queueManager.hasJob()){
			printerArray[i].sendJob(queueManager.getJob());
			printerArray[i].decrementPages();
			if (printerArray[i].isOpen()){
				cout<< "Printer "<<(i+1)<< " finished Job " <<((int)printerArray[i].getCurrentPrintJob().getId())<<endl;
				jobsCompleted++;
			}	
		} else if (startsOpen && !queueManager.hasJob()){
			// if it starts open and no jobs to give, do nothing
		} else if (!startsOpen){	
		//if you got a job start printing
		printerArray[i].decrementPages();
		if (printerArray[i].isOpen()){
		
			cout<< "Printer "<<(i+1)<< " finished Job " <<((int)printerArray[i].getCurrentPrintJob().getId())<<endl;
			jobsCompleted++;
			}
		}
		//If you dont have a job and your open and we have jobs to give, give you one
		if(queueManager.hasJob() && printerArray[i].isOpen()){
			printerArray[i].sendJob(queueManager.getJob());
		}
		
	}
	return jobsCompleted;
}

int PrinterList::getNumPrinters()
{
	return numPrinters;
}

bool PrinterList::isPrinterOpen()
{
	bool outcome = false;
		for(int i=0; i < numPrinters; i++){
			if(printerArray[i].isOpen()){
				outcome=true;
			}
		}
	return outcome;
}

Printer PrinterList::getOpenPrinter()
{
		for(int i=0; i < numPrinters; i++){
			if(printerArray[i].isOpen()){
				return printerArray[i];
			}
	}
}

///////////////////////////////////////

Printer::Printer(int i, int speed)
{
	id=i;
	printerSpeed =speed;
	currentJob=*(new PrintJob(-1,-1, -1));
}

//Send job really recieves jobs.
void Printer::sendJob(PrintJob newJob)
{
	cout<<"Printer "<< (getId()+1)<<" is being sent Job  "<<newJob.getId()<<endl;
	currentJob=newJob;
}

bool Printer::isOpen()
{
	return (currentJob.getRemainingPages() <=0);
}


void Printer::decrementPages()
{
	currentJob.decrementPages(printerSpeed);
}

int Printer::getId()
{
	return id;
}

PrintJob Printer::getCurrentPrintJob()
{
	return currentJob;
}
	
int Printer::getPrinterSpeed()
{
	return printerSpeed;
}

PrintJobQueue::PrintJobQueue()
{
}

