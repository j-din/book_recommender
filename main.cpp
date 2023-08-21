// gets file from command line and constructs an object so the user can enter commands to act on that object

#include "BookRecommender.h"

int main(int argc, char* argv[]) 
{
    string fileName = argv[1]; // get file name from command line
    BookRecommender obj(fileName); // construct an object from file
    string command, name;

    // until user enters quit allow them to keep entering commands
    while (cin >> command) 
    {
        // if recommend get name and call recommend function
        if (command == "recommend") 
        {
            cin.ignore();
            getline(cin, name); // user to get recommendations for
            obj.printRecommend(name);
        }
        // if user enters averages call print averages 
        else if (command == "averages") 
        {
            obj.printAverages();
        } 
        // if user enters quit exit program
        else if (command == "quit") 
        {
        break;
        }
    }

  return 0;
}