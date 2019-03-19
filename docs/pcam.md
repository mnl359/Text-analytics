### Distributed parallel solution

## Problem

There is a free text news set, over which is needed to calculate:

Inverted index, where for each word read by keyboard, there has to be a descendant list ordered by frequence of the word in <content> of the new, the most relevant news. There has to be max 10 registers <frec,doc_id,title>.

Data for this project is a kaggle published news set:
https://www.kaggle.com/snapcrack/all-the-news

__Usage of OpenMP and MPI frameworks with C++__

  1. Ask for route of files to read. 
  2. Open folder where files are going to be read.
  3. Clean files.
  4. Take each file and send it to next available node. 
  5. Each node has to:
      1. Open file.
      2. Define limiter to know what's a word, when there is a end of line and when there is end of the file.
      3. Access to each new.
      4. Read each column of the file.
      5. Access to content column.
      6. When found any word, save it on table and set its corresponding frequency.
  6. Ask for word is going to be searched.
  7. Send word to each node. 
      1. Access to table.
      2. Send top 10 of registers ordered by largest frequency of word in content. 
  8. Receive and compile the work done by slaves in new table. 
  9. Organize new table descending by frequency.
  10. Show first 10 results with word, its frequency, id of the new it's on and title of that new.

## General idea of the solution:
![Solution](https://github.com/mnl359/Text-analytics/blob/master/images/MPI_PCAM.png)

## PCAM Design

* __P - Partitioning:__
![Partitioning](https://github.com/mnl359/Text-analytics/blob/master/images/partitioning.png)

* __C - Communication:__
![Communication](https://github.com/mnl359/Text-analytics/blob/master/images/communication.png)

* __A - Agglomeration:__
![Agglomeration](https://github.com/mnl359/Text-analytics/blob/master/images/agglomeration.png)

* __M - Mapping:__
![Mapping](https://github.com/mnl359/Text-analytics/blob/master/images/mapping.png)

  _Synchronus communication:_ [T14] Because master node has to make sure word got to slaves so they can look for it on their tables. 

  _Asynchronus communication:_ [T7] Slaves send their response with top 10 frequencies to master but they don't have to get an answer back.