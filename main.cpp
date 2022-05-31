#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <vector>
#include <stack>
#include <cmath>
#include "AST.h"

using namespace std;

string* idx[100000000];
string lcs = "";

string split(string input, char delimiter) {
    vector<string> result;
    stringstream ss(input);
    string tmp;
 
    while (getline(ss, tmp, delimiter)) {
        return tmp;
    }
    return tmp;
}

int countNumOfChars(string input, char delimiter) {
    int count = 0;
    for (int i = 0; i < input.length(); i++) {
        if (input[i] == delimiter) {
            count++;
        }
    }
    return count;
}

string create_tree (Graph *graph, int line_num) {
    ifstream indata; 
    string line; 
    indata.open("ast_abstracted.txt");
    if(!indata) { 
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }

    vector<pair<int, int> > space_info; // idx, # of spaces
    vector<pair<int, int> > parent_info; // idx, parent idx
    int start = 1;
    getline(indata, line);
    string root = split(line, char('\n'));
    space_info.push_back(make_pair(0, 0));
    parent_info.push_back(make_pair(0, 0));
    int known_space = 0;
    while (getline (indata, line)) {
        line = split(line, char('\n'));
        int num_of_spaces = countNumOfChars(line, char(' '));
        string src_line = line;
        if (num_of_spaces > 0) src_line.erase(src_line.find(" "), num_of_spaces);
        if (num_of_spaces >= known_space) {
            graph->InsertEdge(start-1, start, src_line);
            space_info.push_back(make_pair(start, num_of_spaces));
            parent_info.push_back(make_pair(start, start-1));
            known_space = num_of_spaces;
        }
        else if (num_of_spaces < known_space) {
            while(space_info.back().second > num_of_spaces) {
                space_info.pop_back();
            }
            while(space_info.back().first < parent_info.back().first) {
                parent_info.pop_back();
            }
            graph->InsertEdge(parent_info.back().second, start, src_line);
            space_info.push_back(make_pair(start, num_of_spaces));
            parent_info.push_back(make_pair(start, parent_info.back().second));
            known_space = num_of_spaces;
        }
        start++;
    } 
    indata.close();
    return root;
}

int extract_node_info(char* file1) {
    ifstream indata; 
    string line; 
    indata.open(file1);
    string filePath = "ast_abstracted.txt";
    ofstream writeFile(filePath.data()); 
    if(!indata) { 
      cerr << "Error: file could not be opened" << endl;
      exit(1);
    }
    int line_num=0;
    getline(indata, line);
    string root = split(line, ' ');
    writeFile << root << endl;
    idx[line_num] = new string;
    *idx[line_num++] = root;
    while (getline (indata, line)) {
        string result = line;
        result.erase(result.find(" 0"), result.length());
        result.replace(line.find("\\"), 1, "");
        if( writeFile.is_open() ){
            writeFile << result << endl; 
        }
        idx[line_num] = new string;
        *idx[line_num] = result;
    }
    writeFile.close();   
    indata.close();
    return line_num;
}

string AST_parser (char* file1, Graph* graph) {
    int line_num = extract_node_info(file1);
    string root = create_tree (graph, line_num);
    return root;
}

void printLcs(char **direction, string firstString, int i, int j) {
    if(i==0 || j==0){
        return;
    }
    if(direction[i-1][j-1]=='\\') {
        printLcs(direction, firstString, i-1, j-1);
        lcs += firstString[i-1];
    }
    else if(direction[i-1][j-1]=='|') {
        printLcs(direction, firstString, i-1, j);
    }
    else if(direction[i-1][j-1]=='-') {
        printLcs(direction, firstString, i, j-1);
    }
    return;
}

void LCSFinder(string firstString, string secondString){
    int firstStringLength = firstString.length(),
        secondStringLength = secondString.length();
    //cout << firstStringLength << " " << secondStringLength;

    int index[firstStringLength+1][secondStringLength+1];

    char **direction;
    direction = new char* [firstStringLength];
    for(int i=0; i<firstStringLength; i++)
        direction[i] = new char [secondStringLength];

    for(int i=0; i<firstStringLength+1; i++){
        index[i][0]=0;
    }
    for(int i=0; i<secondStringLength+1; i++){
        index[0][i]=0;
    }

    for(int i=1; i<=firstStringLength; i++) {
        for(int j=1; j<=secondStringLength; j++) {
            if(firstString[i-1]==secondString[j-1]) {
                index[i][j]=index[i-1][j-1]+1;
                direction[i-1][j-1]='\\';
            }
            else if(index[i-1][j]>=index[i][j-1]) {
                index[i][j]=index[i-1][j];
                direction[i-1][j-1]='|';
            }
            else {
                index[i][j]=index[i][j-1];
                direction[i-1][j-1]='-';
            }
        }
    }
    //print
    printLcs(direction, firstString, firstStringLength, secondStringLength);
    return;
}


int main (int argc, char* argv[]) {
    Graph* AST1 = new Graph();
    string root1 = AST_parser(argv[1], AST1);
    AST1->PrintAdjList();
    AST1->set_root(root1);
    string bfs1 = AST1->BFS_iter(0);

    Graph* AST2 = new Graph();
    string root2 = AST_parser(argv[2], AST2);
    AST2->PrintAdjList();
    AST2->set_root(root2);
    string bfs2 = AST2->BFS_iter(0);

    // cout << bfs1 << endl;
    // cout << bfs2 << endl;
    LCSFinder(bfs1.data(), bfs2.data());
    double similarity = (double)lcs.length()/(double)bfs1.length();
    cout << "similarity: " << round(similarity * 100) << "%"<< endl;
    return 0;
}