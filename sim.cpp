#include <iostream>
#include <string>
#include <cstdlib>

#include "simulation.h"

using namespace std;

//initializes our job queues
JobQueueManager::JobQueueManager(int size)
{
	numOfQueues = size;
    jobQueueArray = new PrintJobQueue[size];
}
//Gets number of jobs
int JobQueueManager::getNumJobs(){
	int sum = 0;
	for(int i =0; i <numOfQueues; i++){
		sum+= jobQueueArray[i].size();
	}
	return sum;
}

void JobQueueManager::addQueue(int cutoff, int index){
	jobQueueArray[index] = PrintJobQueue(cutoff);
}



// GetJob pops a job from the lowest queue and returns it
PrintJob JobQueueManager::getJob()
{
	PrintJob job = *(new PrintJob(-1,-1,-1));
	bool hasChanged= false;
	for(int i =0; (i<numOfQueues&& !hasChanged); i++){
		if(!jobQueueArray[i].empty()){
			job=jobQueueArray[i].front();
			jobQueueArray[i].pop();
			cout << "QueueManager: Transferring Job "<< job.getId()<<" from queue "<<(i+1)<<". There are  "<< jobQueueArray[i].size()<<" Jobs left in the queue"<<endl;	
		}
	}
	
	return job;
}

//returns if we have a job in any queue
bool JobQueueManager::hasJob()
{
	bool hasJob = true;
	for(int i =0; i <numOfQueues; i++){
		if(jobQueueArray[i].empty()){
			hasJob=false;
		}
	}
		return false;
	
	return hasJob;
}

//Takes a job and adds itt into the correct queue
int JobQueueManager::addJob(int queueIndex,int jobIndex, int timer)
{
	srand(time(NULL));
	double r = ((double) rand() / (RAND_MAX));
	int num = 0;
	if(queueIndex ==0){
	num=rand()%(jobQueueArray[queueIndex].getUpperCutoff()-1 + 1) + 1;
	} else{
		num=rand()%(jobQueueArray[queueIndex].getUpperCutoff()-jobQueueArray[queueIndex-1].getUpperCutoff() + 1) + jobQueueArray[queueIndex-1].getUpperCutoff();
	}
	jobQueueArray[queueIndex].push(PrintJob(num,jobIndex,timer));
	
		return 0;
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

PrinterManager::PrinterManager(int nPrinters, int dRate, double fRate, int rRate)
{
	numPrinters=nPrinters;
	printerArray= new Printer[numPrinters];
	degradeRate=dRate;
	failRate=fRate;
	rechargeRate=rRate;
	
}

//This is the bread and butter.
//Loops through all the printers, states its opening situation. if it doesnt have a job, gets one and starts printing, if it does, print. If its open at the end give it a new job.
int PrinterManager::updatePrinters(int clock, JobQueueManager queueManager)
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
			printerArray[i].doJob(queueManager.getJob());
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
			printerArray[i].doJob(queueManager.getJob());
		}
		
	}
	return jobsCompleted;
}

int PrinterManager::getNumPrinters()
{
	return numPrinters;
}

bool PrinterManager::isPrinterOpen()
{
	bool outcome = false;
		for(int i=0; i < numPrinters; i++){
			if(printerArray[i].isOpen()){
				outcome=true;
			}
		}
	return outcome;
}

Printer PrinterManager::getOpenPrinter()
{
		for(int i=0; i < numPrinters; i++){
			if(printerArray[i].isOpen()){
				return printerArray[i];
			}
	}
}

void PrinterManager::addPrinter(int index, int speed, int costs){
	printerArray[index] = Printer(index,speed,costs,degradeRate);
}

///////////////////////////////////////

Printer::Printer(int i, int speed, int costs, int degradeRate)
{
	id=i;
	printerSpeed =speed;
	cost = costs;
	pagesTillDegrade = degradeRate;
	timeTillRecharge = 0;
	currentJob=*(new PrintJob(-1,-1, -1));
}

//Send job really recieves jobs.
void Printer::doJob(PrintJob newJob)
{
	cout<<"Printer "<< (getId()+1)<<" is starting Job  "<<newJob.getId()<<endl;
	currentJob=newJob;
}

bool Printer::isOpen()
{
	return (currentJob.getRemainingPages() <=0 && timeTillRecharge <= 0);
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

void Printer::setPagesTillDegrade(int pages){
	pagesTillDegrade=pages;
}
int Printer::getPagesTillDegrade(){
	return pagesTillDegrade;
}
int Printer::getTimeTillRecharge(){
	return timeTillRecharge;
}
void Printer::setTimeTillRecharge(int time){
	timeTillRecharge=time;
}

PrintJobQueue::PrintJobQueue(int uCutoff)
{
	upperCutoff=uCutoff;
}
PrintJobQueue::PrintJobQueue()
{
	upperCutoff=0;
}

int PrintJobQueue::getUpperCutoff(){
	return upperCutoff;
}






