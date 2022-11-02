#include "../headers/Graph.hpp"

void adjacency_row::push_back(edge e)
{
	edges.push_back(e);
	neighbor_index[e.v] = edges.size() - 1;
}

edge& adjacency_row::operator[](int index)
{
	return edges[index];
}

int adjacency_row::index(int v)
{
	if (exists(v))
	{
		return neighbor_index[v];
	}
	return -1;
}

bool adjacency_row::exists(int v)
{
	return neighbor_index.find(v) != neighbor_index.end();
}

void adjacency_row::merge(int v, int u)
{
	int idx_v = index(v);
	int idx_u = index(u);
	if (idx_v != -1 && idx_u != -1)
	{
		edges[idx_v].cost += edges[idx_u].cost;
		edges[idx_u].mark = true;
		//edges.erase(edges.begin() + idx_u);
		neighbor_index.erase(u);
	}
	else if (idx_u != -1)
	{
		edges[idx_u].v = v;
		neighbor_index[v] = idx_u;
		neighbor_index.erase(u);
	}
}

size_t adjacency_row::size()
{
	return edges.size();
}
