#ifndef GRAPH_H
#define GRAPH_H
#include <unordered_map>
#include <vector>

using namespace std;

typedef struct edge
{
	int u, v, cost;
	bool mark = false;
}edge;

inline bool operator< (const edge &e1, const edge &e2) {return e1.cost <  e2.cost;}

class adjacency_row
{
public:
	void push_back(edge e);
	edge& operator[](int index);
	int index(int v);
	bool exists(int v);
	void merge(int v, int u);

	size_t size();

private:
	unordered_map<int,int> neighbor_index;
	vector<edge> edges;
};

#endif // !GRAPH_H

