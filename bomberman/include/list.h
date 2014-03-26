#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>
#include <stdio.h>
#include <map.h>
#include <sprite.h>
#include <window.h>


struct list {
	void* data;
	int x, y;
	struct list* next;
};


struct list* list_new();
struct list* list_insert_head(struct list* l, int x, int y, void* data);
struct list* list_insert_tail(struct list* l, int x, int y, void* data);
struct list* list_find(struct list* l, int x, int y);
struct list* list_find_delete(struct list* l, int x, int y);
struct list* list_delete_head(struct list* l);
struct list* list_delete_tail(struct list* l);
struct list* list_delete(struct list* l);


#endif
