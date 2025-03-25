#include <bits/stdc++.h>
using namespace std;

//Represents the lowercase alphabet
const int MAXC = 26;

// TrieNode struct to represent each state
struct TrieNode {
    vector<int> transitions; // Transitions for each character
    int failure;  // Failure link
    vector<int> output; //vect of word indices that end at this node

    TrieNode() {
        transitions.assign(MAXC, -1); // Initialize all transitions to -1
        failure = -1; //  failure link  starts at -1
    }
};

// The tri represented as a vect of nodes
vector<TrieNode> trie;

int buildFsa(string arr[], int k) {
    trie.clear(); // Clear the trie
    trie.push_back(TrieNode()); // make root node

    int states = 1; // Start with 1 state bc state 0 is root

    //Insert each word into the trie
    for (int i = 0; i < k; ++i) {
        const string& word = arr[i];
        int currentState = 0; //start at the root node

        for (char c : word) {
            int ch = c - 'a'; // Convert character to index equivolent
            if (trie[currentState].transitions[ch] == -1) {
                trie[currentState].transitions[ch] = states++; //Create a new state
                trie.push_back(TrieNode()); // Add a new state to the trie
            }
            currentState = trie[currentState].transitions[ch]; // Move to the next state
        }

        trie[currentState].output.push_back(i); // Mark the end of word at curr state
    }

    // Set up failure links
    queue<int> q;

    for (int ch = 0; ch < MAXC; ++ch) {
        if (trie[0].transitions[ch] != -1) {
            trie[trie[0].transitions[ch]].failure = 0; // Set failure link for non-zero transitions from root
            q.push(trie[0].transitions[ch]); // Enqueue those states for processing
        }
    }

    while (!q.empty()) {
        int state = q.front();
        q.pop();

        for (int ch = 0; ch < MAXC; ++ch) {
            if (trie[state].transitions[ch] != -1) {
                int failure = trie[state].failure;

                while (failure != -1 && trie[failure].transitions[ch] == -1)
                    failure = trie[failure].failure; // Follow failure link until a match is found

                if (failure == -1)
                    failure = 0; // If no match is found, fallback to the root state
                else
                    failure = trie[failure].transitions[ch]; // Otherwise, move to the next valid state

                trie[trie[state].transitions[ch]].failure = failure; // Set failure link for the new state
                // Merge output from failure state into the current state
                trie[trie[state].transitions[ch]].output.insert(
                    trie[trie[state].transitions[ch]].output.end(),
                    trie[failure].output.begin(),
                    trie[failure].output.end()
                );

                q.push(trie[state].transitions[ch]); // Push the new state into the queue for further processing
            }
        }
    }

    return states; // Return the total number of states created
}

int findNextState(int currentState, char nextInput) {
    // Return the current state if the character is a space
    if (nextInput == ' ') {
        return currentState; 
    }

    int ch = nextInput - 'a'; // Conver character to index 
    while (currentState != -1 && trie[currentState].transitions[ch] == -1)
        currentState = trie[currentState].failure; // Follow the failure link if no match
    return (currentState == -1) ? 0 : trie[currentState].transitions[ch]; // Return the next state or state 0 if no match
}

void search(string arr[], int k, string text) {
    buildFsa(arr, k); // Build the fsa
    int currentState = 0;

    for (int i = 0; i < text.size(); ++i) {
        if (text[i] == ' ') continue; // skip spaces in the input text

        currentState = findNextState(currentState, text[i]); // Find the next state for the character

        if (!trie[currentState].output.empty()) { //Check if any words end at the curr state
            for (int j : trie[currentState].output) {
                cout << "Word " << arr[j] << " appears from "
                     << i - arr[j].size() + 1 << " to " << i << endl;
            }
        }
    }
}

int main() {
    string arr[] = {"cat", "sat", "rat", "at"}; // The words to search for
    string text = "the cat sat on the mat"; // The text to search within
    int k = sizeof(arr) / sizeof(arr[0]); // Number of words in arr

    search(arr, k, text); // Search for words in the text

    return 0;
}
