#ifndef __CONNBEE_QUEUE_H__
#define __CONNBEE_QUEUE_H__
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

/**
* @brief structure representing one element of the queue
*/
struct connbee_queue_item {
  /// pointer to the actual queue data
	void *contents;

  /// link to the next element in the queue
	struct connbee_queue_item* next;

  /// link to the previous element in the queue
  struct connbee_queue_item* previous;
};

/// structure representing the root of a queue with links to head and tail
struct connbee_queue_root {

  /// link to the head of the queue
	struct connbee_queue_item* head;

  /// link to the tail of the queue
	struct connbee_queue_item* tail;
};

/**
* @brief initialize a queue
*
* @param queue - pointer to the queue
*/
void connbee_queue_init(struct connbee_queue_root* queue);


/**
* @brief push contents onto the queue
*
* @param queue - pointer to the head of the queue
* @param contents - the content to enqueue
*/
void connbee_queue_push(struct connbee_queue_root* queue, void *content);

/**
* @brief pop content from the queue
*
* @param queue - the root of the queue
*
* @return pointer to the content
*/
void * connbee_queue_pop(struct connbee_queue_root* queue);

/**
* @brief delete item from queue
*
* @param  queue - the queue from which to delete the item
* @param item   - queue element to delete
*
*/
void connbee_queue_delete(struct connbee_queue_root *queue, struct connbee_queue_item *item);

#endif
