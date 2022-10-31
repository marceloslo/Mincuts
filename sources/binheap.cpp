#include "../headers/binheap.hpp"

using namespace std;

binary_heap::binary_heap()
{
}

void binary_heap::push(pair<int, int> p)
{
	int i = heap.size();
	heap.push_back(p);
	indexes[p.second] = i;
	while (i!=0 && heap[parent(i)] < heap[i])
	{
		this->swap(parent(i), i);
		i = parent(i);
	}
}

void binary_heap::increase_key(int key, int increment)
{
	if (indexes.find(key) == indexes.end())
	{
		push(make_pair(increment, key));
		return;
	}
	int i = indexes[key];
	pair<int, int> new_value = make_pair(heap[i].first + increment, heap[i].second);
	heap[i] = new_value;
	while (i != 0 && heap[parent(i)] < heap[i])
	{
		this->swap(i, parent(i));
		i = parent(i);
	}
}

pair<int, int> binary_heap::extract_max()
{
	pair<int, int> root= heap.front();
	indexes.erase(root.second);
	heap[0] = heap.back();
	heap.pop_back();
	if(!heap.empty())
		indexes[heap[0].second] = 0;
	heapify(0);
	return root;
}



size_t binary_heap::size()
{
	return this->heap.size();
}

void binary_heap::swap(int index1, int index2)
{
	pair<int,int> temp = heap[index1];
	indexes[heap[index1].second] = index2;
	indexes[heap[index2].second] = index1;
	heap[index1] = heap[index2];
	heap[index2] = temp;
}

void binary_heap::heapify(int i)
{
	int l = left(i);
	int r = right(i);
	int biggest = i;
	if (l < heap.size() && heap[l] > heap[i])
		biggest = l;
	if (r < heap.size() && heap[r] > heap[biggest])
		biggest = r;
	if (biggest != i)
	{
		swap(i, biggest);
		heapify(biggest);
	}
}
