#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "dbg.h"
#include "cqcq.h"

CqQueue* queue_add(CqQueue* head, void* value) {
	return _queue_add(head, value, malloc);
}


CqQueue* _queue_add(CqQueue* cq, void* value, void*(malloc_ptr)(size_t size)) {
	debug("in queue_add");
	Element* new;
	new = malloc_ptr(sizeof(Element));
	check_mem(new);

	Element* old_head = cq->head;
	new->next = cq->head;
	new->value = value;
	new->success = true;

	cq->head = new;
	return cq;	

error:
	cq->success = false;
	return cq;
}

CqQueue* queue_create() {
	return _queue_create(malloc);
}

CqQueue* _queue_create(void*(malloc_ptr)(size_t size)) {
	debug("in _queue_create");
	CqQueue* new;
	new = malloc_ptr(sizeof(CqQueue));
	check_mem(new);
	return new;
error:
	return NULL;
}

void* queue_pop(CqQueue* cq) { 
	debug("in queue_pop");
	debug("head value is %d", *(int*)cq->head->value);
	void* to_return = cq->head->value;
	debug("value is %d", *(int*)to_return);

	if ( cq->head->next == NULL) {
		return NULL;
	}
	return to_return;
}

void queue_element_debug(Element* element) {
	debug("value %d, success %d, next %p", *(int*)element->value, element->success, element->next);
}

int queue_size(CqQueue* cq) {
	debug("in queue_size");
	if (cq->head == NULL) {
		debug("null head");
		return 0;
	} else {
		debug("fuck0");
		int count = 1;
		Element* current = cq->head;
		debug("at %d", *(int*)current->value);
		queue_element_debug(current);
		while (current->next != NULL) {
			debug("fuck %d", count);
			count++;
			current = current->next;
		}
		return count;
	}
}
