#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Email {
private:
    string category;
    string subject;
    string date;

public:
    Email(const string& category, const string& subject, const string& date)
        : category(category), subject(subject), date(date) {}

    string getCategory() const {
        return category;
    }

    string getSubject() const {
        return subject;
    }

    string getDate() const {
        return date;
    }

    // Converts MM-DD-YYYY into YYYYMMDD so dates can be compared correctly.
    int dateValue() const {
        int month = stoi(date.substr(0, 2));
        int day = stoi(date.substr(3, 2));
        int year = stoi(date.substr(6, 4));

        return year * 10000 + month * 100 + day;
    }
};

class MaxHeap {
private:
    vector<Email> heap;

    int categoryPriority(const string& category) const {
        if (category == "Boss")
            return 5;
        if (category == "Subordinate")
            return 4;
        if (category == "Peer")
            return 3;
        if (category == "ImportantPerson")
            return 2;
        return 1; // OtherPerson
    }

    // Returns true if a should have higher priority than b.
    bool higherPriority(const Email& a, const Email& b) const {
        int aPriority = categoryPriority(a.getCategory());
        int bPriority = categoryPriority(b.getCategory());

        if (aPriority != bPriority) {
            return aPriority > bPriority;
        }

        // If categories are the same, the newest date has priority.
        return a.dateValue() > b.dateValue();
    }

    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;

            if (!higherPriority(heap[index], heap[parent])) {
                break;
            }

            swap(heap[index], heap[parent]);
            index = parent;
        }
    }

    void heapifyDown(int index) {
        int size = static_cast<int>(heap.size());

        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int highest = index;

            if (left < size && higherPriority(heap[left], heap[highest])) {
                highest = left;
            }

            if (right < size && higherPriority(heap[right], heap[highest])) {
                highest = right;
            }

            if (highest == index) {
                break;
            }

            swap(heap[index], heap[highest]);
            index = highest;
        }
    }

public:
    void insert(const Email& email) {
        heap.push_back(email);
        heapifyUp(static_cast<int>(heap.size()) - 1);
    }

    Email getMax() const {
        return heap[0];
    }

    void removeMax() {
        if (heap.empty()) {
            return;
        }

        heap[0] = heap.back();
        heap.pop_back();

        if (!heap.empty()) {
            heapifyDown(0);
        }
    }

    bool empty() const {
        return heap.empty();
    }

    int size() const {
        return static_cast<int>(heap.size());
    }
};

bool validCategory(const string& category) {
    return category == "Boss" ||
           category == "Subordinate" ||
           category == "Peer" ||
           category == "ImportantPerson" ||
           category == "OtherPerson";
}

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

            if (!validCategory(category)) {
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
