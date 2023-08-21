#ifndef BOOK_RECOMMENDER_H
#define BOOK_RECOMMENDER_H

#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <map>
#include <iomanip>
#include <algorithm>

using namespace std;

class BookRecommender 
{
private:
    // attributes
    vector<string> books; 
    vector<string> users; 
    map<string, vector<double>> ratedBooks; 
    map<string, double> averages;
public:
    // constructors
    BookRecommender() = default; 
    BookRecommender(string fileName); 
    // methods
    void printRecommend(string userName); 
    void printAverages(); 
    double getAverage(string bookTitle); 
    double getSimilarity(string userName1, string userName2); 
    int getBookCount() {return books.size();} // returns books size
    int getUserCount() { return users.size(); } // returns users size
    double getUserBookRating(string userName, string bookTitle); 
};

#endif