#pragma once
#include "stdafx.h"

typedef struct snode {
	TCHAR key[PHONE_DB_PHONE_LEN];
	PHONE_DB_RECORD value;
	struct snode **forward;
} snode;

typedef struct skiplist {
	int level;
	int size;
	struct snode *header;
} skiplist;

skiplist *skiplist_init(skiplist *list);
int rand_level();
int skiplist_insert(skiplist *list, TCHAR* key, PHONE_DB_RECORD value);
snode *skiplist_search(skiplist *list, TCHAR* key);
void skiplist_node_free(snode *x);
int skiplist_delete(skiplist *list, TCHAR* key);
bool compare_numbers(TCHAR a[], TCHAR b[]);


