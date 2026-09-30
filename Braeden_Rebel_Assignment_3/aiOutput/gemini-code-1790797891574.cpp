#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

// Helper function to remove leading and trailing whitespace from strings
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (string::npos == first) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

// Class to represent an Email object
class Email {
public:
    string category;
    string subject;
    string date;
    int priority;

    Email(string c, string s, string d) : category(c), subject(s), date(d) {
        // Remove spaces to cleanly check against allowed categories
        string cleanCat = "";
        for (char ch : c) {
            if (ch != ' ') cleanCat += ch;
        }
        
        // Assign priorities based on the specified hierarchy 
        if (cleanCat == "Boss") priority = 5;
        else if (cleanCat == "ImportantPerson") priority = 4;
        else if (cleanCat == "Peer") priority = 3;
        else if (cleanCat == "Subordinate") priority = 2;
        else if (cleanCat == "OtherPerson") priority = 1;
        else priority = 0; // Fallback
    }

    // Overload the less-than operator to define priority for the MaxHeap
    bool operator<(const Email& other) const {
        if (priority != other.priority) {
            return priority < other.priority;
        }
        
        // Tie-breaker: Date in MM-DD-YYYY format
        int m1 = stoi(date.substr(0, 2));
        int d1 = stoi(date.substr(3, 2));
        int y1 = stoi(date.substr(6, 4));

        int m2 = stoi(other.date.substr(0, 2));
        int d2 = stoi(other.date.substr(3, 2));
        int y2 = stoi(other.date.substr(6, 4));

        // Prioritize newer years, then months, then days
        if (y1 != y2) return y1 < y2;
        if (m1 != m2) return m1 < m2;
        return d1 < d2;
    }
};

// Custom MaxHeap class for priority queue implementation
class MaxHeap {
private:
    vector<Email> heap;

    // Restores the max-heap property by bubbling up
    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[parent] < heap[index]) {
                swap(heap[parent], heap[index]);
                index = parent;
            } else {
                break;
            }
        }
    }

    // Restores the max-heap property by bubbling down
    void heapifyDown(int index) {
        int size = heap.size();
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int largest = index;

            if (left < size && heap[largest] < heap[left]) {
                largest = left;
            }
            if (right < size && heap[largest] < heap[right]) {
                largest = right;
            }

            if (largest != index) {
                swap(heap[index], heap[largest]);
                index = largest;
            } else {
                break;
            }
        }
    }

public:
    void insert(const Email& email) {
        heap.push_back(email);
        heapifyUp(heap.size() - 1);
    }

    Email getMax() const {
        return heap[0];
    }

    void extractMax() {
        if (heap.empty()) return;
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) {
            heapifyDown(0);
        }
    }

    int getSize() const {
        return heap.size();
    }
};

int main(int argc, char* argv[]) {
    string filename;
    if (argc > 1) {
        filename = argv[1];
    } else {
        cout << "Enter filename: ";
        cin >> filename;
    }

    ifstream file(filename);
    if (!file.is_open()) {
        return 1;
    }

    MaxHeap heap;
    bool next_called = false; 
    string line;

    while (getline(file, line)) {
        line = trim(line);
        if (line.empty()) continue;

        if (line.substr(0, 6) == "EMAIL ") {
            // Process the EMAIL command
            string rest = line.substr(6);
            size_t comma1 = rest.find(',');
            if (comma1 == string::npos) continue; 
            size_t comma2 = rest.find(',', comma1 + 1);
            if (comma2 == string::npos) continue; 

            string cat = trim(rest.substr(0, comma1));
            string subj = trim(rest.substr(comma1 + 1, comma2 - comma1 - 1));
            string date = trim(rest.substr(comma2 + 1));

            heap.insert(Email(cat, subj, date));
        } 
        else if (line == "NEXT") {
            // NEXT is invalid if no emails or without a READ between it and the last NEXT
            if (heap.getSize() == 0) continue;
            if (next_called) continue;

            Email nextEmail = heap.getMax();
            cout << "Next email:\n";
            cout << "Sender: " << nextEmail.category << "\n";
            cout << "Subject: " << nextEmail.subject << "\n";
            cout << "Date: " << nextEmail.date << "\n";

            next_called = true;
        }
        else if (line == "READ") {
            // READ is invalid if no emails or without a NEXT between it and the last READ
            if (heap.getSize() == 0) continue;
            if (!next_called) continue;

            heap.extractMax();
            next_called = false;
        }
        else if (line == "COUNT") {
            // Display total number of emails in the heap
            cout << "There are " << heap.getSize() << " emails to read.\n";
        }
    }

    file.close();
    return 0;
}