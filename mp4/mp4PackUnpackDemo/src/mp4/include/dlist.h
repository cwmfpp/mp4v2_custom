/*
 *	file: dlist.h
 *	description: dynamic list
 *	history: dongliqiang created at 20130304
 */

//#include "common.h"
#include <pthread.h>

#define STDCALL 
#define X_API 

typedef struct _DList DList;            /**< 双向链表 */
typedef struct _DListNode DListNode;    /**< 双向链表节点 */

typedef struct {
	pthread_mutex_t mutex;
}LMutex;

struct _DListNode
{
	DListNode* next, * prev;
	char data[0];
};

struct _DList
{
	DListNode* head;
	DListNode* end;
	unsigned int node_size;
	unsigned int len;
	LMutex mutex;
};

/**
    @brief 双端链表构造    
    @param node_size 节点数据的大小
    @retval NULL 失败 
    @retval 其它 链表
 */
DList *X_API STDCALL adt_dlist_create(unsigned int node_size);

/**
    @brief 链表销毁 
    @param dlist 链表
    @retval TRUE 成功 
    @retval FALSE 失败
 */
int X_API STDCALL adt_dlist_destroy(DList *dlist);

/**
    @brief 链表插入 
    @param dlist 链表
    @param node 插入位置
    @param data 数据
    @retval NULL 失败 
    @retval 其它 成功
 */
DListNode *X_API STDCALL adt_dlist_insert(DList *dlist, DListNode *node, void *data);

/**
    @brief 链表添加 
    @param dlist 链表
    @param node 添加位置
    @param data 数据
    @retval NULL 失败 
    @retval 其它 成功
 */
DListNode *X_API STDCALL adt_dlist_add(DList *dlist, DListNode *node, void *data);

/**
    @brief 链表下一个节点 
    @param dlist 链表
    @param node 当前位置
    @return NULL 链表尾 
    @retval 其它 下一个节点
 */
DListNode *X_API STDCALL adt_dlist_next(DListNode *node);

/**
    @brief 链表上一个节点 
    @param dlist 链表
    @param node 当前位置
    @retval NULL 链表尾 
    @retval 其它 上一个节点
 */
DListNode *X_API STDCALL adt_dlist_prev(DListNode *node);

/**
    @brief 链表头节点 
    @param dlist 链表
    @return 链表头
 */
DListNode *X_API STDCALL adt_dlist_head(DList *dlist);

/**
    @brief 链表尾节点 
    @param dlist 链表
    @return 链表尾
 */
DListNode *X_API STDCALL adt_dlist_end(DList *dlist);

/**
    @brief 链表大小 
    @param dlist 链表
    @return 链表中节点个数
 */
unsigned int X_API STDCALL adt_dlist_size(DList *dlist);

/**
    @brief 链表是否为空 
    @param dlist 链表
    @retval TRUE 空 
    @retval FALSE 非空
 */
int X_API STDCALL adt_dlist_empty(DList *dlist);

/**
    @brief  链表删除节点 
     @param dlist 链表
    @param node 节点
    @return TRUE 成功 
    @retval FALSE 失败
 */
int X_API STDCALL adt_dlist_delete(DList *dlist, DListNode *node);

/**
    @brief 链表获得节点数据  
    @param node 节点
    @retval NULL 失败 
    @retval 其它 节点数据
 */
void *X_API STDCALL adt_dlist_getdata(DListNode *node);

/**
    @brief 锁定链表 
    @param dlist 链表
    @retval TRUE 成功 
    @retval FALSE 失败
 */
int X_API STDCALL adt_dlist_lock(DList *dlist);

/**
    @brief 解锁链表 
    @param dlist 链表
    @retval TRUE 成功 
    @retval FALSE 失败
 */
int X_API STDCALL adt_dlist_unlock(DList *dlist);

/**
    @brief 链表锁初始化 
    @param dlist 链表
    @retval TRUE 成功 
    @retval FALSE 失败
 */
int X_API STDCALL adt_dlist_lock_init(DList* dlist);

/**
    @brief 链表锁反初始化
    @param dlist 链表
    @retval TRUE 成功 
    @retval FALSE 失败
 */
int X_API STDCALL adt_dlist_lock_uninit(DList* dlist);

