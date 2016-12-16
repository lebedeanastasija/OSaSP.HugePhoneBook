#pragma once
#include <cuchar>
#include <string>
#include <vector>
using namespace std;


#ifndef SKIP_LIST_H
#define SKIP_LIST_H 
//============================================================================== 
struct SkipNode {
	int key;
	string value;

	// pointers to successor nodes
	vector<SkipNode*> forward;

	SkipNode(int k, const std::string& v, int level);
};

//============================================================================== 
class SkipList {
public:
	SkipList();
	~SkipList();

	// non-modifying member functions
	void print();
	SkipNode* find(int searchKey);

	// modifying member functions
	void insert(int searchKey, std::string newValue);
	void erase(int searchKey);
private:
	// pointer to first node
	SkipNode* head;
	// last node
	SkipNode* NIL;

	// implicitly used member functions
	int randomLevel();
	int nodeLevel(const std::vector<SkipNode*>& v);
	SkipNode* makeNode(int key, std::string val, int level);

	// data members  
	float probability;
	int maxLevel;
};

#include "SkipList.cpp"
#endif
