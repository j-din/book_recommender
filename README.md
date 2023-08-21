# book_recommender
This C++ program provides book recommendations to users in a file by calculating the similarities between ratings

Overview

This program utilizes a dataset/file containing user-book ratings to generate book recommendations. There are two main features:

1. Average ratings of all books in the file
2. Individual recommendations to people in the file

Compiling and Running 

Compile the program using the following command: g++ -o book_recommender main.cpp BookRecommender.cpp
Run the program with: ./book_recommender small_ratings.dat 
(or replace small_ratings.dat with your own file)

Usage

There are three commands - averages, recommend, and quit

Entering "averages" will print the average ratings of all books in the file
Entering "recommend" will require the user to then enter a name from the file. Afterward, the program will show recommendations for the individual entered
(If the user is not in the file, the program will print average ratings of all books)
Entering "quit" will exit the program.

File Format

UserName1
BookTitle1
Rating1
UserName2
BookTitle2
Rating2
...

UserNameX is the name of the user - for example, "Bob".
BookTitleX is the name of the book - for example, "Harry Potter"
RatingX is the individual's rating for that book on a -5 to 5 scale

Function Descriptions

BookRecommender::BookRecommender(string fileName)
Constructor that reads data from file

void BookRecommender::printRecommend(string userName)
Prints book recommendations for given name

void BookRecommender::printAverages()
Displays average ratings for all books

double BookRecommender::getAverage(string bookTitle)
Calculates the average rating for a single specified book

double BookRecommender::getSimilarity(string userName1, string userName2)
Calculates similarity (dot product) between two users' ratings

double BookRecommender::getUserBookRating(string userName, string bookTitle)
Retrieves rating that a user has given to a book
 