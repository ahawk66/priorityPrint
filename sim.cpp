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
	
	cout<< "Creating jobqueue (index,cutoff)"<<"("<<index<<", "<<cutoff<<")"<<endl;
	
	PrintJobQueue *q = new PrintJobQueue(cutoff);
	jobQueueArray[index] = (*q);
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
			cout << "QueueManager: Transferring Job "<< job.getId()<<" from queue "<<(i)<<". There are  "<< jobQueueArray[i].size()<<" Jobs left in this queue"<<endl;	
			hasChanged=true;
		}

	}
	return job;
}

//returns if we have a job in any queue
bool JobQueueManager::hasJob()
{
	return (getNumJobs()==0);
}

//Takes a job and adds itt into the correct queue
int JobQueueManager::addJob(int queueIndex,int jobIndex, int timer)
{
	
	double r = ((double) rand() / (RAND_MAX));
	int num = 0;
	if(queueIndex ==0){
	num=rand()%(jobQueueArray[queueIndex].getUpperCutoff()-1 + 1) + 1;
	} else{
		num=rand()%(jobQueueArray[queueIndex].getUpperCutoff()-jobQueueArray[queueIndex-1].getUpperCutoff() + 1) + jobQueueArray[queueIndex-1].getUpperCutoff();
	}
	PrintJob * newJob = new PrintJob(jobIndex,num,timer);
	jobQueueArray[queueIndex].increaseTotalPageCount(num);
	jobQueueArray[queueIndex].increaseTotalJobCount(1);
	cout<< "Creating job (index,pages,arrival,queueIndex): "<<"("<<jobIndex<<", "<<num<<", "<<timer<<", "<<queueIndex<<")"<<endl;
	jobQueueArray[queueIndex].push(*newJob);
	
		return 0;
}
int JobQueueManager::getCutoff(int index){
	return jobQueueArray[index].getUpperCutoff();
}
void JobQueueManager::stats(){
	cout<< "-------JobQueue Summary--------"<<endl;
	for(int i =0; i < numOfQueues; i++){
		cout<< "JobQueue "<< (i)<< " handled "<< jobQueueArray[i].getTotalJobCount()<< " jobs and "<<jobQueueArray[i].getTotalPageCount()<<" pages"<<endl;
	}
	cout<<endl;
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
	//cout<< " ds: "<<remainingPages << endl;
	remainingPages= remainingPages - pagesToPrint;
	//cout<< " ds2: "<<remainingPages << endl;
}

////////////////////////////////////////

PrinterManager::PrinterManager(int nPrinters)
{
	numPrinters=nPrinters;
	printerArray= new Printer[numPrinters];	
}

//This is the bread and butter.
//Loops through all the printers, states its opening situation. if it doesnt have a job, gets one and starts printing, if it does, print. If its open at the end give it a new job.
int PrinterManager::updatePrinters(int clock, JobQueueManager queueManager)
{

	int jobsCompleted = 0;
	for(int i=0; i < numPrinters; i++){
		 
		 bool startsOpen = printerArray[i].isOpen();
		 //Print opening situation
		 if((printerArray[i].getTimeTillRecharge()-1) >0){
					int rechargeTime = printerArray[i].getTimeTillRecharge();
					printerArray[i].setTimeTillRecharge(rechargeTime-1);
					cout<<"Printer "<< (printerArray[i].getId()+1)<< " is still down, it will be back up in "<<printerArray[i].getTimeTillRecharge()<<" clock ticks"<<endl;
					
				} else{
					if((printerArray[i].getTimeTillRecharge()-1) ==0){
					int rechargeTime = printerArray[i].getTimeTillRecharge();
					printerArray[i].setTimeTillRecharge(rechargeTime-1);
					cout<<"Printer "<< (printerArray[i].getId()+1)<< " just came back online! "<<endl;
					}
					
		 if(!startsOpen){		 
			 PrintJob print = printerArray[i].getCurrentPrintJob();
			cout << "Printer "<<(i+1)<< " has Job "<< (int) print.getId()<< " Start # of Pages:"<< print.getNumPages()<<" Pages Remaining: "<< print.getRemainingPages()<<"" <<endl;
		//	cout <<"Job details: ID: " <<print.getId() <<" atime: "<<print.getArrivalTime()<<endl;
			printerArray[i].decrementPages();
			if (printerArray[i].isOpen()){	
				cout<< "Printer "<<(i+1)<< " finished Job " <<((int)printerArray[i].getCurrentPrintJob().getId())<<endl;
				jobsCompleted++;
			}
		 } else {
				if(printerArray[i].getPagesTillDegrade()<=0){
			 	
					printerArray[i].setPagesTillDegrade(printerArray[i].getDegradeRate());
					printerArray[i].setTimeTillRecharge(printerArray[i].getRechargeRate());
					cout<<"Printer "<< (printerArray[i].getId()+1)<< " has gone down for maintenance and will be back up in "<<printerArray[i].getTimeTillRecharge()<<" clock ticks"<<endl;			
				}else{
			 cout<<"Printer "<<(i+1)<< " is empty! "<<endl;
			 //If you dont have a job and we have a job to give
		if(queueManager.getNumJobs()!=0){
				PrintJob print =queueManager.getJob();
			printerArray[i].doJob(print);
		// printerArray[i].getCurrentPrintJob();
			//cout<<"PrintJob(index,numPages,remainingPages): "<<"("<<print.getId()<<", "<<print.getNumPages()<<", "<<print.getRemainingPages()<<")"<<endl;
			printerArray[i].decrementPages();
			///cout<<"PrintJob(index,numPages,remainingPages): "<<"("<<print.getId()<<", "<<print.getNumPages()<<", "<<print.getRemainingPages()<<")"<<endl;

			if (printerArray[i].isOpen()){
			//cout<<"checcck3";
				cout<< "Printer "<<(i+1)<< " finished Job " <<((int)printerArray[i].getCurrentPrintJob().getId())<<endl;
				jobsCompleted++;
			}	
		}
				}
		}}
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

void PrinterManager::addPrinter(int index, int speed, double costs, int dRate, double fRate, int rRate){
	
	///cout<<"Generating Printer(index, speed, cost, degradeRate, failRate,rechargeRate)"<<"("
	//<<index<<", "<<speed<<", "<<costs<<", "<<dRate<<", "<<fRate<<", "<<rRate<<")"<<endl;
	Printer * newPrinter = new Printer(index,speed,costs,dRate, fRate,rRate);
	printerArray[index] = (*newPrinter);
	
}

void PrinterManager::printerSummary(int time){
	cout<< "-------Printer Summary--------"<<endl;
	for(int i =0; i < numPrinters; i++){
		Printer printer = printerArray[i];
		cout<<"Printer (index, speed, cost, maintenanceRate, rechargeRate)"<<"("
	<<(printer.getId()+1)<<", "<<printer.getPrinterSpeed()<<", "<<printer.getCost()<<", "<<printer.getDegradeRate()<<", "<<printer.getRechargeRate()<<")"<<endl;
		cout<< "Printer "<< (i+1)<< " handled "<< printerArray[i].getTotalJobs()<< " jobs and "<<printerArray[i].getTotalPages()<<" pages, to a tune of " <<printerArray[i].getTotalCost()<<" Dollars. Total time printing: "<< printerArray[i].getTotalTimeSpent()<< " Which gives this printer "<<(((double )printerArray[i].getTotalTimeSpent()/time)*100)<<"% utilization!"<<endl<<endl;
	}
}

double PrinterManager::getTotalCost(){
	double sum = 0;
	for(int i=0; i < numPrinters; i++){
		sum += printerArray[i].getTotalCost();
	}
	return sum;
}
///////////////////////////////////////

Printer::Printer(int i, int speed, double costs, int degradeRat, double failRat, double rechargeRat)
{
	id=i;
	printerSpeed =speed;
	cost = costs;
	pagesTillDegrade = degradeRat;
	timeTillRecharge = 0;
	currentJob=*(new PrintJob(-1,-1, -1));
	totalCost =0;
	totalTimeSpent=0;
	totalPages=0;
	totalJobs=0;
	rechargeRate=rechargeRat;
	failRate=failRat;
	degradeRate=degradeRat;
}

int Printer::getDegradeRate(){
	return degradeRate;
}
int Printer::getRechargeRate(){
	return rechargeRate;
}
double Printer::getCost(){
	return cost;
}

//Send job really recieves jobs.
void Printer::doJob(PrintJob newJob)
{

	if (newJob.getId()!=-1 && timeTillRecharge<=0){
	cout<<"Printer "<< (getId()+1)<<" is starting Job  "<<newJob.getId()<<endl;
	currentJob=newJob;
	totalJobs++;
	} else {
		//cout<<"DoJOb: Rejecting job(pages,index,arrival,queueIndex): "<<"("<<newJob.getNumPages()<<", "<<newJob.getId()<<")"<<endl;
	}
	
}

bool Printer::isOpen()
{
	return (currentJob.getRemainingPages() <=0 && timeTillRecharge <= 1);
}


void Printer::decrementPages()
{
		double r = ((double) rand() / (RAND_MAX));
		if(r<failRate){
			timeTillRecharge=rechargeRate;
			cout<<"The printer has failed! It will be back online soon!"<<endl;
		} else {
		//	cout<<"r: "<<r<<" failrate: "<<failRate<<endl;
	//cout <<"npp1: "<<"PrintJob(index,numPages,remainingPages): "<<"("<<currentJob.getId()<<", "<<currentJob.getNumPages()<<", "<<currentJob.getRemainingPages()<<")"<<endl;
	currentJob.decrementPages(printerSpeed);
	totalPages+=printerSpeed;
	totalTimeSpent++;
	totalCost+=printerSpeed*cost;
	pagesTillDegrade-=printerSpeed;
	//cout <<"npp2: "<<"PrintJob(index,numPages,remainingPages): "<<"("<<currentJob.getId()<<", "<<currentJob.getNumPages()<<", "<<currentJob.getRemainingPages()<<")"<<endl;
		}
}

int Printer::getId()
{
	return id;
}

int Printer::getTotalPages(){
	return totalPages;
}

int Printer::getTotalTimeSpent(){
	return totalTimeSpent;
}

double Printer::getTotalCost(){
	return totalCost;
}

int Printer::getTotalJobs(){
	return totalJobs;
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
	totalJobCount=0;
	totalPageCount=0;
}



int PrintJobQueue::getUpperCutoff(){
	return upperCutoff;
}

int PrintJobQueue::getTotalJobCount(){
	return totalJobCount;
}
int PrintJobQueue::getTotalPageCount(){
		return totalPageCount;
}
void PrintJobQueue::increaseTotalJobCount(int count){
	totalJobCount+=count;
}
void PrintJobQueue::increaseTotalPageCount(int count){
	totalPageCount+=count;
}
