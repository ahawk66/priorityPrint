#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#include <queue>
#include <fstream>
#include "simulation.h"
using namespace std;

void runSim();
int factorial(int n);
int poisson(double k);
int checkThreshold(double rand, double thresholds[], int size);

//calls runSim
int main()
{
	runSim();
	return 0;
}

//Runs the simulation fully, takes in all the inputs and calculates probabilities

void runSim()
{

	int maxNumPagesPerJob = 50; // P, Maximum # of pages per job
	int numOfPrintJobs = 100; // J, # of Print Jobs
	int numOfPrinters = 3; // S, # of printers
	int numOfPrintJobsPerMinute = 5; // R, Avg # of Jobs per minute
	int numOfQueues = 3; // L, # of priorities
	double degradeRate = 100; // E, pages until maintence
	double failureRate = 0.5; //F probability of failure between 0 and 1
	int recoverTime = 2;//M maintenance time
	int seed = 0;
	int completedJobs =0;

	//start user input
	cout << "\n\n";
	cout << "=============================================" << endl;
	cout << "===== Welcome to Printer Simulator v2.0 =====" << endl;
	cout << "=============================================" << endl;
	cout<<"Please enter in details to get us started! If you would like to use the default option for any question, just enter 0!"<<endl;

	cout<< "Please enter the maximum number of pages per job (Default 50): ";
	cin >>maxNumPagesPerJob;
	while(maxNumPagesPerJob < 0){
		cout << endl<< "Please enter a number above 0, or 0 for default: ";
		cin >> maxNumPagesPerJob;
	}
	if(maxNumPagesPerJob == 0){
		maxNumPagesPerJob = 50;
	}

	cout<<endl<< "Please enter the number of print jobs (Default 100): ";
	cin>>numOfPrintJobs;
	while(numOfPrintJobs < 0){
		cout << endl<< "Please enter a number above 0, or 0 for default: ";
		cin >> numOfPrintJobs;
	}
	if(numOfPrintJobs == 0){
		numOfPrintJobs = 100;
	}

	cout<<endl<<"Please enter the number of printers to use (Default 3): ";
	cin>>numOfPrinters;
	while(numOfPrinters < 0){
		cout << endl<< "Please enter a number above 0, or 0 for default: ";
		cin >> numOfPrinters;
	}
	if(numOfPrinters == 0){
		numOfPrinters = 3;
	}

	cout<<endl<<"Please enter the avg # of jobs per minute (Default 3): ";
	cin>>numOfPrintJobsPerMinute;
	while(numOfPrintJobsPerMinute < 0){
		cout << endl<< "Please enter a number above 0, or 0 for default: ";
		cin >> numOfPrintJobsPerMinute;
	}
	if(numOfPrintJobsPerMinute == 0){
		numOfPrintJobsPerMinute = 3;
	}

	cout<<endl<<"Please enter the number of job levels (Default 3): ";
	cin>>numOfQueues;
	while(numOfQueues < 0){
		cout << endl<< "Please enter a number above 0, or 0 for default: ";
		cin >> numOfQueues;
	}
	if(numOfQueues == 0){
		numOfQueues = 3;
	}

	JobQueueManager jobQueueManager (numOfQueues);

	cout<<endl<<"Please enter in the value of the upper page cutoff for each job level, starting with the highest priority . No defaults here, sorry! ";
	int aCutoff = 0;
	int lastCutoff = 0;
	for (int i = 0; i < numOfQueues-1; i++){
		cout<<endl<<" Please enter the upper cutoff of job level "<<(i)<<": ";
		cin >> aCutoff;
		while(aCutoff<=lastCutoff ){
			cout<<endl<<" Please enter an upper cutoff that is larger than the previous cutoff of "<<lastCutoff<<": ";
			cin >> aCutoff;
		}
		lastCutoff = aCutoff;

		jobQueueManager.addQueue(aCutoff, i);
	}

	jobQueueManager.addQueue(maxNumPagesPerJob, numOfQueues-1); // special case, last queue handles the rest, no upper limit

	cout << endl<<"Please enter a seed (Default 0): ";
	cin >> seed;
	if(seed!=0){
		srand(seed);
	} else {
		seed = time(NULL);
		srand(seed);
	}
	cout << endl;

	cout << endl<< "Please enter the number of pages a printer can print before needing maintenance (Default 100): ";
	cin>>degradeRate;
	while(degradeRate < 0){
		cout << endl<< "Please enter a number above 0, or 0 for default: ";
		cin >> degradeRate;
	}
	if(degradeRate == 0){
		degradeRate = 100;
	}

	cout<< endl<< "Please enter the probability, between 0 and 1.0, a printer fails each clock tick (Default 0.10): ";
	cin>>failureRate;
	while(failureRate < 0){
		cout << endl<< "Please enter a number above 0, or 0 for default: ";
		cin >> failureRate;
	}
	if(failureRate == 0){
		failureRate = 0.10;
	}

	cout<< endl << "Please enter the number of clock ticks it takes for a printer to recover from maintenance or a failure (Default 2): ";
	cin>>recoverTime;
	while(recoverTime < 0){
		cout << endl<< "Please enter a number above 0, or 0 for default: ";
		cin >> recoverTime;
	}
	if(recoverTime == 0){
		recoverTime = 2;
	}

	PrinterManager * printerManager = new PrinterManager(numOfPrinters);

	string isCustom = "n";

	double costPerPage = 0.1;
	int printerSpeed = 1;
	cout << "\nWould you like a different cost per page for each printer? (y)es or (n)o" << endl;
	cin >> isCustom;
	double * printerCosts = new double[numOfPrinters];
	if (isCustom == "n" || isCustom == "N" || isCustom == "no" || isCustom == "NO"){
		cout<< "Please enter a Cost per page (Default 0.10): ";
		cin>>costPerPage;
		while(costPerPage < 0)
		{
			cout << "Please enter a number greater than 0: ";
			cin >> costPerPage;
		}

		for(int i =0; i <numOfPrinters; i++){
			printerCosts[i]  = costPerPage;
		}

	} else {
		for(int i =0; i <numOfPrinters; i++){
			cout<< "Please enter a Cost per page (Default 0.10): ";
			cin>>costPerPage;
			while(costPerPage < 0)
			{
				cout << "Please enter a number greater than 0: ";
				cin >> costPerPage;
			}
			printerCosts[i] =costPerPage;
		}
	}


	cout << "\nWould you like a different speed for each printer? (y)es or (n)o" << endl;
	cin >> isCustom;
	if (isCustom == "n" || isCustom == "N" || isCustom == "no" || isCustom == "NO"){
		cout<< "Please enter a Printer Speed (Default 1): ";
		cin>>printerSpeed;
		while(printerSpeed < 0)
		{
			cout << "Please enter a number greater than 0: ";
			cin >> printerSpeed;
		}
		for(int i =0; i <numOfPrinters; i++){
			(*printerManager).addPrinter(i,printerSpeed,printerCosts[i],degradeRate,failureRate,recoverTime);
		}
	} else {
		for(int i =0; i <numOfPrinters; i++){
			cout<< "Please enter a Printer Speed (Default 1): ";
			cin>>printerSpeed;
			while(printerSpeed < 0)
			{
				cout << "Please enter a number greater than 0: ";
				cin >> printerSpeed;
			}
			(*printerManager).addPrinter(i,printerSpeed,printerCosts[i],degradeRate,failureRate,recoverTime);
		}
	}



	double total = 0.0;
	double temp = 0.0;
	double t;
	int maxNumOfJobs = poisson(numOfPrintJobsPerMinute)+1;
	cout<<endl<<maxNumOfJobs<<endl;
	double * poissonArray = new double[maxNumOfJobs];

	for(int i=0; i < maxNumOfJobs-1; i++ ){
		t = pow((double)numOfPrintJobsPerMinute,(double)i);

		double q = (exp(numOfPrintJobsPerMinute)*factorial(i));
		double o = (1.0)/q;
		double poisson = t*o;
		total+=poisson;
		poissonArray[i] = total;
	}
	poissonArray[maxNumOfJobs-1]= 1.0;
    
    //Using poisson formula to generate the number of jobs that arrive
    
	for(int j=0; j< maxNumOfJobs;j++){
		cout<<"Poisson "<<j<<": "<<poissonArray[j]<<endl;
	}
    //dynamically allocating new threshold arrays
	double *cutOffArray = new double[numOfQueues];
	double cutOffTotal =0.0;
	for (int k=0; k< numOfQueues-1; k++){
		cutOffTotal+=  1.0/(double)(2+k+1);
		cutOffArray[k] =cutOffTotal;
	}
	cutOffArray[numOfQueues-1]= 1.0;
	for(int j=0; j< numOfQueues;j++){
		cout<<"CutOff "<<j<<": "<<cutOffArray[j]<<endl;
	}
	int clock;
	double randomNumber;
	int runningJobs=0;
    //loop that checks if there are less completed jobs than number of print jobs, it will generate new jobs
	for( clock = 1; (completedJobs<numOfPrintJobs); clock++){
		cout<<  endl << "--- Clock: "<< clock<< " -- Completed Jobs: "<<completedJobs<<" ----- # of Jobs in JobQueues: "<< jobQueueManager.getNumJobs()<<"-----" <<endl;
		if( runningJobs< numOfPrintJobs ){

			randomNumber = ((double)rand() / (RAND_MAX));
			int numOfJobs= checkThreshold(randomNumber,poissonArray,maxNumOfJobs);
			cout<<" Number of new jobs this round: "<<numOfJobs<<endl;

			for(int l=0;l<numOfJobs;l++){
				randomNumber = ((double)rand() / (RAND_MAX));

				int queueIndex= checkThreshold(randomNumber,cutOffArray,numOfQueues);
				jobQueueManager.addJob(queueIndex,runningJobs,clock);
				runningJobs++;
			}
			cout<<endl;
			//	cout<<  endl << "--- Clock: "<< clock<< " -- Completed Jobs: "<<completedJobs<<" ----- # of Jobs in JobQueues: "<< jobQueueManager.getNumJobs()<<" --------" <<endl;
		}
		completedJobs+= (*printerManager).updatePrinters(clock,jobQueueManager);

	}
    //Prints out entire summary after all jobs have been completed
	clock--;
	cout<<endl<< "------------------------SUMMARY--------------------------"<<endl;
	cout<<"--------Input settings------"<<endl;
	cout<<"Maximum number of pages per job: "<<maxNumPagesPerJob<<endl;
	cout<<"Number of print jobs to complete: "<<maxNumOfJobs<<endl;
	cout<<"Number of printers to use: "<<numOfPrinters<<endl;
	cout<<"Average number of print jobs a minute: "<<numOfPrintJobsPerMinute<<endl;
	cout<<"Queue Levels:\n";
	for(int i=0;i<numOfQueues;i++){
		cout<<"Queue "<<i<<", Upper Cutoff: "<<jobQueueManager.getCutoff(i)<<endl;
	}
	cout<<"Number of pages to print before maintenance: "<<degradeRate<<endl;
	cout<<"Probability the printer fails: "<<failureRate<<endl;
    cout<<"Printer recovery time: "<<recoverTime << endl;
	cout<<"Seed: "<< seed<<endl;
	(*printerManager).printerSummary(clock);
	jobQueueManager.stats();
	cout<<"The simulation took " << (clock)<< " minutes to complete all jobs"<<endl;
	cout<<"The average print-time was "<< ((double) clock/completedJobs)<<endl<<endl;
	cout<<"The total cost was "<<(*printerManager).getTotalCost()<<" dollars." << endl;
    

}
int checkThreshold(double rand, double thresholds[], int sizer){
	int num =-1;

	if (rand<thresholds[0]){
		return 0;
	}
	for(int i =1; i< sizer-1; i++){

		if(rand > thresholds[i-1] && rand <thresholds[i] ){
			num= i;
		} else{
			//	cout<< "rand: "<<rand<<" , isnt between "<<thresholds[i-1]<<" or "<<thresholds[i]<<endl;
		}
	}
	if (num==-1){
		num=sizer-1;
	}
	return num;
}

//Function that calculates the poisson value
int poisson(double k){
	double length;
	double total= 0.00;
	for(length=0; total<0.95; length++ ){
		double t = pow((double)k,length);
		double o = (1.0)/(exp(k)*factorial(length));
		double poisson = t*o;
		total+=poisson;
		//	cout<<" total: " <<total<<endl;
		//printf("%lf and %lf and %lf and %lf and %lf \n ",t,o,poisson);
	}
	return (int) length-1;
}

//generates factorial number
int factorial(int n)
{
	if (n == 0)
		return 1;
	return n * factorial(n - 1);
}
