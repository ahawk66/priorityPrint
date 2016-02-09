#include <iostream>
#include <string>
#include <cstdlib>

#include "simulation.h"

using namespace std;

JobQueueManager::JobQueueManager()
{
	jobQueuesArray[0] = *(new PrintJobQueue());
	jobQueuesArray[1] = *(new PrintJobQueue());
	jobQueuesArray[2] = *(new PrintJobQueue());
}
int JobQueueManager::getNumJobs(){
	int sum = 0;
	for(int i = 0; i<3; i++){
		sum+=jobQueuesArray[i].size();
	}
	return sum;
}
PrintJob JobQueueManager::getJob()
{
	 cout<<endl<<"GETJOB"<<endl;
	PrintJob job = *(new PrintJob(-1,-1,-1));
	if(!jobQueuesArray[0].empty()){
		//cout<<"true";
		job = jobQueuesArray[0].front();
		jobQueuesArray[0].pop();
		cout << "getJob gives job"<< job.getId()<<" from queue 1 "<< " size "<< jobQueuesArray[0].size()<<endl;
		
	} else if(!jobQueuesArray[1].empty()){
		//cout<<"true";
		job = jobQueuesArray[1].front();
		jobQueuesArray[1].pop();
		cout << "getJob gives job"<< job.getId()<<" from queue 2 "<< " size "<< jobQueuesArray[1].size()<<endl;
		
	} else if(!jobQueuesArray[2].empty()){
		//cout<<"true";
		job = jobQueuesArray[2].front();
		jobQueuesArray[2].pop();
		cout << "getJob gives job"<< job.getId()<<" from queue 3 "<< " size "<< jobQueuesArray[2].size()<<endl;
		
	}
	
//	cout<<queue;
//	cout<<endl<<"getjob "<< job.getRemainingPages()<<endl;
	return job;
}

bool JobQueueManager::hasJob()
{
	if(jobQueuesArray[0].empty()&& jobQueuesArray[1].empty() && jobQueuesArray[2].empty()){
		return false;
	}
	return true;
}
int JobQueueManager::addJob(PrintJob job)
{
	int queueNum = 0;
	int numPages = job.getNumPages();
//	cout<<endl<<"addjob "<< job.getRemainingPages()<<endl;
	if(numPages < 10){
		jobQueuesArray[0].push(job);
		queueNum=1;
		} else if (numPages<20){
		jobQueuesArray[1].push(job);
		queueNum=2;
		} else {
		jobQueuesArray[2].push(job);
		queueNum=3;
		}
	//	cout<<endl<<queueNum<<" "<<endl;
	//	cout<<jobQueuesArray[queueNum-1].empty();
	//	cout<<endl<<"addjob2"<<jobQueuesArray[queueNum-1].front().getRemainingPages();
		
		cout<<endl <<"AddJob added job "<<job.getId() << " to queue "<<queueNum<<endl;
		return queueNum;
}

PrintJob::PrintJob(int identifier,int nPages, int arrivalT)
{
	id=identifier;
	numPages=nPages;
	arrivalTime=arrivalT;
	remainingPages=nPages;
}
bool operator== (PrintJob &cP1, PrintJob &cP2)
{
    return (cP1.getId() == cP2.getId());
}
bool operator!= (PrintJob &cP1, PrintJob &cP2){
	return !(cP1 == cP2);
}

int PrintJob::getNumPages()
{
	return numPages;
}

int PrintJob::getArrivalTime()
{
	return arrivalTime;
}

int PrintJob::getId()
{
	return id;
}
int PrintJob::getRemainingPages()
{
	return remainingPages;
}
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

int PrinterList::updatePrinters(int clock, JobQueueManager queueManager)
{
	int jobsCompleted = 0;
	for(int i=0; i < numPrinters; i++){
		 
		 bool startsOpen = printerArray[i].isOpen();
		 cout<< endl << endl <<" Printer "<<i<< " Starts open? " <<startsOpen;
		 if(!startsOpen){
			 PrintJob print = printerArray[i].getCurrentPrintJob();
			 cout << " JOb ID: "<< (int) print.getId()<< " numPages: "<< print.getNumPages()<<" remainingPages: "<< print.getRemainingPages()<<endl<<endl;
		 }
		if(startsOpen && queueManager.hasJob()){
			printerArray[i].sendJob(queueManager.getJob());
			printerArray[i].decrementPages();
			if (printerArray[i].isOpen()){
				cout<< "Job " <<((int)printerArray[i].getCurrentPrintJob().getId())<<" Finished"<<endl;
				jobsCompleted++;
			}	
		} else if (startsOpen && !queueManager.hasJob()){
			// if it starts open and no job, do nothing
		} else if (!startsOpen){	
		printerArray[i].decrementPages();
		if (printerArray[i].isOpen()){
		
			cout<< "*Job " <<((int)printerArray[i].getCurrentPrintJob().getId())<<" Finished"<<endl;
			jobsCompleted++;
			}
		}
		
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

void Printer::sendJob(PrintJob newJob)
{
	cout<<"Printer "<< getId()<<" is being sent Job  "<<newJob.getId()<<endl;
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




