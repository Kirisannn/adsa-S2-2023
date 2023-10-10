#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

const vector<string> states = {"never used", "tombstone", "occupied"};

class Node {
   private:
    char hashVal;   // Has value for identification
    string key;     // Key from input
    string status;  // "never used", "tombstone" or "occupied".
    Node* next;     // Next node
    friend class HashTable;

   public:
    // Default Constructor
    Node() {
        hashVal = '\0';
        key = "";
        status = states[0];
        next = nullptr;
    };

    // Regular Constructor for hashValue
    Node(char hash) {
        hashVal = hash;
        key = "";
        status = states[0];
        next = nullptr;
    };

    // Default deconstructor
    ~Node(){};
};

// Function to get hash value from a key
char getHash(string key) {
    char result;
    size_t last = key.length() - 1;
    result = key[last];
    return result;
}

// Custom Hash Table of all letters 'a' - 'z'
class HashTable {
   private:
    Node* head;

   public:
    // Default constructor
    HashTable() {
        head = new Node('a');
        Node* prev = head;

        for (char index = 'b'; index < 'z' + 1; index++) {
            Node* curr = new Node(index);
            prev->next = curr;
            prev = curr;

            // Set node 'z' to point to head
            if (index == 'z') {
                prev->next = head;
            }
        }
    };

    // Search method. Returns false if not in the hashmap, true if in the hashmap
    Node* search(string key) {
        char hash = getHash(key);
        Node* start = nullptr;

        // Find the address of the hash
        bool end = false;
        for (Node* curr = head; end != true; curr = curr->next) {
            if (curr->hashVal == hash) {
                start = curr;
                break;
            } else if (curr->next == head) {
                end = true;
            }
        }

        // If the node has never been used, the key is definitely not in the hashmap
        if (start->key == "") {
            return nullptr;
        }
        // else if it is occupied, check if the key exists with a loop through all the other nodes
        else {
            end = false;
            for (Node* curr = start; end != true; curr = curr->next) {
                if (curr->key == key) {
                    return curr;
                } else if (curr->next == start) {
                    end = true;
                }
            }
        }

        return nullptr;
    }

    // Insertion method.
    void insert(string key) {
        // Conduct insertion if key not already in hashmap
        if (search(key) == nullptr) {
            // cout << "Commencing Insertion of " << key << endl;  // Output for easier validation

            char hash = getHash(key);
            Node* start = nullptr;

            // Find the address of the hash
            bool end = false;
            for (Node* curr = head; end != true; curr = curr->next) {
                if (curr->hashVal == hash) {
                    start = curr;
                    break;
                } else if (curr->next == head) {
                    end = true;
                }
            }

            // If key is "never used" or "tombstone", insert directly
            if (start->status == states[0] || start->status == states[1]) {
                start->key = key;
                start->status = states[2];
            }

            // otherwise, loop through to find the next available slot
            else {
                end = false;
                for (Node* curr = start; end != true; curr = curr->next) {
                    if (curr->status == states[0] || curr->status == states[1]) {
                        curr->key = key;
                        curr->status = states[2];
                        end = true;
                    } else if (curr->next == start) {
                        end = true;
                    }
                }
            }
        }

        return;
    }

    // Deletion method
    void remove(string key) {
        // Conduct removal if key is found
        if (search(key) != nullptr) {
            // cout << "Commencing Deletion of " << key << endl;  // Output for easier validation

            Node* objective = search(key);
            objective->status = states[1];
        }
    };

    // Print hashmap
    void printHash() {
        for (Node* curr = head;; curr = curr->next) {
            if (curr->status == states[2]) {
                cout << curr->key << " ";
            }
            if (curr->next == head) {
                break;
            }
        }

        cout << endl;

        return;
    };
};

// Split input into vector of instructions
vector<string> separateInput(string input) {
    vector<string> instructions;
    stringstream ss(input);
    string instruction;

    while (ss >> instruction) {
        instructions.push_back(instruction);
    }

    return instructions;
}

int main() {
    string input;
    getline(cin, input);
    HashTable* h1 = new HashTable();

    vector<string> instructions = separateInput(input);

    // Testing search().
    for (int i = 0; i < (int)instructions.size(); i++) {
        h1->search(instructions[i].substr(1, instructions[i].size() - 1));
    }  // Self note: Comment out once done

    // Execute Instructions
    for (int i = 0; i < (int)instructions.size(); i++) {
        if (instructions[i][0] == 'A') {
            h1->insert(instructions[i].substr(1, instructions[i].size() - 1));
        } else if (instructions[i][0] == 'D') {
            h1->remove(instructions[i].substr(1, instructions[i].size() - 1));
        }
    }

    h1->printHash();

    return 0;
}