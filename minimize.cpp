#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

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





// vector<string> findRightEnd(string str, int k, int l){
//     string rightEnd = "";
//     for(int i = l; i < k; i++) rightEnd += findMinimizer(str.substr(i, l - i), k);
//     return rightEnd;
// }

// vector<string> removeDups(vector<string> minimizers){
//     vector<string> uniqueMinimizers;
//     for(string minimizer: minimizers){
//         size_t size = uniqueMinimizers.size();
//         if(size == 0 || uniqueMinimizers[size - 1] != minimizer)
//             uniqueMinimizers.push_back(minimizer);
//     }
//     return uniqueMinimizers;
// }

int main(int argc, char** argv){
    string inputFileName = argv[1];
    int w = stoi(argv[2]);
    int k = stoi(argv[3]);
    int l = w + k - 1;

    //read genome file into single string
    ifstream input(inputFileName);
    string genome, line;
    while (getline(input, line)) genome += line;

    cout << "Extracting minimizers from '" << inputFileName << "' for k=" << k << " and w=" << w << endl;

    // vector<string> minimizers;
    // for(string str: findLeftEnd(genome.substr(0, l), k, l)) 
    //     minimizers.push_back(str);

    // for(int i = 0; i < genome.size() - l + 1; i++)
    //     minimizers.push_back(findMinimizer(genome.substr(i, 1), k));



}