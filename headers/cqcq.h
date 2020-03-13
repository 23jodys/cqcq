#ifndef CQCQ_H
#define CQCQ_H
#include <stdbool.h>
#include <stdlib.h>
/** @file */

/** 
 * @brief
 * Represents elements in the queueSD:
 */
typedef struct Element {
	void* value; /**< value to in this element */
	struct Element* next; /**< pointer to next element in queue */
	bool success; /**< whether or not the operation on this element was successful */
} Element;

/**
 * @brief
 * Represents the queue itself
 */
typedef struct CqQueue {
	int size;
	Element* head;
	bool success;
} CqQueue;

/** @brief
 * Adds an element to the queue
 */
CqQueue* queue_add(CqQueue* head, void* value);

/** 
 * @brief Create a new queue object
 * Create a new queue object
 */
CqQueue* queue_create();

/**
 * @brief return the value the item popped from the queue
 * @param [in,out] head the queue to pop from 
 */
void* queue_pop(CqQueue* head);

/**
 * @brief return size of queue
 * @param [in] head the queue object 
 */
int queue_size(CqQueue* head);

/* Private functions */
/**
 * @brief behind the facade for queue_add, this allows you to pass in a function pointer to malloc, useful mostly for testing
 */
CqQueue* _queue_add(CqQueue* head, void* value, void*(malloc_ptr)(size_t size));

/* Private functions */
/**
 * @brief behind the facade for queue_create, this allows you to pass in a function pointer to malloc, useful mostly for testing
 */
CqQueue* _queue_create(void*(malloc_ptr)(size_t size));

#endif /* CQCQ_H */
