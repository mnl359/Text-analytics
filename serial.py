#!/usr/bin/python3

from csv import DictReader, field_size_limit
from pandas import read_csv
from sys import maxsize
from os import listdir

field_size_limit(maxsize)

def find_word(news, word):
    count_tit = 0
    count_des = 0
    for title, title_word in enumerate(news[0].split(' ')):
        if title_word == word:
            count_tit += 1
    for des, des_word in enumerate(news[1].split(' ')):
        if des_word == word:
            count_des += 1
    return count_tit + count_des

directory = input("Ingrese la ruta de los directorios: ")
files = listdir(directory)
articles = []
word = input("Ingrese la palabra a buscar: ")

for file in files:
    filename, extension = file.split(".")
    if extension == "csv":
        articles.append(file)
temp = []
for article in articles:
    with open(article) as csv_file:
        cont = 0
        csv_reader = read_csv(csv_file, usecols=[1,2,9])
        word = word.lower()
        print_news = []
        for row in csv_reader.values.tolist():
            csv_reader = read_csv(csv_file, usecols=[1,2,9])
            word = word.lower()
            for row in csv_reader.values.tolist():
                title = str(row[1]).lower()
                news = str(row[2]).lower()
                count = find_word((title, news), word)
                if count > 0:
                    temp.append([count, row[0], row[1]])
    csv_file.close()

print_news = sorted(temp, key=lambda x: x[0])
print_news.reverse()
for to_print in print_news[:10]:
    print(to_print[0], to_print[1], to_print[2])

