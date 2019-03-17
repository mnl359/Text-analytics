#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <map>
#include <string>
#include <cstring>
#include <cctype>
#include <algorithm>
#include <cstdio>
#include <stdio.h>
#include "mpi.h"
#include <omp.h>
#include <ctime>

using namespace std;

class Counter {
    public:
        int cont;
        Counter(): cont(0) {}
        
        //When created an instance of Counter, if it's called
        //Counter instance; instance++; -> then it increases cont in 1
        void operator++(int) {cont++;} 
};

typedef struct freq {
    int frequency;
    string id;
    string title;
} freq;

ostream &operator<<(ostream &counterValue, Counter &counter) { return counterValue << counter.cont; }

map<string, map<string, freq> > wordsFrequency1;
map<string, map<string, freq> > wordsFrequency2; 
map<string, map<string, freq> > wordsFrequency3; 

map<string, Counter> wordCount(const char* input) {
    map<string, Counter> wordCounter;

    char* saveptr;
    char* token = strtok_r((char *) input, " ", &saveptr); 

    while(token != NULL){
        wordCounter[token]++; // Uses operator ++ definend in Counter class to increase cont variable
        token = strtok_r(NULL, " ", &saveptr);
    }    

    return wordCounter;
}

MPI_Comm comm = MPI_COMM_WORLD;

multimap<int, freq, greater<int> > mmap1;
multimap<int, freq, greater<int> > mmap2;
multimap<int, freq, greater<int> > mmap3;

int rank;
int sum;
int charArrLen = 256;

int wordsCounting(const string path) {
    cout << path << endl;
    
    ifstream file(path);
    
    if(!file.is_open()){
        cout << "ERROR: Couldn't open file" << '\n';
        return -1;
    }

    string header;
    getline(file, header); //Saves what's in file inside header

    #pragma omp parallel
    {
        string index = "";
        string id = "";
        string title = "";
        string content = "";

        while(file.good()) {
            #pragma omp critical
            {
                // Every time it finds a tabulator, saves what found till there on each variable. 
                getline(file, index, '\t');
                getline(file, id, '\t');
                getline(file, title, '\t');
                getline(file, content, '\n');
            }

            map<string, Counter> counter = wordCount(content.c_str());
            map<string, Counter, less<string> >::iterator i;

            for(i = counter.begin(); i != counter.end(); ++i) {
                struct freq frequency;
                frequency.frequency = (*i).second.cont; //Acceses to the value in the map (Counter) and to its attribute 'cont'
                frequency.id = id;
                frequency.title = title;

                if(rank == 0){
                    #pragma omp critical
                    {
                        wordsFrequency1[(*i).first][id] = frequency;
                    }
                } else if(rank == 1){
                    #pragma omp critical
                    {
                        wordsFrequency2[(*i).first][id] = frequency;
                    }
                } else if(rank == 2){
                    #pragma omp critical
                    {
                        wordsFrequency3[(*i).first][id] = frequency;
                    }
                }                       
            }
        }               
    }
    file.close();
    return 0;
}

int searchWord(string searchedWord) {
    // Saves frequencies from the tables where occurrencies are being stored
    // Depending on the rank, it acceses to the table that node created
    // Stores id of the new and a freq structure
    map<string, freq> tables;

    if(rank == 0){
        tables = wordsFrequency1[searchedWord];
    } else if(rank == 1){
        tables = wordsFrequency2[searchedWord];
    } else if(rank == 2) {
        tables = wordsFrequency3[searchedWord];
    }

    map<string, Counter, less<string> >::iterator i;
    int partialSum = 0;

    for(i = tables.begin(); i != tables.end(); ++i){
        if (rank == 0){
            mmap1.insert(pair<int, freq>((*i).second.frequency, (*i).second));
        } else if (rank == 1){
            mmap2.insert(pair<int, freq>((*i).second.frequency, (*i).second));
        } else if (rank == 2){
            mmap3.insert(pair<int, freq>((*i).second.frequency, (*i).second));
        }

        partialSum += (*i).second.frequency;
    }

    sum += partialSum;

    // Each node accesses to these variables only once.
    // Variables are 'protected' by rank variable, which determines which slave is trying to access
    multimap<int, freq>::iterator mmit1;
    int it = 0;
    int frequency = 0;
    string newId = "";
    string newTitle = "";

    if(rank == 1){
        for(mmit1 = mmap2.begin(); mmit1 != mmap2.end(); ++mmit1){
            if(it == 10) break;

            frequency = (*mmit1).first;

            // Copies title and id in char array variables, so they can be sent by MPI_Send function
            // The idea is to avoid sending a custom data structure, so both string fields are converted
            // and sent as char arrays
            newId = (*mmit1).second.id;
            char charId[newId.length() + 1];
            strcpy(charId, newId.c_str());

            newTitle = (*mmit1).second.title;
            char charTitle[newTitle.length() + 1];
            strcpy(charTitle, newTitle.c_str());

            // Sends to master node word's frequency and new's id and title. 
            MPI_Send(&frequency, 1, MPI_INT, 0, 0, comm);
            MPI_Send(&charId, charArrLen, MPI_CHAR, 0, 0, comm);
            MPI_Send(&charTitle, charArrLen, MPI_CHAR, 0, 0, comm);

            ++it;
        }
        MPI_Send(&partialSum, 1, MPI_INT, 0, 0, comm);
    } else if(rank == 2) {
        for(mmit1 = mmap3.begin(); mmit1 != mmap3.end(); ++mmit1){
            if(it == 10) break;

            frequency = (*mmit1).first;

            newId = (*mmit1).second.id;
            char charId[newId.length() + 1];
            strcpy(charId, newId.c_str());

            newTitle = (*mmit1).second.title;
            char charTitle[newTitle.length() + 1];
            strcpy(charTitle, newTitle.c_str());

            MPI_Send(&frequency, 1, MPI_INT, 0, 0, comm);
            MPI_Send(&charId, charArrLen, MPI_CHAR, 0, 0, comm);
            MPI_Send(&charTitle, charArrLen, MPI_CHAR, 0, 0, comm);

            ++it;
        }
        MPI_Send(&partialSum, 1, MPI_INT, 0, 0, comm);
    }    

    return 0;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int size;
    MPI_Comm_rank(comm, &rank); // Rank of processors
    MPI_Comm_size(comm, &size); 

    MPI_Status status;

    char* files[3];
    string file1 = "../serial/file1.csv";
    string file2 = "../serial/file2.csv";
    string file3 = "../serial/file3.csv";
    files[0] = (char*) file1.c_str();
    files[1] = (char*) file2.c_str();
    files[2] = (char*) file3.c_str();

    unsigned time0, time1;
    time0 = clock();

    // Sends a file to each node
    if(rank == 0) wordsCounting(files[0]);
    else if(rank == 1) wordsCounting(files[1]);
    else if(rank == 2) wordsCounting(files[2]);

    time1 = clock();
    double time = (double(time1 - time0)/CLOCKS_PER_SEC);
    cout << "Inverted index execution: " << time << endl;

    bool hasToWork = true;
    int numOfRegisters = 10;
    char rcvWord[charArrLen];

    while(hasToWork){
        if (rank == 0){
            string searchedWord;
            char charSearchedWord[searchedWord.length() + 1];
            cout << "Enter word to search: (Press - to finish)";
            while(cin >> searchedWord){
                if(searchedWord == "-"){
                    hasToWork = false;
                    strcpy(charSearchedWord, searchedWord.c_str());
                    MPI_Send(&charSearchedword, charArrLen, MPI_CHAR, 1, 0, comm);
                    MPI_Send(&charSearchedword, charArrLen, MPI_CHAR, 2, 0, comm);
                    break;
                }

                transform(searchedWord.begin(), searchedWord.end(), searchedWord.begin(), ::tolower); // Turns searched word in lower case
                // MPI accepts char array, not string. It has to be converted.
                strcpy(charSearchedWord, searchedWord.c_str());
                // Sends word to search to each node 
                MPI_Send(&charSearchedword, charArrLen, MPI_CHAR, 1, 0, comm);
                MPI_Send(&charSearchedword, charArrLen, MPI_CHAR, 2, 0, comm);
                searchWord(searchedWord);

                multimap<int, freq, greater<int> > mmap;
                multimap<int, freq>::iterator mmit;
                int it = 0;

                for(mmit = mmap1.begin(); mmit != mmap1.end(); ++mmit){
                    if(it == numOfRegisters) break;
                    mmap.insert(pair<int, freq>((*mmit).first, (*mmit).second));
                    ++it;
                }

                // Stores in multimap top 10 of registers that are in slave 1
                // Receives each component of the structure, then creates a freq struct to 
                // store it and adds it to multimap
                for(int i = 0; i < numOfRegisters; ++i){
                    int frequ;
                    char id[charArrLen];
                    char title[charArrLen];

                    MPI_Recv(&frequ, 1, MPI_INT, 1, 0, comm, &status);
                    MPI_Recv(&id, charArrLen, MPI_CHAR, 1, 0, comm, &status);
                    MPI_Recv(&title, charArrLen, MPI_CHAR, 1, 0, comm, &status);

                    struct freq f;
                    f.frequency = frequ;
                    f.id = id; 
                    f.title = title;

                    mmap.insert(pair<int, freq>(frequ, f);
                }

                int sumSlave1;
                MPI_Recv(&sumSlave1, 1, MPI_INT, 1, 0, comm, &status);
                sum += sumSlave1;

                // Stores in multimap top 10 of registers that are in slave 2
                for(int i = 0; i < numOfRegisters; ++i){
                    int frequ;
                    char id[charArrLen];
                    char title[charArrLen];

                    MPI_Recv(&frequ, 1, MPI_INT, 2, 0, comm, &status);
                    MPI_Recv(&id, charArrLen, MPI_CHAR, 2, 0, comm, &status);
                    MPI_Recv(&title, charArrLen, MPI_CHAR, 2, 0, comm, &status);

                    struct freq f;
                    f.frequency = frequ;
                    f.id = id; 
                    f.title = title;

                    mmap.insert(pair<int, freq>(frequ, f);
                }
                
                int sumSlave2;
                MPI_Recv(&sumSlave2, 1, MPI_INT, 2, 0, comm, &status);
                sum += sumSlave2;

                it = 0;
                for(mmit = mmap.begin(); mmit != mmap.end(); ++mmit){
                    if(it == numOfRegisters) break;
                    cout << (*mmit).second.frequency << " ---- " << (*mmit).second.id << " --- " << (*mmit).second.title << endl;
                    ++mmit;
                }

                cout << "Input word " << searchedWord << " is " << sum << " times in the whole dataset." << endl;
                sum = 0;
                mmap1.clear();
                mmap.clear();
                cout << "Enter next word to search: (Press - to finish) ";
            }
            hasToWork = false;
        } else if(rank == 1) {
            MPI_Rcv(&rcvWord, charArrLen, MPI_CHAR, 0, 0, comm, &status);

            if(strcmp(rcvWord, "-") == 0) break;

            string searchedWord(rcvWord);
            searchWord(searchedWord);

            mmap2.clear();
        } else if(rank == 2) {            
            MPI_Rcv(&rcvWord, charArrLen, MPI_CHAR, 0, 0, comm, &status);

            if(strcmp(rcvWord, "-") == 0) break;

            string searchedWord(rcvWord);
            searchWord(searchedWord);

            mmap3.clear();
        }
    }
    MPI_Finalize();
    return 0;
}