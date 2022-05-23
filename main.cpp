#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include "AST.h"
using namespace std;

class stackNode {
public:
    int depth;
    string data;
    stackNode *link;

    void push(int depth, string data) {
        stackNode *new_node = new stackNode;
        new_node->depth = depth;
        new_node->data = data;
        new_node->link = link;
        link = new_node;
    }

    string pop() {
        string data = link->data;
        int depth = link ->depth;
        stackNode *temp = link;
        link = link->link;
        delete temp;
        string result = to_string(depth) + " " + data;
        return result;
    }
};


void AST_parser (char* file1);
void read_cli (char* file1);
void extract_node_info();

int main (int argc, char* argv[]) {

    Graph* AST = new Graph();
    AST_parser(argv[1], AST);
    

    return 0;
}


void AST_parser (char* file1, Graph* graph) {
    read_cli(file1);
    extract_node_info();
    create_tree (graph);
}

void create_tree (Graph *graph) {
    ifstream indata; 
    string line; 
    indata.open("ast_abstracted.txt");
    if(!indata) { 
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }
    int start = 0;
    while (getline (indata, line)) {
        //To Do: use stack to do the graph insert edge
        start++;
        if (start == 1) continue;
        
        
    
    } 
    indata.close();
}

void read_cli (char* file1) {
    FILE*  fp;
    FILE* fp2;
    const int sizebuf= 1234;
    char buff[sizebuf];
    char cmd[sizebuf];  
    strcpy(cmd,"clang -Xclang -ast-dump -fsyntax-only ");
    strcat(cmd, file1);
    fp2 = fopen("ast.txt","w");
    cout << cmd << endl;
    if ((fp = popen (cmd,"r"))!= NULL){
        while (fgets(buff, sizeof (buff), fp)) {     
            fprintf(fp2, "%s",buff);
        }
    }
    pclose(fp);
}

void extract_node_info() {
    ifstream indata; 
    string line; 
    indata.open("ast.txt");
    string filePath = "ast_abstracted.txt";
    ofstream writeFile(filePath.data()); 
    if(!indata) { 
      cerr << "Error: file could not be opened" << endl;
      exit(1);
    }
    while (getline (indata, line)) {
        int i = 0;
        while (line[i] < 65 || line[i] > 90) {
            i++;
        }
        int j = i;
        while (line[j] != '[') {
            j++;
        }
        string node_name = line.substr(i-1, j-i);
        node_name[0] = ' ';

        cout << i << node_name << endl;
        
        if( writeFile.is_open() ){
            writeFile << i;
            writeFile << node_name << endl;
            
        }

    
    }
    writeFile.close();   
    indata.close();

        // write File
        

    
   

}