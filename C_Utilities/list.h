#ifndef DEF_LIST
#define DEF_LIST



// Le type des éléments de cette liste
#define LIST_ELT_TYPE	int



#ifndef __cplusplus
#include <stdbool.h>
#endif

typedef struct Cell Cell;
typedef struct List List;

List* list_create();
void list_delete(List* l);
void list_pushBack(List* l, LIST_ELT_TYPE v);
void list_pushFront(List* l, LIST_ELT_TYPE v);
void list_popBack(List* l);
void list_popFront(List* l);

bool list_isEmpty(List* l);
unsigned int list_getSize(List* l);

LIST_ELT_TYPE list_back(List* l);
LIST_ELT_TYPE list_front(List* l);

Cell* list_getBackIterator(List* l);
Cell* list_getFrontIterator(List* l);
LIST_ELT_TYPE list_getElt(Cell* it);
Cell* list_incrIt(Cell* it);
Cell* list_decrIt(Cell* it);
bool list_isValidIt(Cell* it);
bool list_isLast(Cell* it);
bool list_isFirst(Cell* it);

#endif
