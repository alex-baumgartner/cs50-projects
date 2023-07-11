from cs50 import get_string

text = get_string("Text: ")

words = len(text.split(' '))

sentence_endings = ['.', '!', '?']

sentences = 0
letters = 0

for char in text:
    if char in sentence_endings:
        sentences += 1
    if char.isalpha():
        letters += 1

average_letters = letters / words * 100
average_sentences = sentences / words * 100

index = round(0.0588 * average_letters - 0.296 * average_sentences - 15.8)

if index < 1:
    print("Before Grade 1")
elif index > 16:
    print("Grade 16+")
else:
    print("Grade " + str(index))