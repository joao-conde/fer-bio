#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

/**
 * Sort minimizers based on descending order of their frequency.
 */
struct SortByFrequency {
    inline bool operator() (pair<string, set<int>> &minimizer1, pair<string, set<int>> &minimizer2){
        return minimizer1.second.size() > minimizer2.second.size();
    }
};

/**
 * Finds the minimizer for a given string from all the possible k-mers.
 *
 * Uses lexicographical order to pick the lowest k-mer.
 *
 * @param str The string to search for the minimizer.
 * @param k The k-mer size.
 * @return Minimizer and its position.
 */
pair<string, int> findMinimizer(string str, int k){
    vector<pair<string, int>> tmp;
    for(int i = 0; i < str.size() - k + 1; i++) tmp.push_back(make_pair(str.substr(i, k), i));
    sort(tmp.begin(), tmp.end());
    return tmp[0];
}

/**
 * Finds the left end (or start of the genome) minimizers.
 *
 * @param str The begining of the genome.
 * @param k The k-mer size.
 * @param l The maximum consecutive k-mers.
 * @return Minimizers and their positions.
 */
vector<pair<string, int>> findLeftEnd(string str, int k, int l){
    vector<pair<string,int>> leftEnd;
    for(int i = k; i < l; i++) leftEnd.push_back(findMinimizer(str.substr(0, i), k));
    return leftEnd;
}

/**
 * Finds the right end (or end of the genome) minimizers.
 *
 * @param str The end of the genome.
 * @param k The k-mer size.
 * @param l The maximum consecutive k-mers.
 * @return Minimizers and their positions.
 */
vector<pair<string,int>> findRightEnd(string str, int k, int l){
    vector<pair<string,int>> rightEnd;
    for(int i = 0; i < l - k; i++){
        pair<string, int> minimizerPos = findMinimizer(str.substr(i, l - i), k);
        minimizerPos.second += i; // adjust considering substring starts i positions after
        rightEnd.push_back(minimizerPos);
    }
    return rightEnd;
}

int main(int argc, char** argv){
    string inputFileName = argv[1], outputFileName = argv[2];
    int w = stoi(argv[3]);   // number of k-mers per window
    int k = stoi(argv[4]);   // k-mer size
    int l = w + k - 1;       // maximum length (letters) for consecutive k-mers
    float f = stof(argv[5]); // top most frequent minimizers to remove

    ifstream input(inputFileName);
    string genome = "", line;
    while (getline(input, line)) genome += line;
    input.close();

    cout << "Extracting minimizers from '" << inputFileName << "' for k=" << k << " and w=" << w << endl;
    cout << "Excluding top " << f << " frequent minimizers" << endl;

    map<string, set<int>> minimizerPositions;

    // find left end minimizers
    for(pair<string, int> minimizerPos: findLeftEnd(genome.substr(0, l), k, l)){
        if(minimizerPositions.find(minimizerPos.first) != minimizerPositions.end())
            minimizerPositions[minimizerPos.first].insert(minimizerPos.second);
        else
            minimizerPositions[minimizerPos.first] = {minimizerPos.second};
    }

    // find interior minimizers
    for(int i = 0; i < genome.size() - l + 1; i++){
        pair<string, int> minimizerPos = findMinimizer(genome.substr(i, l), k);
        minimizerPos.second += i; // adjust considering substring starts i positions after
        if(minimizerPositions.find(minimizerPos.first) != minimizerPositions.end())
            minimizerPositions[minimizerPos.first].insert(minimizerPos.second);
        else
            minimizerPositions[minimizerPos.first] = {minimizerPos.second};
    }

    // find right end minimizers
    for(pair<string, int> minimizerPos: findRightEnd(genome.substr(genome.size() - l + 1, l + 1), k, l)) {
        minimizerPos.second += genome.size() - l + 1; // adjust considering substring starts genome.size() - l + 1 positions after
        if(minimizerPositions.find(minimizerPos.first) != minimizerPositions.end())
            minimizerPositions[minimizerPos.first].insert(minimizerPos.second);
        else
            minimizerPositions[minimizerPos.first] = {minimizerPos.second};
    }
    
    // number of minimizers to discard (top f)
    int discard = ceil(minimizerPositions.size() * f);
    cout << "A total of " << minimizerPositions.size() << " minimizers were found." << endl;
    cout << "Discarding top " << f << " minimizers (roughly " << discard << " minimizers)" << endl;

    // order minimizers by descending frequency
    vector<pair<string, set<int>>> minimizerPosSortedByFreq;
    transform(minimizerPositions.begin(), minimizerPositions.end(), back_inserter(minimizerPosSortedByFreq), [](const pair<string, set<int>> &entry){return entry;});
    sort(minimizerPosSortedByFreq.begin(), minimizerPosSortedByFreq.end(), SortByFrequency());

    // discard top frequency minimizers, write others to output file
    ofstream output(outputFileName);
    for(int i = 0; i < minimizerPosSortedByFreq.size(); i++){
        pair<string, set<int>> minimizerPos = minimizerPosSortedByFreq[i];
        if(i < discard){
            cout << "Discarding frequent minimizer '" << minimizerPos.first << "'" << endl;
            continue;
        }
        // write minimizer' positions
        output << "Minimizer '" << minimizerPos.first << "' positions in genome: ";
        for(int pos: minimizerPos.second) output << pos << " ";
        output << endl;
    }

    output.close();
    cout << "Done! Generated " << minimizerPositions.size() - discard << " minimizers" << endl;
}