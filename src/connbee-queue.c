/*
 * This file is part of the libconnbee library distribution (https://gitcloud.federationhq.de/byterazor/libconnbee)
 * Copyright (c) 2019 Dominik Meyer <dmeyer@federationhq.de>.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

 /** @file */
#include <connbee-queue.h>
#include <stdlib.h>

/**
* @brief initialize a queue
*
* @param queue - pointer to the queue
*/
void connbee_queue_init(struct connbee_queue_root* queue){
	queue->head = queue->tail = NULL;
}

/**
* @brief push contents onto the queue
*
* @param queue - pointer to the head of the queue
* @param contents - the content to enqueue
*/
void connbee_queue_push(struct connbee_queue_root* queue, void *content){
	struct connbee_queue_item *item = malloc(sizeof(item));
	item->contents = content;
	item->next     = NULL;
  item->previous = NULL;

	if (queue->head == NULL){
		queue->head = queue->tail = item;
	} else {
    item->previous = queue->tail;
		queue->tail = queue->tail->next = item;
	}
}

/**
* @brief pop content from the queue
*
* @param queue - the root of the queue
*
* @return pointer to the content
*/
void * connbee_queue_pop(struct connbee_queue_root* queue){
	void * popped;
	if (queue->head == NULL){
		return NULL; // causes a compile warning.  Just check for ==NULL when popping.
	} else {
		popped = queue->head->contents;
		struct connbee_queue_item* next = queue->head->next;
		free(queue->head);
		queue->head = next;
		if (queue->head == NULL)
			queue->tail = NULL;
	}
	return popped;
}


/**
* @brief delete item from queue
*
* @param  queue - the queue from which to delete the item
* @param item   - queue element to delete
*
*/
void connbee_queue_delete(struct connbee_queue_root *queue, struct connbee_queue_item *item)
{
  if (item->previous == NULL)
  {
    queue->head = item->next;
  }
  else
  {
    item->previous->next=item->next;
  }
  free(item);
}
