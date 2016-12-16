#pragma once

class SimpleHashSet {
public:
	SimpleHashSet(int headNum, int maxSize);

	bool add(int x);
	bool contains(int x) const;
	static int hash(int x);
	int index(int hash) const;

private:
	int* head; // массив голов
	int* next; // массив ссылок на следующий элемент
	int* keys; // массив с ключами (вместо int'а следует поставить нужный тип)
	int headNum; // количество голов
	int cnt = 1; // ссылка на место для вставки нового эдемента
};
