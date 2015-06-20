#include <stdlib.h>
#include "list.h"

struct Cell
{
	LIST_ELT_TYPE val;
	Cell* prev;
	Cell* next;
};
struct List
{
	Cell* first;
	Cell* last;
	unsigned int size;
};

List* list_create()
{
	List* l = (List*)malloc(sizeof(List));
	if (!l)	return NULL;
	l->first = NULL;
	l->last = NULL;
	l->size = 0;
	return l;
}
void list_delete(List* l)
{
	Cell* p = l->first;
	while (p)
	{
		Cell* tmp = p;
		p = p->next;
		free(tmp);
	}
	free(l);
}
void list_pushBack(List* l, LIST_ELT_TYPE v)
{
	Cell* c = (Cell*)malloc(sizeof(Cell));
	if (!c)	return;
	c->val = v;
	c->prev = l->last;
	c->next = NULL;

	if (!l->first)
		l->first = c;

	if (l->last)
	{
		l->last->next = c;
		l->last = c;
	}
	else
		l->last = c;

	l->size++;
}
void list_pushFront(List* l, LIST_ELT_TYPE v)
{
	Cell* c = (Cell*)malloc(sizeof(Cell));
	if (!c)	return;
	c->val = v;
	c->prev = NULL;
	c->next = l->first;

	if (!l->last)
		l->last = c;

	if (l->first)
	{
		l->first->prev = c;
		l->first = c;
	}
	else
		l->first = c;

	l->size++;
}
void list_popBack(List* l)
{
	Cell* c = l->last;
	l->last = l->last->prev;
	if (l->last)
		l->last->next = NULL;
	else
		l->first = NULL;
	l->size--;
	free(c);
}
void list_popFront(List* l)
{
	Cell* c = l->first;
	l->first = l->first->next;
	if (l->first)
		l->first->prev = NULL;
	else
		l->last = NULL;
	l->size--;
	free(c);
}

bool list_isEmpty(List* l)
{
	return (l->size == 0);
}
unsigned int list_getSize(List* l)
{
	return l->size;
}

LIST_ELT_TYPE list_back(List* l)
{
	return l->last->val;
}
LIST_ELT_TYPE list_front(List* l)
{
	return l->first->val;
}

Cell* list_getBackIterator(List* l)
{
	return l->last;
}
Cell* list_getFrontIterator(List* l)
{
	return l->first;
}
LIST_ELT_TYPE list_getElt(Cell* it)
{
	return it->val;
}
Cell* list_incrIt(Cell* it)
{
	return it->next;
}
Cell* list_decrIt(Cell* it)
{
	return it->prev;
}
bool list_isValidIt(Cell* it)
{
	return (it != NULL);
}

bool list_isLast(Cell* it)
{
	return (it->next == NULL);
}
bool list_isFirst(Cell* it)
{
	return (it->prev == NULL);
}
