#!/usr/local/lib/python3.7

from csv import DictReader, field_size_limit
from pandas import read_csv
from sys import maxsize

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

word = input("Ingrese la palabra a buscar: ")
articles = ['../all-articles/articles1.csv', '../all-articles/articles2.csv', '../all-articles/articles3.csv']
#articles=["prueba.csv"]

biggest = []
for article in articles:
    with open(article) as csv_file:
        cont = 0
        try:
            csv_reader = read_csv(csv_file)
            word = word.lower()
            print_news = []
            for row in csv_reader.values.tolist():
                title = row[2].lower()
                news = row[9].lower()
                if title.find(word) > 0 or news.find(word) > 0:
                    count = find_word((title, news), word)
                    if cont < 10:
                        print_news.append([count, row[1], row[2]])
                    else:
                        print_news.sort(reverse=True)
                        print_news.append([count, row[1], row[2]])
                        print_news.pop(len(print_news) - 1)
                    cont += 1
        except UnicodeDecodeError:
            pass

for to_print in print_news:
    print(to_print[0], to_print[1], to_print[2])

