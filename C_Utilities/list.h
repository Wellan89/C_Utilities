#ifndef DEF_LIST
#define DEF_LIST

#include "bool.h"

#define ELT_TYPE	int

typedef struct Cell Cell;
typedef struct List List;

List* list_create();
void list_delete(List* l);
void list_pushBack(List* l, ELT_TYPE v);
void list_pushFront(List* l, ELT_TYPE v);
void list_popBack(List* l);
void list_popFront(List* l);

bool list_isEmpty(List* l);
unsigned int list_getSize(List* l);

ELT_TYPE list_back(List* l);
ELT_TYPE list_front(List* l);

Cell* list_getBackIterator(List* l);
Cell* list_getFrontIterator(List* l);
ELT_TYPE list_getElt(Cell* it);
Cell* list_incrIt(Cell* it);
Cell* list_decrIt(Cell* it);
bool list_isValidIt(Cell* it);
bool list_isLast(Cell* it);
bool list_isFirst(Cell* it);

#endif
