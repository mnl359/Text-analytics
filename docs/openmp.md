# Text mining with OpenMP and MPI

## Problem

There is a free text news set, over which is needed to calculate:

Inverted index, where for each word read by keyboard, there has to be a descendant list ordered by frequence of the word in <content> of the new, the most relevant news. There has to be max 10 registers <frec,doc_id,title>.

Data for this project is a kaggle published news set:
https://www.kaggle.com/snapcrack/all-the-news

# Parallel solution

__Usage of OpenMP framework with C++__
  1. Ask for route of files to read.
  2. Open folder where files are going to be read.
  3. Iterate over each file. 
  4. Clean file (erase quotation and stop words).
  5. Open file. 
  6. Define limiter to know what's a word, when there is a end of line and when there is end of the file. 
  7. Access to each new.
  8. Read each column of the file.
  9. Access to content column.
  10. When found any word, save it on table and set its correspondant frequency.
  11. Do steps 5 to 10 till there are no more files. 
  12. Ask for word is going to be searched.
  13. Access to table and take results where the actual word is equal to searched one.
  14. Organize results descending by frequency.
  15. Show first 10 results with word, its frequency, id of the new it's on and title of that new. 


Parallel is reflected this way:
  * Each thread accesses to a different new every time.
  * News are protected by race condition, given various threads shouldn't access to same new.
  * After getting its line, each thread starts to calculate frequency of the word in content, no matter if there are other threads doing the same over another new. 
  * When finished, thread tries to access to table where words and their frequency are being saved. This table is also protected so various threads cannot acces to the same entry.
  * Threads are queued to access to critical sections.