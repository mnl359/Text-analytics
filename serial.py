#!/usr/bin/python3

from csv import reader, field_size_limit
from sys import maxsize

field_size_limit(maxsize)

word = "house"

def find_word(news, word):
    if news[0].find(word) >= 0 or news[1].find(word) >= 0:
        count_tit = 0
        count_des = 0
        for title, title_word in enumerate(news[0].split(' ')):
            if title_word == word:
                count_tit += 1
        for des, des_word in enumerate(news[1].split(' ')):
            if des_word == word:
                count_des += 1
        return count_tit + count_des
    return 0

with open('../all-articles/articles1.csv') as csv_file:
    csv_reader = reader(csv_file, delimiter=',')
    line = 0
    word = word.lower()
    for row in csv_reader:
        title = row[2].lower()
        new = row[9].lower()
        count = find_word((title, new), word)
        if count > 0:
            print(row[1], count, row[2])
