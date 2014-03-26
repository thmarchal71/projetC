#include <list.h>
#include <assert.h>

struct list* list_new()
{
	return NULL;
}

struct list* list_get_tail(struct list* l)
{
	if(l != NULL)
		while(l->next != NULL)
			l = l->next;
	return l;
}

struct list* list_insert_tail(struct list* l, int x, int y, void* data)
{
	struct list* list_new = l;
	struct list* tmp = malloc( sizeof(*tmp) );
	tmp->data = data;
	tmp->x = x;
	tmp->y = y;
	tmp->next = NULL;

	if( l != NULL ){
		l = list_get_tail(l);
		l->next = tmp;
	} else {
		list_new = tmp;
}

	return list_new;
}

struct list* list_find(struct list* l, int x, int y)
{
	while(l != NULL)
	{
		if(l->x == x && l->y == y)
			break;
		else
			l = l->next;
	}
	return l;
}

struct list* list_find_delete(struct list* l, int x, int y)
{
	struct list* tmp = l;
	struct list* list_n = list_new();

	while(l != NULL)
	{
		if(l->x != x || l->y != y) {
			list_n = list_insert_tail(list_n, l->x, l->y, l->data);
		}
		l = l->next;
	}

	tmp = list_delete(tmp);

	return list_n;
}

struct list* list_delete_head(struct list* l)
{
	assert(l);

	struct list* head = l;

	if(l != NULL && l->next != NULL)
		l = l->next;
	else
		l = NULL;

	free(head);

	return l;
}

struct list* list_delete(struct list* l)
{
	while(l != NULL)
		l = list_delete_head(l);
	return l;
}
