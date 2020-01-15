#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

using namespace std;

//todo positions
//discard top frequency
//default values for (k, w, f) should be (15, 5, 0.001)
string findMinimizer(string str, int k){
    vector<string> tmp;
    for(int i = 0; i < str.size() - k + 1; i++) tmp.push_back(str.substr(i, k));
    sort(tmp.begin(), tmp.end());
    return tmp[0];
}

vector<string> findLeftEnd(string str, int k, int l){
    vector<string> leftEnd;
    for(int i = k; i < l; i++) leftEnd.push_back(findMinimizer(str.substr(0, i), k));
    return leftEnd;
}

vector<string> findRightEnd(string str, int k, int l){
    vector<string> rightEnd;
    for(int i = 0; i < l - k; i++) rightEnd.push_back(findMinimizer(str.substr(i, l - i), k));
    return rightEnd;
}

int main(int argc, char** argv){
    string inputFileName = argv[1], outputFileName = argv[2];
    int w = stoi(argv[3]);
    int k = stoi(argv[4]);
    int l = w + k - 1;

    //read genome file into single string
    ifstream input(inputFileName);
    string genome = "", line;
    while (getline(input, line)) genome += line;
    input.close();

    cout << "Extracting minimizers from '" << inputFileName << "' for k=" << k << " and w=" << w << endl;

    map<string, int> minimizerFrequencies;

    //left minimizers
    for(string str: findLeftEnd(genome.substr(0, l), k, l)){
        if(minimizerFrequencies.find(str) != minimizerFrequencies.end())
            minimizerFrequencies[str]++;
        else
            minimizerFrequencies[str] = 0;
    }

    //interior minimizers
    for(int i = 0; i < genome.size() - l + 1; i++){
        string str = findMinimizer(genome.substr(i, l), k);
        if(minimizerFrequencies.find(str) != minimizerFrequencies.end())
            minimizerFrequencies[str]++;
        else
            minimizerFrequencies[str] = 0;
    }

    //right minimizers
    for(string str: findRightEnd(genome.substr(genome.size() - l + 1, l + 1), k, l)) {
        if(minimizerFrequencies.find(str) != minimizerFrequencies.end())
            minimizerFrequencies[str]++;
        else
            minimizerFrequencies[str] = 0;
    }

    cout << "Writing minimizers to '" << outputFileName << "'" << endl;
    ofstream output(outputFileName);
    for(auto minFreq: minimizerFrequencies) output << minFreq.first << " " << minFreq.second << endl;
    output.close();

    cout << "Done!" << endl;
}