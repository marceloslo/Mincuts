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
	while (i > 0 && heap[parent(i)] < heap[i])
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
	pair<int, int> new_value = make_pair(heap[i].first+increment, heap[i].second);
	heap[i] = new_value;
	while (i > 0 && heap[parent(i)] < heap[i])
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

	int i = 0;
	while (left(i) < heap.size())
	{
		if (right(i) >= heap.size() || heap[left(i)] >= heap[right(i)])
		{
			if (heap[i] > heap[left(i)])
			{
				swap(i, left(i));
			}
			else
			{
				break;
			}
		}
		else
		{
			if (heap[i] > heap[right(i)])
			{
				swap(i, right(i));
			}
			else
			{
				break;
			}
		}
	}
	return root;
}



size_t binary_heap::size()
{
	return this->heap.size();
}

void binary_heap::swap(int i, int j)
{
	pair<int,int> temp = heap[i];
	heap[i] = heap[j];
	heap[j] = temp;
	indexes[heap[i].second] = i;
	indexes[heap[j].second] = j;
}