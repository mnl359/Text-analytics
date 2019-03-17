#!/home/gwen/miniconda3/bin/python

import pandas
import nltk
nltk.download('punkt')
from nltk.corpus import stopwords
from nltk.tokenize import word_tokenize
nltk.download('stopwords')

import glob 

used_columns = ['id', 'title', 'content']

stop_words = ["i", "me", "my", "myself", "we", "our", "ours", "ourselves", "you", "your", "yours", "yourself", 
"yourselves", "he", "him", "his", "himself", "she", "her", "hers", "herself", "it", "its", "itself", "they", "them", 
"their", "theirs", "themselves", "what", "which", "who", "whom", "this", "that", "these", "those", "am", "is", "are", 
"was", "were", "be", "been", "being", "have", "has", "had", "having", "do", "does", "did", "doing", "a", "an", "the", 
"and", "but", "if", "or", "because", "as", "until", "while", "of", "at", "by", "for", "with", "about", "against", 
"between", "into", "through", "during", "before", "after", "above", "below", "to", "from", "up", "down", "in", "out", 
"on", "off", "over", "under", "again", "further", "then", "once", "here", "there", "when", "where", "why", "how", "all", 
"any", "both", "each", "few", "more", "most", "other", "some", "such", "no", "nor", "not", "only", "own", "same", "so", 
"than", "too", "very", "s", "t", "can", "will", "just", "don", "should", "now", ".", "?", "!", ",", ";", ":", "-", "_",
"[", "]", "{", "}", "(", ")", "...", "\'", "\"", '“', '’', '”', "$", "%", "^", "&", "*", "-", "\\", "/", "@", "!", "—"]


stop_words = set(stopwords.words('english'))

quotation = [".", "?", "!", ",", ";", ":", "-", "_", "[", "]", "{", "}", "(", ")", "...", "\'", "\"", '“', '’', '”', "$", "%", "^", "&", "*", "-", "\\", "/", "@", "!", "—"]

for i in quotation: stop_words.add(i)

def cleaning_content(content):
    words = word_tokenize(content)
    cleaned_str = []
    for word in words:
        word = word.lower()
        if word not in stop_words:
            cleaned_str.append(word)
    
    return ' '.join(cleaned_str)

folder = "/opt/datasets"
dataset = glob.glob(folder + "/*.csv")

file_word = 'file'
i = 0

for file_name in dataset:
    print(file_name)
    data_frame = pandas.read_csv(file_name, skipinitialspace=True, usecols=used_columns)
    data_frame['content'].dropna(inplace=True)
    data_frame['content'] = data_frame['content'].apply(cleaning_content)
    i += 1
    data_frame.to_csv(file_word + str(i) + ".csv", sep='\t')