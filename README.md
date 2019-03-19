# Text Analytics

Programa que busca y cuenta el número de apariciones de una palabra en un dataset de noticias.

## Theory behind code

* [OpenMP](https://github.com/mnl359/Text-analytics/blob/master/docs/openmp.md)
* [PCAM](https://github.com/mnl359/Text-analytics/blob/master/docs/pcam.md)

## Requirements

* Python 3+
  * Python modules:
    - pandas
      ```
      pip install pandas
      ```
    - nltk
      ```
      pip install nltk
      ```
* Various nodes:
  * This project is meant to be a work for TET subject in EAFIT University, for Systems Engineering students. We are given access to a cluster and we have two slave nodes and a master node which we are using in the code. If you have you're own nodes, you can set their ip adresses in file mpi/hosts_mpi. 

## Usage

First, you have to know for solution there is a batch part, whose idea is to create a table with each word's occurrency in the content of a new. To do so, a table is created where each word is paired to the id of the new it was in, how many times it was found (frequency) and the title of the new. 

After the table is completely created, you have to enter the word you are looking for and the algorithm accesses to the mentioned table to search for that word. Saves registers where it was found and organizes those registers descending to finally show top 10 of registers (biggest frequency). It shows frequency, id of the new and its title. This is the online part of code.

__Conventions:__ nThreads: number of threads you want the code runs with.

To use this program, you have to follow the next steps:

* **To clean files**

    __Note:__ Make sure you have files in /opt/datasets so code can access to clean them. Otherwise, go to  code and change path in cleaning.py, line 42.

    ```
    $ cd serial/
    $ python cleaning.py
    ```

* **To execute serial algorithm**

    __Note:__ After cleaning, files are saved automatically in this folder. If you want to save it somewhere else, you'll have to change all .cpp files to indicate where they are stored. 

    ```
    $ cd serial/
    $ g++ serial.cpp -std=c++11 -o serial
    $ time ./serial
    ```

* **To execute OpenMP algorithm**

    ```
    $ cd openmp/
    $ g++ wordCounterOMP.cpp -std=c++11 -o omp -fopenmp
    $ export OMP_NUM_THREADS=<nThreads>
    $ export OMP_DISPLAY_ENV='true'
    $ time ./omp
    ```
    
    __Note:__ an inverted time index is going to be displayed. Divide that time on the number of threads you set and you'll get the real time it took to create batch part (table with the occurrency of each word).

* **To execute MPI with OpenMP algorithm**

    __Note:__ np refers to number of nodes the algorithm is going to run in. It's specifically 3 because we count on 2 slaves nodes and 1 master node, as explained before. 

    ```
    $ cd mpi
    $ mpi++ mpi.cpp -o mpi -std=c++11 -fopenmp
    $ export OMP_NUM_THREADS=<nThreads>
    $ export OMP_DISPLAY_ENV='true'
    $ mpirun -f hosts_mpi -np 3 ./mpi
    ```

## Performance

__Serial__

It took 203.18s (3:23:18m) to create inverted index in serial algorithm.

This algorithm is obviously slow, given the fact creating the initial table is O(n), with n equals to number of words in all culumn content of the whole dataset. 

Accessing to the created table in the online part is log(n), which gives a little efficiency to it.

__OpenMP__

Implementing threads helped in execution. However, as explained in OpenMP design, there were two problems with threads: they couldn't either access to the same new nor access to the same table entry to set its value. What threads could do was, given a line (new), they could start counting each word, and different lines could be given to various threads at the same time. 

As shown in the following charts, we can conclude that using 3 threads is the most optimum. The more threads, the worst is going to become performance.

![OpenMP](https://github.com/mnl359/Text-analytics/blob/master/images/OMPEfficiency.png)
![OMPTable](https://github.com/mnl359/Text-analytics/blob/master/images/SpeedEfficiencyOMP.png)

__MPI with OpenMP__

Doing a distributed serial algorithm improves efficiency in a very palpable way. Each node took afile, created the table with all words existing in each new and after master sends the word to search, they made their table with top 10 frequencies. After that, master collects those tables and organizes them to show the final table that evidences top 10 in the whole dataset. 

The following table shows that, even though difference with 3 threas is really small, it keeps being more omptimum to use two threads per node. 

![MPI](https://github.com/mnl359/Text-analytics/blob/master/images/MPIEfficiency.png)
![MPITable](https://github.com/mnl359/Text-analytics/blob/master/images/SpeedEfficiencyMPI.png)

## Team members

* Wendy María Arango Chavarría <warango4@eafit.edu.co>
* Manuela Carrasco Pinzón <mcarras1@eafit.edu.co>