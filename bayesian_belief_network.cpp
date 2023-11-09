#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <map>
#include <string>
#include<iomanip>

using namespace std;

class Node {
public:
    string name;
    vector<pair<int, double>> probabilities;  // Probability distribution for the node
    map<vector<int>, vector<double>> Final_prob;

    Node(){}

    Node(string names, vector<int>& arr, int size){
        vector<pair<int, double>> prob;
        int arr_size = arr.size();
        for(int i=0; i<arr_size; i++){
            double temp = (double)arr[i]/(double)size;
            if(temp!=0){
                probabilities.push_back(make_pair(i, temp));  
            }
        }
        Final_prob = {};
        name = names;
    }

    Node(string names,  map<vector<int>, vector<double>>& probs, int size){
        name = names;
        probabilities = {};
        Final_prob = probs;
    }
};


class BayesianNetwork {
public:
    vector<Node> nodes; // List of nodes
    unordered_map<string, vector<string>> edges;  // Adjacency list to represent edges

    void addNode(Node node){
        nodes.push_back(node);
    }

    void addEdge(const string& parent, const string& child){
        edges[parent].push_back(child);
    }

    void variable_elimination(map<vector<int>, vector<double>>& CPT, const unordered_map<string, int>& observedEvidence){
        for (const auto& evidence : observedEvidence) {
            string nodeName = evidence.first;
            int observedValue = evidence.second;

            auto itr = CPT.begin();
            while (itr != CPT.end()) {
                const vector<int>& key = itr->first;

                if (nodeName == "OC") {
                    // special case for "OC" node
                    int ocValue = key[0];
                    vector<double> ocProbabilities = itr->second;

                    // if OC value matches observed value vector
                    if (ocProbabilities[observedValue - 1] == 0) {
                        itr = CPT.erase(itr);
                    } else {
                        ++itr;
                    }
                } else {
                    // handle nodes based on their positions in the key vector
                    int nodePosition;
                    if (nodeName == "B") nodePosition = 0;
                    else if (nodeName == "M") nodePosition = 1;
                    else if (nodeName == "D") nodePosition = 2;
                    else if (nodeName == "P") nodePosition = 3;
                    else if (nodeName == "LB") nodePosition = 4;
                    else if (nodeName == "S") nodePosition = 5;

                    // check if the observed value matches the value at the specific position in the key vector
                    if (key[nodePosition] != observedValue) {
                        itr = CPT.erase(itr);
                    } else {
                        ++itr;
                    }
                }
            }
        }
    }

    unordered_map<int, double> inference(const string& queryVariable, const unordered_map<string, int>& observedEvidence){
        unordered_map<int, double> ans;
        map<vector<int>, vector<double>> CPT;
        Node temp;
        for (const Node& node : nodes) {
            if(node.name == "OC"){
                CPT = node.Final_prob; // get CPT
            }
            if(node.name == queryVariable){
                temp = node; // get query node
            }
        }
        variable_elimination(CPT, observedEvidence);

        if(temp.name == "OC"){ // special case of OC
            int a=0, b=0, c=0, d=0;
            for (auto itr = CPT.begin(); itr != CPT.end(); ++itr){
                if(itr->second[0]==1){
                    a++;
                }
                else if(itr->second[1]==1){
                    b++;
                }
                else if(itr->second[2]==1){
                    c++;
                }
                else{
                    d++;
                }
            }
            int total = a+b+c+d;
            ans[1] = (double)a/(double)total;
            ans[2] = (double)b/(double)total;
            ans[3] = (double)c/(double)total;
            ans[4] = (double)d/(double)total;
        }
        else{ // case for other nodes
            unordered_map<int, int> nums;
            int total=0;
            for(auto itr = temp.probabilities.begin(); itr!=temp.probabilities.end(); ++itr){
                nums[itr->first] = 0;
            }
            for (auto itr = CPT.begin(); itr != CPT.end(); ++itr){
                if(temp.name == "B"){
                    nums[itr->first[0]] +=1;
                }
                else if(temp.name == "M"){
                    nums[itr->first[1]] +=1;
                }
                else if(temp.name == "D"){
                    nums[itr->first[2]] +=1;
                }
                else if(temp.name == "P"){
                    nums[itr->first[3]] +=1;
                }
                else if(temp.name == "LB"){
                    nums[itr->first[4]] +=1;
                }
                else{
                    nums[itr->first[5]] +=1;
                }
                total++;
            }
            for(auto itr = nums.begin(); itr!=nums.end(); ++itr){
                ans[itr->first] = (double)itr->second / (double)total;
            }
        }
        return ans;
    }

};

int readCSV(string filename, vector<int>& buying, vector<int>& maint, vector<int>& doors, vector<int>& persons, vector<int>& lug_boot, vector<int>& safety, map<vector<int>, vector<double>>& overall_class){
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        int i=0;
        while (getline(file, line)) {
            stringstream ss(line);
            vector<string> tokens;
            string token;
            while (getline(ss, token, ',')) {
                tokens.push_back(token);
            }
            if(i>0){ // first line is left because it contains headings
                buying[stoi(tokens[0])] ++;
                maint[stoi(tokens[1])] ++;
                doors[stoi(tokens[2])] ++;
                persons[stoi(tokens[3])] ++;
                lug_boot[stoi(tokens[4])] ++;
                safety[stoi(tokens[5])] ++;
                if(stoi(tokens[6])==1){
                    overall_class[{stoi(tokens[0]), stoi(tokens[1]), stoi(tokens[2]), stoi(tokens[3]), stoi(tokens[4]), stoi(tokens[5])}] = {1.0, 0.0, 0.0, 0.0}; //[B, M, D, P, LB, S] -> {1,2,3,4}(OC)
                }
                else if(stoi(tokens[6])==2){
                    overall_class[{stoi(tokens[0]), stoi(tokens[1]), stoi(tokens[2]), stoi(tokens[3]), stoi(tokens[4]), stoi(tokens[5])}] = {0.0, 1.0, 0.0, 0.0};
                }
                else if(stoi(tokens[6])==3){
                    overall_class[{stoi(tokens[0]), stoi(tokens[1]), stoi(tokens[2]), stoi(tokens[3]), stoi(tokens[4]), stoi(tokens[5])}] = {0.0, 0.0, 1.0, 0.0};
                }
                else{
                    overall_class[{stoi(tokens[0]), stoi(tokens[1]), stoi(tokens[2]), stoi(tokens[3]), stoi(tokens[4]), stoi(tokens[5])}] = {0.0, 0.0, 0.0, 1.0};
                }
            }
            i++;
        }
        file.close();
        return i-1;
    }
    else {
        cout << "Error: Unable to open file!" << endl;
        return 0;
    }
}


int main() {
    BayesianNetwork network;
    vector<int> buying = {0,0,0,0,0}; // Price rating values: 1, 2, 3, 4
    vector<int> maint = {0,0,0,0,0}; // Maintenance rating values: 1, 2, 3, 4
    vector<int> doors = {0,0,0,0}; // Number of Doors values: 1, 2, 3
    vector<int> persons = {0,0,0,0,0}; // Number of Persons values: 2, 4
    vector<int> lug_boot = {0,0,0,0}; // Luggage-Boot rating values: 1, 2, 3
    vector<int> safety = {0,0,0,0}; // Safety rating values: 1, 2, 3
    map<vector<int>, vector<double>> overall_class; // Overall class rating values 1, 2, 3, 4
    
    int total = readCSV("bbn_car_data.csv", buying, maint, doors, persons, lug_boot, safety, overall_class);
    // replace "bbn_car_data.csv" with data file path
    
    Node B("B", buying, total);
    Node M("M", maint, total);
    Node D("D", doors, total);
    Node P("P", persons, total);
    Node LB("LB", lug_boot, total);
    Node S("S", safety, total);
    Node OC("OC", overall_class, total);

    network.addNode(B);
    network.addNode(M);
    network.addNode(D);
    network.addNode(P);
    network.addNode(LB);
    network.addNode(S);
    network.addNode(OC);

    network.addEdge("B", "OC");
    network.addEdge("M", "OC");
    network.addEdge("D", "OC");
    network.addEdge("P", "OC");
    network.addEdge("LB", "OC");
    network.addEdge("S", "OC"); // OC is child node of every other node
    
    /* queryVariables can be nodes:
    B, M, D, P, LB, S, OC
    */
    string queryVariable = "OC";  // Example query variables

    /* observedEvidences can be:
    B: 1, 2, 3, 4
    M: 1, 2, 3, 4
    D: 2, 3, 4
    P: 2, 4
    LB: 1, 2, 3
    S: 1, 2, 3
    OC: 1, 2, 3, 4
    */
    unordered_map<string, int> observedEvidence = {{"B", 1}, {"D", 2}};  // Example observed evidence
    // add upto one observed evidence for each node
    // number of nodes with evidence can be 0 or greater
    
    unordered_map<int, double> inferenceResult = network.inference(queryVariable, observedEvidence);

    for (const auto& pair : inferenceResult) {
        cout << "Probability of " << pair.first << " in " <<queryVariable<< ": "<< fixed << setprecision(6)  << pair.second << endl;
    }
    return 0;
}
