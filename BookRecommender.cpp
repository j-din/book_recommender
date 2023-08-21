/*
This program provides recommendations to users by utilizing the average rating or by calculating the 
similarity / dot product between users.
*/

#include "BookRecommender.h"

// function to compare ratings and titles so sorting is possible
bool compare(pair<double, string> a, pair<double, string> b)
{
    if (a.first > b.first) 
    {
        return true;
    }    
    else if (a.first == b.first) 
    {
        return a.second < b.second;
    }
    return false;
}

/* 
constructor that reads in file from the parameter and gets user book and rating for each user rating 
used to populate the books and users vector and also the ratedBooks and averages map
*/
BookRecommender::BookRecommender(string fileName) 
{
    ifstream infile(fileName);
    set<string> bookSet, userSet;
    string line;
    // while in file insert variable into corresponding set
    while (getline(infile, line))
    {
        if (line[line.length() - 1] == '\r')
        {
            line.erase(line.length() - 1);
        }
        userSet.insert(line);
        getline(infile, line);
        bookSet.insert(line);
        getline(infile, line);
    }
    // convert sets to vectors
    users = vector<string>(userSet.begin(), userSet.end());
    books = vector<string>(bookSet.begin(), bookSet.end());
    // start from beginning of file 
    infile.close();
    infile.open(fileName);
    // go through file again and populate ratedBooks map 
    while (getline(infile, line))
    {
        if (line[line.length() - 1] == '\r')
        {
            line.erase(line.length() - 1);
        }
        string user = line;
        getline(infile, line);
        string book = line;
        getline(infile, line);
        int rating = stoi(line);
        // find where rating should be and resize the vector so we can add rating to map
        int index = distance(books.begin(), find(books.begin(), books.end(), book)); 
        ratedBooks[user].resize(books.size(), 0);
        ratedBooks[user][index] = rating;
    }
    infile.close();
    // call get averages to calculate the average rating for all books and create key for averages map
    for (auto b : books)
    {
        double avg = getAverage(b);
        averages[b] = avg;
    }
}

/*
this function takes in a user name as the parameter so it can calculate the similarity between the person
and other users it then stores the top 3 users and calculates the average rating for each book.
The average ratings are then sorted and recommended.
*/
void BookRecommender::printRecommend(string userName)
{
    // check to see if the user exists if not print all book averages
    if (ratedBooks.find(userName) == ratedBooks.end()) 
    {
        printAverages();
        return;
    }

    vector<pair<double, string>> similarities;
    // loop through users and calculate similarity between the current user and the rest
    for (auto user : users) 
    {
        // do not compare user to themselves
        if (user != userName) 
        {
            // call get similarity to calculate dot-product
            double sim = getSimilarity(userName, user);
            similarities.push_back(make_pair(sim, user));
        }
    }

    // sort by rating
    sort(similarities.begin(), similarities.end(), compare);

    // stores top 3 similar users 
    vector<pair<double, string>> topThree;
    auto it = similarities.begin();
    for (int i = 0; i < 3 && it != similarities.end(); i++, it++) 
    {
        topThree.push_back(*it);
    }
    // create a vector the same size as book filled with 0's
    vector<double> avg(getBookCount(), 0.0);
    // loop through avg and for every index loop through top three users to calculate average ratings
    for (int i = 0; i < getBookCount(); i++) 
    {
        int n = 0;
        double sum = 0.0;
        for (int j = 0; j < 3 && j < similarities.size(); j++) 
        {
            // get rating by passing in username and book title 
            double r = getUserBookRating(similarities[j].second, books[i]);
            sum += r;
            if (r != 0) 
            {
                n++;
            }
        }
        if (n != 0) 
        {
            avg[i] = sum / n;
        }
    }

    // vector of pairs that contains the average rating and book title
    vector<pair<double, string>> recommendedBooks;
    for (int i = 0; i < books.size(); i++) 
    {
        if (avg[i] > 0) 
        {
            recommendedBooks.push_back(make_pair(avg[i], books[i]));
        }
    }

    // sort by rating and if there is a tie sort by title
    sort(recommendedBooks.begin(), recommendedBooks.end(), compare);

    // print recommended books and delete '\r'
    for (auto book : recommendedBooks) 
    {
        if (book.second[book.second.length() - 1] == '\r')
        {
            book.second.erase(book.second.length() - 1);
        }
        cout << book.second << " " << fixed << setprecision(2) << book.first << endl;
    }
}

// This function prints out the average rating of all books in the averages map
void BookRecommender::printAverages() 
{
    // create a vector to to convert map to so sorting is possible
    vector<pair<double, string>> sortedAverages;
    // pair it in a way the sorting function can be called
    for (auto pair : averages) 
    {
        sortedAverages.push_back(make_pair(pair.second, pair.first));
    }
    // sort by rating (highest to lowest) and if there is a tie sort by title (alphabetical)
    sort(sortedAverages.begin(), sortedAverages.end(), compare); 
    // print averages after erasing '\r' character
    for (auto p : sortedAverages) 
    {
        if (p.second[p.second.length() - 1] == '\r')
        {
            p.second.erase(p.second.length() - 1);
        }
        cout << p.second << " " << fixed << setprecision(2) << p.first << endl;
    }
}

// This function calculates and returns the average rating for a given book title. It also stores the average rating in the averages map.
double BookRecommender::getAverage(string bookTitle) 
{
    double sum = 0.0;
    int n = 0;
    // loop through each user in ratedBooks map
    for (auto user : ratedBooks) 
    {
        // get the rating for the current book title from the user's ratings (user.second is rating)
        int rating = user.second[distance(books.begin(), find(books.begin(), books.end(), bookTitle))];
        // sum up non 0 ratings and increment number of non 0 ratings
        if (rating != 0) 
        {
            sum += rating;
            n++;
        }
    }
    double average;
    // if no ratings average is 0 else claculate avergae by dividing sum of ratings by number of ratings
    if (n == 0) { average = 0; } else { average = sum / n; }
    // populate map with value
    averages[bookTitle] = average;
    return average;
}

// calculates dot product between two users in parameter
double BookRecommender::getSimilarity(string userName1, string userName2)
{
    // get ratings of two users
    vector<double> ratings1 = ratedBooks[userName1];
    vector<double> ratings2 = ratedBooks[userName2];
    double dotProduct = 0.0;
    // calculate dot product by multiplying each users index by each other and adding them together
    // (ratings1[1] * ratings2[1]) + (ratings1[2] * ratings2[2])....
    for (int i = 0; i < ratings1.size(); i++)
    {
        dotProduct += ratings1[i] * ratings2[i];
    }

    return dotProduct;
}

// This function takes in user and book and returns the rating that the user has given the specific book.
double BookRecommender::getUserBookRating(string userName, string bookTitle)
{
    // get the index of the book 
    int bookIndex = distance(books.begin(), find(books.begin(), books.end(), bookTitle));
    // if the book is not found return 0
    if (bookIndex == getBookCount()) 
    {
        return 0.0;
    }
    // return the rating corresponding to the index found from the book title
    return ratedBooks[userName][bookIndex];
}