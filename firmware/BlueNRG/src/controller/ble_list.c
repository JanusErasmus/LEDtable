/******************** (C) COPYRIGHT 2012 STMicroelectronics ********************
 * File Name          : ble_list.c
 * Author             : AMS - HEA&RF BU
 * Version            : V1.0.0
 * Date               : 19-July-2012
 * Description        : Circular Linked List Implementation.
 ********************************************************************************
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 * AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
 * INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
 * CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
 * INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *******************************************************************************/

/******************************************************************************
 * Include Files
 ******************************************************************************/
#include <stdint.h>
#include <cyg/kernel/kapi.h>
#include "ble_list.h"

//cyg_uint8 mListInit = 0;
//cyg_mutex_t mListMutex;

/******************************************************************************
 * Function Definitions 
 ******************************************************************************/
void list_init_head (tListNode * listHead)
{
	listHead->next = listHead;
	listHead->prev = listHead;

//	if(!mListInit)
//	{
//		mListInit = 1;
//		cyg_mutex_init(&mListMutex);
//	}
}

uint8_t list_is_empty (tListNode * listHead)
{
	uint8_t return_value;

	//cyg_mutex_lock(&mListMutex);

	if(listHead->next == listHead)
	{
		return_value = TRUE;
	}
	else
	{
		return_value = FALSE;
	}
	//cyg_mutex_unlock(&mListMutex);

	return return_value;
}

void list_insert_head (tListNode * listHead, tListNode * node)
{
	//cyg_mutex_lock(&mListMutex);


	node->next = listHead->next;
	node->prev = listHead;
	listHead->next = node;
	(node->next)->prev = node;

	//cyg_mutex_unlock(&mListMutex);
}

void list_insert_tail (tListNode * listHead, tListNode * node)
{
	//cyg_mutex_lock(&mListMutex);


	node->next = listHead;
	node->prev = listHead->prev;
	listHead->prev = node;
	(node->prev)->next = node;

	//cyg_mutex_unlock(&mListMutex);
}

void list_remove_node (tListNode * node)
{
	//cyg_mutex_lock(&mListMutex);


	(node->prev)->next = node->next;
	(node->next)->prev = node->prev;

	//cyg_mutex_unlock(&mListMutex);
}

void list_remove_head (tListNode * listHead, tListNode ** node )
{
	//cyg_mutex_lock(&mListMutex);


	*node = listHead->next;
	list_remove_node (listHead->next);
	(*node)->next = NULL;
	(*node)->prev = NULL;

	//cyg_mutex_unlock(&mListMutex);
}

void list_remove_tail (tListNode * listHead, tListNode ** node )
{
	//cyg_mutex_lock(&mListMutex);


	*node = listHead->prev;
	list_remove_node (listHead->prev);
	(*node)->next = NULL;
	(*node)->prev = NULL;

	//cyg_mutex_unlock(&mListMutex);
}

void list_insert_node_after (tListNode * node, tListNode * ref_node)
{
	//cyg_mutex_lock(&mListMutex);


	node->next = ref_node->next;
	node->prev = ref_node;
	ref_node->next = node;
	(node->next)->prev = node;

	//cyg_mutex_unlock(&mListMutex);
}

void list_insert_node_before (tListNode * node, tListNode * ref_node)
{
	//cyg_mutex_lock(&mListMutex);

	node->next = ref_node;
	node->prev = ref_node->prev;
	ref_node->prev = node;
	(node->prev)->next = node;

	//cyg_mutex_unlock(&mListMutex);
}

int list_get_size (tListNode * listHead)
{
	int size = 0;
	tListNode * temp;

	//cyg_mutex_lock(&mListMutex);


	temp = listHead->next;
	while (temp != listHead)
	{
		size++;
		temp = temp->next;
	}

	//cyg_mutex_unlock(&mListMutex);
	return (size);
}

void list_get_next_node (tListNode * ref_node, tListNode ** node)
{
	//cyg_mutex_lock(&mListMutex);
	*node = ref_node->next;
	//cyg_mutex_unlock(&mListMutex);
}

void list_get_prev_node (tListNode * ref_node, tListNode ** node)
{
	//cyg_mutex_lock(&mListMutex);
	*node = ref_node->prev;
	//cyg_mutex_unlock(&mListMutex);
}

