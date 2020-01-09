#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

char findMinimizer(string str, int k){
    vector<char> tmp;
    for(int i = 0; i < str.size() - k + 1; i++)
        for(int j = 0; j < k; j++)
            tmp.push_back(str[i + j]);
    sort(tmp.begin(), tmp.end());
    return tmp[0];
}

string findLeftEnd(string str, int k, int l){
    string leftEnd = "";
    for(int i = k; i < l; i++) leftEnd += findMinimizer(str.substr(0, i), k);
    return leftEnd;
}

string findRightEnd(string str, int k, int l){
    string rightEnd = "";
    for(int i = l; i < k; i++) rightEnd += findMinimizer(str.substr(i, l - i), k);
    return rightEnd;
}



int main(int argc, char** argv){
    string inputFileName = argv[1];
    int w = stoi(argv[2]);
    int k = stoi(argv[3]);

    //read genome file into single string
    vector<string> minimizers;
    ifstream input(inputFileName);
    string genome, line;
    while (getline(input, line)) genome += line;

    cout << "Extracting minimizers from '" << inputFileName << "' for k=" << k << " and w=" << w << endl;


}