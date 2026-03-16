/*
    Description: Spellchecks an input file given a dictionary file
    Input: Dictionary file of valid words, and a file to spellcheck
    Output: Suggested words for detected spelling errors
*/

#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

int main(int args, char * argv[])
{
    vector<string> d; // this will hold our valid words
    ifstream dF(argv[1]); // open dict file
    string s;
    while(dF >> s){ // loop until end of file
        for(int i = 0; i < s.length(); i++) // make string uppercase
            s.replace(i, 1, string(1, toupper(s[i])));
        d.push_back(s); // valid words that are in the dictionary will be true
    }
    dF.close(); // dictionary complete

    ifstream iF(argv[2]); // open input file
    vector<vector<string>> totalSuggestions; // contains the vectors that have the suggestions, in the order of the checked words
    vector<string> checked; // stores the words i have already checkded, in the order they were checked
    while(iF >> s){
        for(int i = 0; i < s.length(); i++) // make string uppercase
            s.replace(i, 1, string(1, toupper(s[i])));
        if ((find(d.begin(), d.end(), s) == d.end()) && (find(checked.begin(), checked.end(), s) == checked.end())){ // check if not in dict and not in the words that have been checked
            vector<string> suggestions; // holds all the words that might be what the user meant
            for(int i = 0; i < s.length()+1; i++){
                for(int j = 65; j < 91; j++){
                    string str = s;
                    str.insert(str.begin()+i, static_cast<char>(j));
                    if (find(d.begin(), d.end(), str) != d.end())
                        suggestions.push_back(str); // if in dict, add to suggestions
                }
            }
            for(int i = 0; i < s.length(); i++){
                for(int j = 65; j < 91; j++){
                    string str = s;
                    str.replace(i, 1, string(1, static_cast<char>(j)));
                    if (find(d.begin(), d.end(), str) != d.end())
                        suggestions.push_back(str); // if in dict, add to suggestions
                }
            }
            sort(suggestions.begin(), suggestions.end()); // make it nice for codegrade
            totalSuggestions.push_back(suggestions);
            checked.push_back(s);
        }
    }

    for (int i = 0; i < checked.size(); i++){ // final output
        cout << checked[i] << " is misspelled\n\n";
        if(totalSuggestions[i].size() == 0){
            cout << "No suggestions\n";
        }
        else{
            cout << "Suggested words\n";
            for(string suggestion : totalSuggestions[i]){
                cout << suggestion << endl;
            }
        }
        cout << endl;
    }

    return 0;
}   