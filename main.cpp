#include <vector>
#include <iostream>
#include <climits>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <sstream>
#include <string>
#include <cstring>
#include <chrono>
#include <fstream>
#include <random>
#include <queue>

#include "headers/binheap.hpp"
#include "headers/Graph.hpp"
#include "headers/fibonacciheap.hpp"

#define inf INT_MAX

using namespace std;

int read_problem(stringstream &s)
{
    string throaway;
    int n;
    s >> throaway;
    s >> n;
    return n;
}

void read_source(int &a,stringstream& s)
{
    string desc;
    int aux;
    s >> aux;
    s >> desc;
    if (desc == "s")
    {
        a = aux-1;
    }
}

void add_edge(vector<adjacency_row> &adjacency, vector<edge> &edges, stringstream& s)
{
    int u, v, cost;
    s >> u;
    s >> v;
    s >> cost;
    //para que os vertices comecem em 0 e terminem em n-1
    u = u - 1;
    v = v - 1;
    //excluindo arestas "que n�o existem"
    if (cost == 0)
        return;
    //n�o direcionado
    edge e1 = { u,v,cost };
    edge e2 = { v,u,cost };
    if (!adjacency[u].exists(v) && !adjacency[v].exists(u))
    {
        adjacency[u].push_back(e1);
        adjacency[v].push_back(e2);
        edges.push_back(e1);
    }
}

vector<adjacency_row> read_input(int &n,int &a,string file_path, vector<edge> &edges)
{
    n = 0;
    string aux;
    ifstream file(file_path);
    string word;
    vector<adjacency_row> adjacency;
    while (getline(file, aux))
    {
        stringstream s(aux);
        s >> word;
        //comment
        if (word == "c")
        {
            continue;
        }
        //problem descriptor
        else if(word == "p")
        {
            n=read_problem(s);
            adjacency.resize(n);
        }
        //source and sink
        else if(word == "n")
        {
            read_source(a,s);
        }
        //edge attributes
        else if (word == "a")
        {
            add_edge(adjacency, edges, s);
        }
    }
    return adjacency;
}



pair<int,pair<int,int>> MinCutPhase_binaryheap(vector<adjacency_row> &adjacency,vector<int> &vertices,int a,int n)
{
    vector<int> A;
    int v_size = vertices.size();
    bool* in_a = new bool[n]();
    int s=a,t=a,cut_cost;
    binary_heap costs;
    A.push_back(a);
    in_a[a] = true;
    //add most tightly connected vertices
    for (int i = 0; i < adjacency[a].size(); i++)
    {
        //ignore "removed" edges
        if(!adjacency[a][i].mark)
            costs.push(make_pair(adjacency[a][i].cost, adjacency[a][i].v));
    }

    while (A.size() < v_size)
    {
        pair<int, int> next = costs.extract_max();
        s = t;
        t = next.second;
        cut_cost = next.first;
        A.push_back(t);
        in_a[t] = true;
        //update costs
        for (int i = 0; i < adjacency[t].size(); i++)
        {
            if(!in_a[adjacency[t][i].v] && !adjacency[t][i].mark)
                costs.increase_key(adjacency[t][i].v, adjacency[t][i].cost);
        }
    }
    delete[] in_a;
    //merge s and t

    // update edges leaving s (O(V))
    for (int i = 0; i < adjacency[t].size(); i++)
    {
        if (adjacency[t][i].v == s)
        {
            int j = adjacency[s].index(t);
            adjacency[s][j].mark = true;
            continue;
        }
        int j = adjacency[s].index(adjacency[t][i].v);
        //if vertex v is adjacent to t and s, sum the weights
        if (j != -1)
        {
            adjacency[s][j].cost += adjacency[t][i].cost;
        }
        //else, add v to the adjacency of s
        else
        {
            edge aux = adjacency[t][i];
            aux.u = s;
            aux.v = adjacency[t][i].v;
            adjacency[s].push_back(aux);
        }
    }
    
    int b;
    //update edges leading to s (O(v))
    for (int i = 0; i < v_size; i++)
    {
        if (vertices[i] == s || vertices[i] == t)
        {
            continue;
        }
        adjacency[vertices[i]].merge(s, t);
    }



    vertices.erase(remove(vertices.begin(), vertices.end(), t),vertices.end());
    return make_pair(cut_cost,make_pair(s,t));
}

pair<int, pair<int, int>> MinCutPhase_fibonacciheap(vector<adjacency_row>& adjacency, vector<int>& vertices, int a,int n)
{
    vector<int> A;
    int v_size = vertices.size();
    bool* in_a = new bool[n]();
    int s = a, t = a, cut_cost;
    FibHeap<pair<int, int>> costs;
    unordered_map<int, FibHeap<pair<int,int>>::FibNode*> handles;
    A.push_back(a);
    in_a[a] = true;
    //add most tightly connected vertices
    for (int i = 0; i < adjacency[a].size(); i++)
    {
        //ignore "removed" edges
        if (!adjacency[a][i].mark)
            handles[adjacency[a][i].v] = costs.push(make_pair(adjacency[a][i].cost, adjacency[a][i].v));
    }
    while (A.size() < v_size)
    {
        pair<int, int> next = costs.top();
        costs.pop();
        s = t;
        t = next.second;
        handles.erase(t);

        cut_cost = next.first;
        A.push_back(t);
        in_a[t] = true;
        //update costs
        for (int i = 0; i < adjacency[t].size(); i++)
        {
            if (!in_a[adjacency[t][i].v] && !adjacency[t][i].mark)
            {
                //if vertex is not yet in heap, add it
                if (handles.find(adjacency[t][i].v) == handles.end()) {
                    handles[adjacency[t][i].v] = costs.push(make_pair(adjacency[t][i].cost, adjacency[t][i].v));
                }
                //else, increase cost
                else {
                    auto node = handles[adjacency[t][i].v];
                    costs.increase_key(node, make_pair(adjacency[t][i].cost+node->key.first, adjacency[t][i].v));
                }
            }
        }
    }
    delete[] in_a;
    //merge s and t

    // update edges leaving s (O(V))
    for (int i = 0; i < adjacency[t].size(); i++)
    {
        if (adjacency[t][i].v == s)
        {
            int j = adjacency[s].index(t);
            adjacency[s][j].mark = true;
            continue;
        }
        int j = adjacency[s].index(adjacency[t][i].v);
        //if vertex v is adjacent to t and s, sum the weights
        if (j != -1)
        {
            adjacency[s][j].cost += adjacency[t][i].cost;
        }
        //else, add v to the adjacency of s
        else
        {
            edge aux = adjacency[t][i];
            aux.u = s;
            aux.v = adjacency[t][i].v;
            adjacency[s].push_back(aux);
        }
    }


    //update edges leading to s (O(v))
    for (int i = 0; i < v_size; i++)
    {
        if (vertices[i] == s || vertices[i] == t)
        {
            continue;
        }
        adjacency[vertices[i]].merge(s, t);
    }




    vertices.erase(remove(vertices.begin(), vertices.end(), t), vertices.end());
    return make_pair(cut_cost, make_pair(s, t));
}

int Stoer_Wagner(vector<adjacency_row> adjacency, vector<int> vertices, int n, int a, bool fibonacci){
    int mincost = inf;
    pair <int, int> mincut;
    pair<int, pair<int, int>> cut;
    while (vertices.size() > 1){
        if(fibonacci)
             cut = MinCutPhase_fibonacciheap(adjacency, vertices, a, n);
        else
             cut = MinCutPhase_binaryheap(adjacency, vertices, a, n);
        int cost = cut.first;
        if (cost < mincost){
            mincost = cost;
            mincut = cut.second;
        }

    }
    return mincost;
}

int find(int i, int set[]){
    return set[i] == -1 ? i : find(set[i], set);
}

void join(int i1, int i2, int set[]){
    set[find(i1, set)] = find(i2, set);
}

//Kruskal's algorithm
vector<edge> find_mst(vector<edge> edges, unordered_set<int> v){
    vector<edge> random_edges = edges;
    for (auto &it : random_edges){
        it.cost = rand();
    }
    sort(random_edges.begin(), random_edges.end());

    int *sets = new int[v.size()];
    memset(sets, -1, sizeof(int) * v.size());

    vector<edge> mst;
    int u, v_;
    for (auto &it : random_edges){
        u = it.u;
        v_ = it.v;
        if (find(u, sets) != find(v_, sets)){
            mst.push_back(it);
            join(u, v_, sets);
        }
    }
    delete[] sets;
    return mst;
}

//BFS
unordered_set<int> find_vertices(int s, vector<edge> mst){
    queue<int> q;
    unordered_set<int> visited;
    q.push(s);
    visited.insert(s);
    while (!q.empty()){
        int v = q.front();
        q.pop();
        for (auto &it : mst){
            if (it.u == v){
                if(!visited.count(it.v)){
                    visited.insert(it.v);
                    q.push(it.v);
                }
            }else if(it.v == v){
                if(!visited.count(it.u)){
                    visited.insert(it.u);
                    q.push(it.u);
                }
            }
        }
    }
    return visited;
}

int RunKarger(vector<edge> edges, unordered_set<int> v){
    vector<edge> mst = find_mst(edges, v);
    auto it = max_element(mst.begin(), mst.end());
    edge max_edge = *it;
    mst.erase(it);
    
    int v1 = max_edge.u;
    int v2 = max_edge.v;

    unordered_set<int> left = find_vertices(v1, mst);
    unordered_set<int> right = find_vertices(v2, mst);

    int k = 0;
    for (auto &it : edges){
        if ((left.count(it.u) && right.count(it.v)) || (right.count(it.u) && left.count(it.v)))
            k++;
    }
    return k;
}

int Karger(vector<edge> edges, unordered_set<int> v, int max){
    int min = 10000000, index = -1;
    for (int i = 0; i < max; i++){
        vector<edge> aux_edges = edges;
        unordered_set<int> aux_v = v;
        int k = RunKarger(aux_edges, aux_v);
        if (k < min){
            min = k;
            index = i;
        }
    }
    cout << "Found minimum at " << index << " iterations" << endl;
    return min;
}

int main(int argc, char* argv[])
{
    int n, a, k;
    string file_path = (argc>1) ? argv[1] : "./instances/test.max";
    string algorithm = (argc>2) ? argv[2] : "K";
	bool heap_type = (argc>3) ? stoi(argv[3]) : false;
    vector<edge> edges;
    vector<adjacency_row> adjacency = read_input(n,a,file_path, edges);
    vector<int> vertices(n);
    unordered_set<int> v;
    for (int i = 0; i < n; i++)
    {
        v.insert(i);
        vertices[i] = i;
    }
	auto started = chrono::high_resolution_clock::now();
    if (algorithm == "K"){
        k = Karger(edges, v, 50);
    }else{
        k = Stoer_Wagner(adjacency, vertices,n,a,heap_type);    
    }
    auto done = chrono::high_resolution_clock::now();
    cout << k << " " <<chrono::duration_cast<chrono::milliseconds>(done-started).count() << endl;
    return 0;
}