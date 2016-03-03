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

int main()
{
    runSim();
    return 0;
}

void runSim()
{
    
    int maxNumPagesPerJob = 50; // P, Maximum # of pages per job
    int numOfPrintJobs = 100; // J, # of Print Jobs
    int numOfPrinters = 3; // S, # of printers
    int numOfPrintJobsPerMinute = 3; // R, Avg # of Jobs per minute
    int numOfQueues = 3; // L, # of priorities
    double degradeRate = 100; // E, pages until maintence
    double failureRate = 0.5; // probability of failure between 0 and 1
    int recoverTime = 2;
    int seed = 0;
    int completedJobs =0;
    
    double costPerPage = 0.1;
    int printerSpeed = 1;
   
    
    
    
    string isCustom = "n";
    cout << "\n\n";
    cout << "=============================================" << endl;
    cout << "===== Welcome to Printer Simulator v2.0 =====" << endl;
    cout << "=============================================" << endl;
    
    cout << "\nWould you like to have custom simulation parameters? (y)es or (n)o" << endl;
    cin >> isCustom;
    
    if (isCustom == "n" || isCustom == "N" || isCustom == "no" || isCustom == "NO")
    {
        maxNumPagesPerJob = 50;
        numOfPrintJobs = 100;
        
        numOfPrintJobsPerMinute = 3;
        numOfQueues = 3;
        
    }
    else if (isCustom == "y" || isCustom == "Y" || isCustom == "yes" || isCustom == "YES")
    {
        cout << "Please Enter your custom parameters below...\n" << endl;
        
        cout<< "Max number of pages per job (Default: 50): ";
        cin >>maxNumPagesPerJob;
        while(maxNumPagesPerJob < 0)
        {
            cout << endl<< "Please enter a number greater than 0: ";
            cin >> maxNumPagesPerJob;
        }
        
        cout<< "Number of print jobs (Default: 100): ";
        cin>>numOfPrintJobs;
        while(numOfPrintJobs < 0)
        {
            cout << endl<< "Please enter a number greater than 0: ";
            cin >> numOfPrintJobs;
        }
        
        cout<<"Number of printers to use (Default: 3): ";
        cin>>numOfPrinters;
        while(numOfPrinters < 0)
        {
            cout << endl<< "Please enter a number greater than 0: ";
            cin >> numOfPrinters;
        }
        
        cout<<"Average jobs per minute (Default: 3): ";
        cin>>numOfPrintJobsPerMinute;
        while(numOfPrintJobsPerMinute < 0)
        {
            cout << endl<< "Please enter a number greater than 0: ";
            cin >> numOfPrintJobsPerMinute;
        }
        
        cout<<"Number of job levels (Default: 3): ";
        cin>>numOfQueues;
        while(numOfQueues < 0)
        {
            cout << endl<< "Please enter a number greater than 0: ";
            cin >> numOfQueues;
        }
    }
    else
    {
        cout << "ERROR: Invalid Option" << endl;
        exit(1);
    }
    
    PrinterManager *printerManager;
    JobQueueManager jobQueueManager(numOfQueues);
    
    cout<<endl<<"Upper page cutoffs for each job level, highest priority first.\n* No Defaults Avalible *\n";
    int aCutoff = 0;
    int lastCutoff = 0;
    for (int i = 1; i <= numOfQueues-1; i++){
        cout<<"\tUpper cutoff of job level "<<(i)<<": ";
        cin >> aCutoff;
        while(aCutoff<=lastCutoff ){
            cout<<"\tUpper cutoff that is larger than the previous cutoff of "<<lastCutoff<<": ";
            cin >> aCutoff;
        }
        lastCutoff = aCutoff;
        
        jobQueueManager.addQueue(aCutoff, i);
    }
    
    jobQueueManager.addQueue(-1, 0); // special case, last queue handles the rest, no upper limit
    
    
    if (isCustom == "n" || isCustom == "N" || isCustom == "no" || isCustom == "NO")
    {
        seed = time(NULL);
        srand(seed);
        degradeRate = 100;
        failureRate = 0.10;
        recoverTime = 2;
        costPerPage = 0.10;
        printerSpeed = 1;
        
    }
    else if (isCustom == "y" || isCustom == "Y" || isCustom == "yes" || isCustom == "YES")
    {
        cout <<"Seed (Default: 0): ";
        cin >> seed;
        if(seed!=0)
            srand(seed);
        
        cout<< "Number of pages printed before maintenance (Default: 100): ";
        cin>>degradeRate;
        while(degradeRate < 0)
        {
            cout << endl << "Please enter a number greater than 0: ";
            cin >> degradeRate;
        }
        
        cout<< "Probability a printer fails, between 0 and 1.0 (Default 0.10): ";
        cin>>failureRate;
        while(failureRate < 0 || failureRate > 1.0)
        {
            cout << endl << "Please enter a number between 0 and 1.0: ";
            cin >> failureRate;
        }
        
        cout<< "Clock ticks to recover from maintenance/failure (Default 2): ";
        cin>>recoverTime;
        while(recoverTime < 0)
        {
            cout << "Please enter a number greater than 0: ";
            cin >> recoverTime;
        }
		
		  printerManager = new PrinterManager(numOfPrinters,degradeRate,failureRate,recoverTime);
        
		cout << "\nWould you like a different cost per page for each printer? (y)es or (n)o" << endl;
		cin >> isCustom;
		int * printerCosts = new int[numOfPrinters];
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
				(*printerManager).addPrinter(i,printerSpeed,printerCosts[i]);
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
				(*printerManager).addPrinter(i,printerSpeed,printerCosts[i]);
			}
		}
        
        
        cout << endl;
        
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
    
    for(int j=0; j< maxNumOfJobs;j++){
        cout<<"Poisson "<<j<<": "<<poissonArray[j]<<endl;
    }
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
    for( clock = 1; (completedJobs<numOfPrintJobs); clock++){
		cout<<  endl << "--- Clock: "<< clock<< " -- Completed Jobs: "<<completedJobs<<" ----- # of Jobs in JobQueues: "<< jobQueueManager.getNumJobs()<<" --------" <<endl;
		
        if( runningJobs< numOfPrintJobs ){
            
            randomNumber = ((double)rand() / (RAND_MAX));
            int numOfJobs= checkThreshold(randomNumber,poissonArray,maxNumOfJobs);
            cout<<" Number of new jobs this clock-tick: "<<numOfJobs<<endl;
            
            for(int l=0;l<numOfJobs;l++){
                randomNumber = ((double)rand() / (RAND_MAX));
                
                int queueIndex= checkThreshold(randomNumber,cutOffArray,numOfQueues);
                jobQueueManager.addJob(queueIndex,runningJobs,clock);
                runningJobs++;
            }
        }
		
        completedJobs+= (*printerManager).updatePrinters(clock,jobQueueManager);
        
    }
	clock--;
	cout<<endl<< "------------------------SUMMARY--------------------------"<<endl;
	cout<<"Seed: "<< seed<<endl;
	cout<<"The simulation took " << (clock)<< " minutes to complete all jobs"<<endl;
	cout<<"The average print-time was "<< ((double) clock/completedJobs)<<endl<<endl;
	cout<<"The total cost was "<<(*printerManager).getTotalCost()<<" dollars.";
	(*printerManager).printerSummary(clock);
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

int factorial(int n) 
{
    if (n == 0)
        return 1;
    return n * factorial(n - 1);
}
