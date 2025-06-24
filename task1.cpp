#include <iostream>
#include <fstream>
#include <string>
using namespace std;


//Overwrites the file with new content entered by the user.
void writeToFile(const string& filename) {
    ofstream outFile(filename); // Opens file in overwrite mode
    if (!outFile) {
        cerr << "Error: Could not open file for writing.\n";
        return;
    }

    cout << "Enter text to write to the file (overwrite mode):\n> ";
    string input;
    getline(cin, input);

    outFile << input << endl;
    outFile.close();
    cout << "Data written to file.\n";
}


//Appends user input to the end of the file without erasing existing content.
void appendToFile(const string& filename) {
    ofstream outFile(filename, ios::app); 
    if (!outFile) {
        cerr << "Error: Could not open file for appending.\n";
        return;
    }

    cout << "Enter text to append to the file:\n> ";
    string input;
    getline(cin, input);

    outFile << input << endl;
    outFile.close();
    cout << "Data appended to file.\n";
}


//Reads and displays the content of the file line by line.
void readFromFile(const string& filename) {
    ifstream inFile(filename);
    if (!inFile) {
        cerr << "Error: Could not open file for reading.\n";
        return;
    }

    cout << "Contents of '" << filename << "':\n";
    string line;
    while (getline(inFile, line)) {
        cout << line << endl;
    }

    inFile.close();
}

int main() {
    string filename = "data.txt";
    int choice;

    do {
        cout << "\n📘 File Handling Menu:\n";
        cout << "1. Write to file (overwrite)\n";
        cout << "2. Append to file\n";
        cout << "3. Read from file\n";
        cout << "4. Exit\n";
        cout << "Choose an option (1-4): ";
        cin >> choice;
        cin.ignore(); // Flush newline character from buffer

        switch (choice) {
            case 1:
                writeToFile(filename);
                break;
            case 2:
                appendToFile(filename);
                break;
            case 3:
                readFromFile(filename);
                break;
            case 4:
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid option. Please choose 1-4.\n";
        }
    } while (choice != 4);

    return 0;
}
