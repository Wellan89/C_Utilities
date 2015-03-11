#include "list.h"

struct Cell
{
};
struct List
{
};

List* list_create()
{
}
void list_delete(List* l)
{
}
void list_pushBack(List* l, ELT_TYPE v)
{
}
void list_pushFront(List* l, ELT_TYPE v)
{
}
void list_popBack(List* l)
{
}
void list_popFront(List* l)
{
}

bool list_isEmpty(List* l)
{
}
int list_getSize(List* l)
{
}

ELT_TYPE list_front(List* l)
{
}
ELT_TYPE list_back(List* l)
{
}

Cell* list_getFrontIterator(List* l)
{
}
ELT_TYPE list_getElt(Cell* it)
{
}
void list_incrIt(Cell* it)
{
}
void list_decrIt(Cell* it)
{
}
bool list_isBack(Cell* it)
{
}
bool list_isFront(Cell* it)
{
}
