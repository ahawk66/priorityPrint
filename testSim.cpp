
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


    queue<printJobType> queueA; // # < 10
    queue<printJobType> queueB; // # >= 10 and < 20
    queue<printJobType> queueC; // # >= 20


    int randPageNum;
    for (int i = 0; i < numberOfPrintJobs; ++i)
    {
        randPageNum = rand() % maxNumOfPages +1;
        if (randPageNum < 10)
        {
            queueA.push(randPageNum);
            cout << "Queue A Added [" << randPageNum << "]" << endl;
        }
            
        else if (randPageNum >= 10 && randPageNum < 20)
        {
            queueB.push(randPageNum);
            cout << "Queue B Added [" << randPageNum << "]" << endl;
        }
        else
        {
            queueC.push(randPageNum);
            cout << "Queue C Added [" << randPageNum << "]" << endl;
        }
        
    }

    
    
    for(int clock = 1; completedJobs < numberOfPrintJobs; clock++)
    {



    }



}
