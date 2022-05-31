#include <iostream>
using namespace std; 
#include "queue.h"

#define MAX_VERTICES    1000000

typedef struct t_node {
    int vertex;
    string data;
    struct t_node *link;
} node; 

class Graph {
  private: 
    node *graph[MAX_VERTICES];      // Adjacency list
    bool visited[MAX_VERTICES];     // Visited vertex
    int num_valid_vertex;           // Number of vertices currently in use  
    string root;                    // Root of the tree
    
  public: 
    Graph();
    void set_root(string root);
    bool IsEmpty();
    void InsertEdge(int src, int dest, string data) ;
    string BFS_iter(int v);           // Iterative BFS 
    void PrintAdjList();            // Print all adjacency list
    int  GetNumVertices() { return num_valid_vertex; };
}; 


Graph::Graph() {
    num_valid_vertex = 0;
    for (int i = 0; i < MAX_VERTICES; i++) {
        graph[i] = NULL;
        visited[i] = false;
    }
}

void Graph::set_root(string root) {
    this->root = root;
}

bool Graph::IsEmpty() {
    return (num_valid_vertex == 0);
}

// TODO: Insert an edge between vertex src and vertex dest
void Graph::InsertEdge(int src, int dst, string data) {
    node* src_node = new node;
    src_node->vertex = dst;
    src_node->data = data;
    
    if(!graph[src]) num_valid_vertex++;
    if(!graph[dst]) num_valid_vertex++;
    src_node->link = graph[src];
    graph[src] = src_node;    
}


// iterative BFS algorithm
string Graph::BFS_iter(int v) {
    string bfs = root + " ";
    for (int i = 0; i < MAX_VERTICES; i++){
        visited[i] = false;
    }
    Queue graphQ;
    node* w;
    visited[v] = true;
    graphQ.AddQ(v);
    while (!graphQ.IsEmpty()) {
        v = graphQ.DeleteQ();
        for (w = graph[v]; w; w = w->link) {
            if(!visited[w->vertex]) {
                bfs += w->data + " ";
                visited[w->vertex] = true;
                graphQ.AddQ(w->vertex);
            }
        }
    }
    return bfs;
}

// Print all adjacency lists linked by graph[]
void Graph::PrintAdjList() {
    for(int i = 0;i < num_valid_vertex; i++) {
        if (!graph[i]) continue;
        cout<<"graph["<<i<<"]: ";
        for(node* cur = graph[i];cur;cur = cur->link) {
            cout<<cur->vertex<<"(" << cur->data <<")" <<" ";
            if(cur->link)
                cout<<" -> ";
        }
        cout<<endl;
    }
    cout << endl;
}