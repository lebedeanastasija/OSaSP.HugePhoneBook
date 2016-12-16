#include "stdafx.h"

#include "Hash.h"

SimpleHashSet::SimpleHashSet(int headNum, int maxSize)
	: headNum(headNum)
{
	head = new int[headNum];
	next = new int[maxSize + 1];
	keys = new int[maxSize + 1];
}

bool SimpleHashSet::add(int x)
{
	if (contains(x)) {
		return false;
	}

	auto h = index(hash(x));
	next[cnt] = head[h];
	keys[cnt] = x;
	head[h] = cnt++;

	return true;
}

bool SimpleHashSet::contains(int x) const
{
	auto h = index(hash(x));

	for (auto i = head[h]; i != 0; i = next[i]) {
		if (keys[i] == x) {
			return true;
		}
	}

	return false;
}

int SimpleHashSet::hash(int x)
{
	return x >> 15 ^ x;
}

int SimpleHashSet::index(int hash) const
{
	return abs(hash) % headNum;
}
