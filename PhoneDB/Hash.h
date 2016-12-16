#pragma once

class SimpleHashSet {
public:
	SimpleHashSet(int headNum, int maxSize);

	bool add(int x);
	bool contains(int x) const;
	static int hash(int x);
	int index(int hash) const;

private:
	int* head; // ������ �����
	int* next; // ������ ������ �� ��������� �������
	int* keys; // ������ � ������� (������ int'� ������� ��������� ������ ���)
	int headNum; // ���������� �����
	int cnt = 1; // ������ �� ����� ��� ������� ������ ��������
};
