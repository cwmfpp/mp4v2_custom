/*
 *	file: dlist.c
 *	description: dynamic list
 *	history: dongliqiang created at 20130304
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dlist.h"
//#include "common.h"
#ifdef MEMWATCH
#include "memwatch.h"
#endif
#include "libtiandy.h"
#include "td_errno.h"
#ifndef FOR_MP4SDK
#include "debug_lock.h"
#endif

DList* X_API STDCALL adt_dlist_create(unsigned int node_size)
{
    DList* dlist = (DList*)malloc(sizeof(DList));
	if (dlist == NULL)
	{
		td_printf(0,"malloc fail");
        return NULL;
	}
	dlist->node_size = node_size;
	dlist->head = NULL;
	dlist->end = NULL;
	dlist->len = 0;
//	os_mutex_init(&dlist->mutex, TRUE);
	adt_dlist_lock_init(dlist);

	return dlist;
}

int  X_API STDCALL adt_dlist_destroy(DList* dlist)
{
    if (dlist == NULL)
    {
		td_printf(0,"dlist is null");
		return TD_FALSE;
	}

	void* ptr;
	DListNode* node = dlist->head;
	while (node != NULL)
	{
		ptr = node;
		node = node->next;
		free(ptr);
	}

//	os_mutex_destroy(&dlist->mutex);
	adt_dlist_lock_uninit(dlist);

	free(dlist);
	return TD_TRUE;
}

DListNode* X_API STDCALL adt_dlist_insert(DList* dlist, DListNode* node, void* data)
{
    if (dlist == NULL)
	{
		td_printf(0,"dlist is null");
		return TD_FALSE;
	}

	DListNode* new_node = (DListNode*)malloc(sizeof(DListNode) + dlist->node_size);
	memcpy(new_node->data, data, dlist->node_size);
	new_node->next = new_node->prev = NULL;

	if (dlist->head == NULL)
	{
		dlist->head = new_node;
		dlist->end = new_node;
	}
	else
	{
		if (node == NULL || node == dlist->head)
		{
			dlist->head->prev = new_node;
			new_node->next = dlist->head;
			dlist->head = new_node;
		}
		else
		{
			new_node->prev = node->prev;
			if (node->prev != NULL)
			{
				node->prev->next = new_node;
			}
			new_node->next = node;
			node->prev = new_node;
		}
	}

	++dlist->len;
	return new_node;
}

DListNode* X_API STDCALL adt_dlist_add(DList* dlist, DListNode* node, void* data)
{
	if (dlist == NULL)
	{
		td_printf(0,"dlist is null");
		return NULL;
	}

	DListNode* new_node = (DListNode*)malloc(sizeof(DListNode) + dlist->node_size);
	if(NULL == new_node)
	{
		//my_perror("adt_dlist_add, malloc node failed");
		td_printf(0,"adt_dlist_add, malloc node failed");
		return NULL;
	}
	memcpy(new_node->data, data, dlist->node_size);
	new_node->next = new_node->prev = NULL;

	if(dlist->head == NULL)
	{
		dlist->head = new_node;
		dlist->end = new_node;
	}
	else
	{
		if(node == NULL || node == dlist->end)
		{
			dlist->end->next = new_node;
			new_node->prev = dlist->end;
			dlist->end = new_node;
		}
		else
		{
			new_node->prev = node;
			if(node->next != NULL)
			{
				node->next->prev = new_node;
			}
			new_node->next = node->next;
			node->next = new_node;
		}
	}

	++dlist->len;
	return new_node;
}

DListNode* X_API STDCALL adt_dlist_next(DListNode* node)
{
    if (node == NULL)
	{
		return NULL;
	}

	return node->next;
}

DListNode* X_API STDCALL adt_dlist_prev(DListNode* node)
{
    if (node == NULL)
	{
		return NULL;
	}

	return node->prev;
}

DListNode* X_API STDCALL adt_dlist_head(DList* dlist)
{
    if (dlist == NULL)
	{
		td_printf(0,"dlist is null");
		return TD_FALSE;
	}

	return dlist->head;
}

DListNode* X_API STDCALL adt_dlist_end(DList* dlist)
{
    if (dlist == NULL)
	{
		td_printf(0,"dlist is null");
		return TD_FALSE;
	}

	return dlist->end;
}

unsigned int X_API STDCALL adt_dlist_size(DList* dlist)
{
	if (dlist == NULL)
	{
		td_printf(0,"dlist is null");
		return TD_FALSE;
	}

	return dlist->len;
}

int X_API STDCALL adt_dlist_empty(DList* dlist)
{
	if (dlist == NULL)
	{
		td_printf(0,"dlist is null");
		return TD_TRUE;//FALSE;
	}

    return (dlist->head == NULL);
}

int X_API STDCALL adt_dlist_delete(DList* dlist, DListNode* node)
{
    if (dlist == NULL)
	{
		td_printf(0,"dlist is null");
		return TD_FALSE;
	}

	if (node == NULL)
	{
		td_printf(0,"node is null");
		return TD_FALSE;
	}

	if (dlist->head == node)
	{
		dlist->head = dlist->head->next;
		if (dlist->head != NULL)
		{
			dlist->head->prev = NULL;
		}

		free(node);
	}
	else if (dlist->end == node)
	{
		dlist->end = dlist->end->prev;
		if (dlist->end != NULL)
		{
			dlist->end->next = NULL;
		}

		free(node);
	}
	else
	{
		node->prev->next = node->next;
		if (node->next != NULL)
		{
			node->next->prev = node->prev;
		}
		free(node);
	}

	--dlist->len;
	return TD_TRUE;

}

void* X_API STDCALL adt_dlist_getdata(DListNode* node)
{
    return &node->data[0];
}

int X_API STDCALL adt_dlist_lock_init(DList* dlist)
{
    if (dlist == NULL)
    {
		td_printf(0,"dlist is null");
		return TD_FALSE;
	}

	//return os_mutex_lock(&dlist->mutex);
	return pthread_mutex_init(&dlist->mutex.mutex, NULL);
}

int X_API STDCALL adt_dlist_lock_uninit(DList* dlist)
{
    if (dlist == NULL)
    {
		td_printf(0,"dlist is null");
		return TD_FALSE;
	}

	//return os_mutex_lock(&dlist->mutex);
	return pthread_mutex_destroy(&dlist->mutex.mutex);
}

int X_API STDCALL adt_dlist_lock(DList* dlist)
{
    if (dlist == NULL)
    {
		td_printf(0,"dlist is null");
		return TD_FALSE;
	}

	//return os_mutex_lock(&dlist->mutex);
	pthread_mutex_lock(&dlist->mutex.mutex);
	return 0;
}

int X_API STDCALL adt_dlist_unlock(DList* dlist)
{
    if (dlist == NULL) {
		td_printf(0,"dlist is null");
		return TD_FALSE;
	}

	//return os_mutex_unlock(&dlist->mutex);
	pthread_mutex_unlock(&dlist->mutex.mutex);
	return 0;
}

