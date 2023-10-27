#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

const string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

// Function to turn the input strings into the vectors
vector<vector<string>> make2D(string str) {
    vector<vector<string>> v;
    int n = 1;

    // Find number of cities, which is number of ',' +1.
    for (int i = 0; i < (int)str.length(); i++) {
        if (str[i] == ',') {
            n++;
        }
    }

    // Separate the elements
    int start = 0;
    for (int i = 0; i < n; i++) {
        vector<string> temp;
        for (int j = start; j < (int)str.length(); j++) {
            if (str[j] == ',') {
                start = j + 1;
                break;
            }
            temp.push_back(str.substr(j, 1));
        }
        v.push_back(temp);
    }

    return v;
};

vector<pair<int, int>> findExistingEdges(vector<vector<string>> country) {
    vector<pair<int, int>> edges;

    for (int row = 0; row < (int)country.size(); row++) {
        for (int col = row; col < (int)country[row].size(); col++) {
            if (country[row][col] == "1") {
                edges.push_back(make_pair(row, col));
            }
        }
    }

    return edges;
};

vector<pair<int, int>> findPossibleBuilds(vector<vector<string>> country) {
    vector<pair<int, int>> edges;

    for (int row = 0; row < (int)country.size(); row++) {
        for (int col = row; col < (int)country[row].size(); col++) {
            if (country[row][col] == "0" && row != col) {
                edges.push_back(make_pair(row, col));
            }
        }
    }

    return edges;
}

vector<vector<string>> weightEdges(vector<vector<string>> v) {
    // Create map of letters & their weights
    map<string, int> weights;
    for (int i = 0; i < (int)letters.length(); i++) {
        weights[letters.substr(i, 1)] = i;
        // cout << letters[i] << ":\t" << i << endl;
    }

    for (int i = 0; i < (int)v.size(); i++) {
        for (int j = i; j < (int)v.size(); j++) {
            v[i][j] = to_string(weights[v[i][j]]);
            v[j][i] = v[i][j];
        }
    }

    return v;
};

int findParent(map<int, int> parents, int city) {
    if (parents[city] == city) {
        return city;
    }
    return findParent(parents, parents[city]);
};

int main() {
    string temp = "", countryStr = "", buildStr = "", destroyStr = "";
    getline(cin, temp);

    // Split the temp string into countryStr, buildStr and destroyStr, based on the spaces
    countryStr = temp.substr(0, temp.find(" "));
    buildStr = temp.substr(temp.find(" ") + 1, temp.find(" ", temp.find(" ") + 1) - temp.find(" ") - 1);
    destroyStr = temp.substr(temp.find(" ", temp.find(" ") + 1) + 1);

    // Build the vectors country, build & destroy
    vector<vector<string>> country = make2D(countryStr);
    vector<vector<string>> build = make2D(buildStr);
    vector<vector<string>> destroy = make2D(destroyStr);

    // Extract all the edges in a country country
    vector<pair<int, int>> allEdges = findExistingEdges(country);

    // Create a map of cities and their parents.
    // e.g. "0" will have "parent = parents[0]"
    map<int, int> parents;
    // Initially, each city's parent is itself
    for (int i = 0; i < (int)country.size(); i++) {
        parents[i] = i;
    }

    // Convert Destroy and Build's characters into their integer values.
    destroy = weightEdges(destroy);  //  2D string vector for destruction costs
    build = weightEdges(build);      //  2D string vector for building costs

    // Create an edge list
    vector<pair<int, pair<int, int>>> edgeWeights;
    for (int i = 0; i < (int)allEdges.size(); i++) {
        edgeWeights.push_back(make_pair(stoi(destroy[allEdges[i].first][allEdges[i].second]), allEdges[i]));
    }
    sort(edgeWeights.begin(), edgeWeights.end());

    // Find max spanning tree to remove the most expensive edges
    reverse(edgeWeights.begin(), edgeWeights.end());
    vector<int> visited;
    vector<int> indexesToDestroy;
    // Kruskal's max spanning tree
    for (int i = 0; i < (int)edgeWeights.size(); i++) {
        int a = edgeWeights[i].second.first;
        int b = edgeWeights[i].second.second;

        int aParent = findParent(parents, a);
        int bParent = findParent(parents, b);

        if (aParent != bParent) {
            parents[a] = bParent;
            indexesToDestroy.push_back(i);
        }

        if (find(visited.begin(), visited.end(), a) == visited.end()) {
            visited.push_back(a);
        }
        if (find(visited.begin(), visited.end(), b) == visited.end()) {
            visited.push_back(b);
        }

        // Check if visited size is equal to number of cities == country.size()
        if (visited.size() == country.size()) {
            break;
        }
    }

    // Add up to costs of the remaining edges.
    int destructionCost = 0;
    for (int i = 0; i < (int)edgeWeights.size(); i++) {
        if (find(indexesToDestroy.begin(), indexesToDestroy.end(), i) == indexesToDestroy.end()) {
            destructionCost += edgeWeights[i].first;
        }
    }

    // // Create a vector of strings, containing groups, by their respective parents.
    // map<int, vector<int>> groups;
    // for (int i = 0; i < (int)parents.size(); i++) {
    //     groups[parents[i]].push_back(i);
    // }

    // clear edgeWeights & visited to contain build costs now
    edgeWeights.clear();
    visited.clear();

    // Fill edgeWeights with the weights of roads that might be built. This excludes the weights of existing roads
    vector<pair<int, int>> possibleBuilds = findPossibleBuilds(country);
    for (int i = 0; i < (int)possibleBuilds.size(); i++) {
        edgeWeights.push_back(make_pair(stoi(build[possibleBuilds[i].first][possibleBuilds[i].second]), possibleBuilds[i]));
    }
    sort(edgeWeights.begin(), edgeWeights.end());

    // Kruskal's min spanning tree (no need to reverse this time)
    vector<int> indexesToBuild;
    for (int i = 0; i < (int)edgeWeights.size(); i++) {
        int a = edgeWeights[i].second.first;
        int b = edgeWeights[i].second.second;

        int aParent = findParent(parents, a);
        int bParent = findParent(parents, b);

        if (aParent != bParent) {
            // Set all cities with aParent to be bParent
            for (int j = 0; j < (int)parents.size(); j++) {
                if (parents[j] == aParent) {
                    parents[j] = bParent;
                }
            }
            indexesToBuild.push_back(i);
        }

        if (find(visited.begin(), visited.end(), a) == visited.end()) {
            visited.push_back(a);
        }
        if (find(visited.begin(), visited.end(), b) == visited.end()) {
            visited.push_back(b);
        }

        // Check if visited size is equal to number of cities == country.size()
        if (visited.size() == country.size()) {
            break;
        }
    }

    // Add up build costs
    int buildCost = 0;
    for (int i = 0; i < (int)edgeWeights.size(); i++) {
        if (find(indexesToBuild.begin(), indexesToBuild.end(), i) != indexesToBuild.end()) {
            buildCost += edgeWeights[i].first;
        }
    }

    int cost = destructionCost + buildCost;  // Sum the costs
    cout << cost << endl;

    return 0;
}