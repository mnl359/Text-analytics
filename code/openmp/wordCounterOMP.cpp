#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <map>
#include <string>
#include <cctype>
#include <algorithm>
#include <cstdio>
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

//Table that saves each word with the id of the notice it belongs to and the frequency that has in 
//the content of the new
map<string, map<string, freq> > wordsFrequency; 

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

int wordsCounting(const string path){
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

            map<string, Counter> counter = wordCount(content.c_str()); // Returns how many times word is in content
            map<string, Counter, less<string> >::iterator i;

            for(i = counter.begin(); i != counter.end(); ++i) {
                struct freq frequency;
                frequency.frequency = (*i).second.cont; //Acceses to the value in the map (Counter) and to its attribute 'cont'
                frequency.title = title;
                #pragma omp critical
                {
                    // Fills the table with word, id of the new and frequency of the word. 
                    wordsFrequency[(*i).first][id] = frequency;
                }             
            }
        }               
    }
    file.close();
    return 0;
}

int searchWord() {
    string searchedWord;
    cout << "Enter word to search: (Press - to finish)";
    while(cin >> searchedWord){
        if(searchedWord == "-") break;

        // Points from the beggining to the end of the word, applies to it tolower function
        // and saves it in the space address where searchedWord starts.  
        transform(searchedWord.begin(), searchedWord.end(), searchedWord.begin(), ::tolower);

        map<string, freq> res = wordsFrequency[searchedWord]; // Saves id of the new and how many times the word appears on it.
        map<string, freq, less<string> >::iterator i;
        
        // Stores frequency paired to the struct frequency that it belongs to
        // Saves it from largest to shortest frequency
        multimap<int, freq, greater<int> > mmap;                                      

        int sum = 0;

        for (i = res.begin(); i != res.end(); ++i) {
            (*i).second.id = (*i).first;
            // Inserts values to the multimap structure where stores the frequency of the word
            // and the freq structure it's linked to 
            mmap.insert(pair<int, freq>((*i).second.frequency, (*i).second));
            sum += (*i).second.frequency;

            //cout << (*i).second.frequency << " --- " << (*i).second.id << " --- " << (*i).second.title << endl;
        }

        multimap<int, freq>::iterator mmit;
        int it = 0;
        for(mmit = mmap.begin(); mmit != mmap.end(); ++mmit){
            if(it == 10) break;
            cout << (*mmit).first << " ---- " << (*mmit).second.id << " --- " << (*mmit).second.title << endl;
            ++it;
        }

        cout << "Input word " << searchedWord << " is " << sum << " times in the whole dataset." << endl;
        cout << "Enter next word to search: (Press - to finish) ";
    }
    return 0;
}

int main(int argc, char* argv[]) {
    string files[3];
    files[0] = "../serial/file1.csv";
    files[1] = "../serial/file2.csv";
    files[2] = "../serial/file3.csv";

    unsigned time0, time1;
    int i = 0;
    time0 = clock();

    while(i < 3){
        wordsCounting(files[i]);
        ++i;
    }

    time1 = clock();
    double time = (double(time1 - time0)/CLOCKS_PER_SEC);
    cout << "Inverted index execution: " << time << endl;
    searchWord();
    return 0;
}