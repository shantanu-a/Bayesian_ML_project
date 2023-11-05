#include<bits/stdc++.h>
// #include <iostream>
// #include <fstream>
// #include <sstream>
// #include <string>
// #include <vector>
// #include <map>
using namespace std;

class NaiveBayesClassifier {
private:
    map<string, int> spamWordCount;
    map<string, int> hamWordCount;
    int totalSpamEmails = 0;
    int totalHamEmails = 0;
    int totalSpamWords = 0;
    int totalHamWords = 0;

    vector<string> tokenize(const string &text) {
        vector<string> tokens;
        istringstream iss(text);
        string token;
        while (iss >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }

public:
    void train(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Unable to open file." << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            istringstream lineStream(line);
            string email, label;
            if (getline(lineStream, email, ',') && getline(lineStream, label, ',')) {
                vector<string> words = tokenize(email);
                if (label == "spam") {
                    totalSpamEmails++;
                    for (const auto &word : words) {
                        spamWordCount[word]++;
                        totalSpamWords++;
                    }
                } else if (label == "ham") {
                    totalHamEmails++;
                    for (const auto &word : words) {
                        hamWordCount[word]++;
                        totalHamWords++;
                    }
                }
            }
        }
        file.close();
        // cout<<"totalSpamEmails: "<<totalSpamEmails<<endl;
        // cout<<"totalHamEmails: "<<totalHamEmails<<endl;
        // cout<<"totalSpamWords: "<<totalSpamWords<<endl;
        // cout<<"totalHamWords: "<<totalHamWords<<endl;
    }

    string classify(const string& email) {
        vector<string> words = tokenize(email);

        double spamProbability = static_cast<double>(totalSpamEmails) / (totalSpamEmails + totalHamEmails);
        double hamProbability = static_cast<double>(totalHamEmails) / (totalSpamEmails + totalHamEmails);

        for (const auto &word : words) {
            spamProbability *= (static_cast<double>(spamWordCount[word] + 1) / (totalSpamWords + spamWordCount.size()));
            hamProbability *= (static_cast<double>(hamWordCount[word] + 1) / (totalHamWords + hamWordCount.size()));
        }

        if (spamProbability > hamProbability) {
            return "spam";
        } else {
            return "ham";
        }
    }
};

int main() {
    NaiveBayesClassifier classifier;
    classifier.train("spam_ham_dataset.csv");

    // Test emails
    string email1 = "Buy cheap viagra now!";
    string email2 = "Hi, how are you?";
    string email3= "Hi how Dhruv you!";

    cout << "Email 1 classified as: " << classifier.classify(email1) << endl;
    cout << "Email 2 classified as: " << classifier.classify(email2) << endl;
    cout << "Email 3 classified as: " << classifier.classify(email3) << endl;


    return 0;
}
