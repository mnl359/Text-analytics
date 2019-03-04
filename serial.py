#!/usr/bin/python3

from csv import DictReader, field_size_limit
from pandas import read_csv
from sys import maxsize
from os import listdir

field_size_limit(maxsize)

def find_word(news, word):
    count_tit = news[0].count(word)
    count_des = news[1].count(word)
    return count_tit + count_des

directory = input("Ingrese la ruta de los directorios: ")
files = listdir(directory)
if directory[len(directory)-1] != "/":
    directory += "/"
articles = []
for file in files:
    filename, extension = file.split(".")
    if extension == "csv":
        articles.append(directory + file)
articles = ['../all-articles/articles3.csv', '../all-articles/articles1.csv', '../all-articles/articles2.csv']
word = input("Ingrese la palabra a buscar: ")
temp = []
for article in articles:
    print(article)
    with open(article) as csv_file:
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

