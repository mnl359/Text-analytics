#!/usr/bin/python3

from csv import reader

word = "House"
with open('/opt/datasets/articles1.csv') as csv_file:
    csv_reader = reader(csv_file, delimiter=',')
    line = 0
    for row in csv_reader:
        if line == 0:
            line += 1
        else:
            if find_word((row[2], row[9]), word):
                print(row[1], row[2])

def find_word(news, word):
    if news[0].find(word) or news[1].find(word):
        return 0
    else:
        return -1
