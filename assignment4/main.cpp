#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
int main() {
    string temp = "", countryStr = "", buildStr = "", destroyStr = "";
    getline(cin, temp);

    for (int i = 0; i < 3; i++) {
        
    }

    // Create map of letters & their weights
    map<char, int> weights;
    for (int i = 0; i < (int)letters.length(); i++) {
        weights[letters[i]] = i;
        cout << letters[i] << ":\t" << i << endl;
    }

    return 0;
}