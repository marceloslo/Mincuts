// A C++ program to demonstrate common Binary Heap Operations
#ifndef BIN_HEAP_H
#define BIN_HEAP_H
#include <vector>
#include <unordered_map>

using namespace std;

class binary_heap
{
public:
	binary_heap();

	//pushes p to the heap
	void push(pair<int, int> p);

	//increases key of node with value (cost,key), if key does not exist, push it 
	void increase_key(int key,int increment);
	//extracts biggest value
	pair<int, int> extract_max();

	//get index of parent of node at index i
	int parent(int i) { return (i - 1) / 2; }

	//get index of left child of node at index i
	int left(int i) { return (2 * i + 1); }

	//get index of right child of node at index i
	int right(int i) { return (2 * i + 2); }

	size_t size();

	void swap(int index1, int index2);

private:
	vector<pair<int, int>> heap;
	unordered_map<int,int> indexes;
};


#endif // !BIN_HEAP_H