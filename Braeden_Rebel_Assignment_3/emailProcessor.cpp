/*
Name: Email Processor
Description:
    Takes in a list of email commands of EMAIL, NEXT, READ, COUNT
    Program assumes that provided file will only include these commands
    Processes those commands and print output to console based on the execution of those commands
    On EMAIL <CATEGORY>,<SUBJECT>,<DATE>:
        Creates an email using the category, subject, and date arguments
        Category is restricted to 5 values Boss, Peer, Subordinate, ImportantPerson, or OtherPerson, the priority of these people follow their list order
        Date is in the format MM-DD-YYYY
    On NEXT:
        The current email is set to the most important email and then it is displayed
    On READ:
        The email with the highest priority is removed
    On COUNT:
        The program prints the current count of emails in the heap
Input:
    On 0 arguments:
        The program will ask the user for input to the filepath or an empty string to end
        If the user inputs something that does not lead to a file then the query is repeated until valid input given
        After getting valid input, program will begin
    On 1 argument:
        The program checks if the path given is a valid file path
        If the path is correct then the program will start with the file it was led to
        If the path is incorrect, the program informs the user of the invalid path and then acts as if no argument was given
    Else:
        The program informs the user of the proper usage of the program, program then acts as if only one argument was given
Output: 
    Program outputs the result of processing the file while also providing any extra messages that are necessary to describe the state of the program
Collaborators: N/A
Sources: ChatGPT (ChatGPT)
Author: Braeden Rebel (Braeden)
Creation Date: 9/31/2026
Revision Date: 10/01/2026
*/
// Include various packages
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// Use std for the standard input output string functionality
using namespace std;

/*
Class: Email
Description: A class that represents a single email and can perform comparisons or prints
Properties:
    private string category
    private string subject
    private string date
    private int id
*/
// Start Email definition (ChatGPT)
class Email {
// Define private properties of the Email (ChatGPT)
private:
    string category;
    string subject;
    string date;
    inline static int EMAIL_ID = 0;
    int id;

    /*
    Method: getCategoryPriority();
    Return: int
    Description: Helper function that returns the priority value of the emails category
    Author: Braeden, ChatGPT
    */
    int getCategoryPriority() const {
        if (category == "Boss")
            return 5; // Boss has priority of 5
        if (category == "Subordinate")
            return 4; // Suboridinate has priority of 4
        if (category == "Peer")
            return 3; // Peer has priority of 3
        if (category == "ImportantPerson")
            return 2; // ImportantPerson has priority of 2
        return 1; // OtherPerson has priority of 1, category has to be OtherPerson as it is last option
    }

    /*
    Method: getYearPriority();
    Return: int
    Description: Helper function that returns the priority value of the year in date
    Author: Braeden
    */
    int getYearPriority() const
    {
        return stoi(date.substr(6, 4));
    }

    /*
    Method: getMonthPriority();
    Return: int
    Description: Helper function that returns the priority value of the month in date
    Author: Braeden
    */
    int getMonthPriority() const
    {
        return stoi(date.substr(0, 2));
    }

    /*
    Method: getDayPriority();
    Return: int
    Description: Helper function that returns the priority value of the day in date
    Author: Braeden
    */
    int getDayPriority() const
    {
        return stoi(date.substr(3, 2));
    }

public:
    // Define public constructor that assigns values to the email's private properties (ChatGPT) and then assigns id to the current EmailID and then increment sit (Braeden)
    Email(const string& category, const string& subject, const string& date)
        : category(category), subject(subject), date(date), id(EMAIL_ID++) {}

    /*
    Method: getCategory();
    Return: string
    Description: Returns the category private property
    Author: ChatGPT
    */
    string getCategory() const {
        return category;
    }

    /*
    Method: getSubject();
    Return: string
    Description: Returns the subject private property
    Author: ChatGPT
    */
    string getSubject() const {
        return subject;
    }

    /*
    Method: getDate();
    Return: string
    Description: Returns the date private property
    Author: ChatGPT
    */
    string getDate() const {
        return date;
    }

    // Overload the greater-than operator for comparing between emails (Braeden)
    bool operator>(const Email& other)
    {
        // First try to compare by categories
        if (category != other.category)
        {
            // The email with the higher category priority is greater than
            return getCategoryPriority() > other.getCategoryPriority();
        }
        // Categories are same so try to sort by date
        else if (date != other.date)
        {
            // Dates are different, try to sort by year first
            int yearPriority = getYearPriority();
            int otherYearPriorty = other.getYearPriority();
            if (yearPriority != otherYearPriorty)
            {
                // The email with the higher year priority is greater than
                return yearPriority > otherYearPriorty;
            }
            else
            {
                // Years are same so try to sort by month
                int monthPriority = getMonthPriority();
                int otherMonthPriority = other.getMonthPriority();
                if (monthPriority != otherMonthPriority)
                {
                    // The email with higher month priority is greater than
                    return monthPriority > otherMonthPriority;
                }
                // Months are equal but date is inequal so days are automatically different
                int dayPriority = getDayPriority();
                int otherDayPriority = other.getDayPriority();
                // Email with higher day priority is greater than
                return dayPriority > otherDayPriority;
            }
        }
        // Dates and category are same, so to keep most recent email prioritized, use id
        else
        {
            // Email with lower id is more recent and thus email with lower id is greater than
            return other.id > id;
        }
    }
};

/*
Class: MaxHeap
Description: A list-based MaxHeap that stores Email objects and sort them by priority
Properties:
    private vector<Email> heap
Author: ChatGPT
*/
class MaxHeap {
private:
    // Internal vector of emails for list-based implementation (ChatGPT)
    vector<Email> heap;

    // Heapify up function that follows standard heapify up procedure to keep structure, takes current index (ChatGPT)
    void heapifyUp(int index) {
        // Try to keep heapifying up until index is at index 0 (ChatGPT)
        while (index > 0) {
            // Get parent index (ChatGPT)
            int parent = (index - 1) / 2;
            // If the Email at the current index is not greater than the parent then break (Braeden)
            if (!(heap[index] > heap[parent])) {
                break;
            }
            // Swap the two Emails at the index and parent index since the Email at the index is greater than the Email at the parent index (ChatGPT)
            swap(heap[index], heap[parent]);
            // Set new target index to parent index as the current email is now stored there (ChatGPT)
            index = parent;
        }
    }

    // Heapify down function that follows standard heapify down procedure to keep structure, takes curent index (ChatGPT)
    void heapifyDown(int index) {
        // Get size of heap as an integer (ChatGPT)
        int heapSize = size();
        // Loop until the heapifyDown is broken out of (ChatGPT)
        while (true) {
            // Get left and right child indexes (ChatGPT)
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            // Start with highest index being the current index (ChatGPT)
            int highest = index;

            // If the left child exists and is greater than the current Email then the highest is left (Braeden)
            if (left < heapSize && heap[left] > heap[highest]) {
                highest = left;
            }
            // If the right child exists and is greater than both the currentEmail and the leftEmail then it is the highest Email (Braeden)
            if (right < heapSize && heap[right] > heap[highest]) {
                highest = right;
            }
            // If the current Email is the highest then there is no more need to heapify down (ChatGPT)
            if (highest == index) {
                break;
            }
            // Since current email is not highest then swap the current Email and highest Email (ChatGPT)
            swap(heap[index], heap[highest]);
            // Set index to highest to keep the index properly targeted on current Email (ChatGPT)
            index = highest;
        }
    }

public:
    // Insert an Email into the MaxHeap (ChatGPT)
    void insert(const Email& email) {
        // Push the new email into the back of the MaxHeap (ChatGPT)
        heap.push_back(email);
        // Heapify up at the last index of the vector to maintain sorting (ChatGPT)
        heapifyUp(size()- 1);
    }

    // Get the max email in the MaxHeap (ChatGPT)
    Email getMax() const {
        return heap[0]; // Return the first element in the vector as it is the max email in the MaxHeap (ChatGPT)
    }

    // Remove the top value of the heap (ChatGPT)
    void removeMax() {
        // No need to remove if the heap is empty (ChatGPT)
        if (heap.empty()) {
            return;
        }
        // Set the first element in the heap to match the element in the back of the heap (ChatGPT)
        heap[0] = heap.back();
        // Remove the back element (ChatGPT)
        heap.pop_back();
        // If the removed element wasn't the last, heapify down the element that replaced the initial max to maintain sorting (ChatGPT)
        if (!heap.empty()) {
            heapifyDown(0);
        }
    }

    // Return if the MaxHeap is empty or not (ChatGPT)
    bool empty() const {
        return heap.empty(); // Return if the internal vector is empty since it matches if the MaxHeap is empty or not (ChatGPT)
    }

    // Return the size of the MaxHeap as an integer (ChatGPT)
    int size() const {
        return static_cast<int>(heap.size()); // Return the size of the internal vector as it matches the MaxHeap's size, but convert to an integer first (ChatGPT)
    }
};

int main(int argc, char* argv[]) {
    // The input file is supplied as a command-line argument.
    // Example: ./assignment3 input.txt
    if (argc < 2) {
        return 0;
    }

    ifstream inputFile(argv[1]);

    if (!inputFile.is_open()) {
        return 0;
    }

    MaxHeap emailQueue;

    // true  -> NEXT is allowed
    // false -> NEXT is not allowed until READ occurs
    bool nextAllowed = true;

    // true  -> READ is allowed because NEXT was most recently performed
    // false -> READ is not currently allowed
    bool readAllowed = false;

    string line;

    while (getline(inputFile, line)) {
        // Ignore completely empty lines.
        if (line.empty()) {
            continue;
        }

        // EMAIL command
        if (line.rfind("EMAIL ", 0) == 0) {
            string data = line.substr(6);

            string category;
            string subject;
            string date;

            stringstream ss(data);

            if (!getline(ss, category, ',')) {
                continue;
            }

            if (!getline(ss, subject, ',')) {
                continue;
            }

            if (!getline(ss, date)) {
                continue;
            }

            Email email(category, subject, date);
            emailQueue.insert(email);

            // EMAIL does not change whether NEXT/READ is required.
        }

        // NEXT command
        else if (line == "NEXT") {
            // NEXT is invalid when there are no emails or when the
            // previous NEXT has not been followed by READ.
            if (emailQueue.empty() || !nextAllowed) {
                continue;
            }

            const Email& email = emailQueue.getMax();

            cout << "Next email:" << endl;
            cout << "Sender: " << email.getCategory() << endl;
            cout << "Subject: " << email.getSubject() << endl;
            cout << "Date: " << email.getDate() << endl;

            nextAllowed = false;
            readAllowed = true;
        }

        // READ command
        else if (line == "READ") {
            // READ is invalid when there are no emails or when NEXT
            // has not been performed since the previous READ.
            if (emailQueue.empty() || !readAllowed) {
                continue;
            }

            emailQueue.removeMax();

            nextAllowed = true;
            readAllowed = false;
        }

        // COUNT command
        else if (line == "COUNT") {
            cout << "There are " << emailQueue.size()
                 << " emails to read." << endl;
        }

        // Any other command is simply ignored.
    }

    inputFile.close();

    return 0;
}
