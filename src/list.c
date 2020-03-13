#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>

#include "dbg.h"
#include "list.h"

#define List_new_node(new, store) do {       \
	new = malloc(sizeof(List_T)); \
	check_mem(new);                      \
	new->tail = NULL;                  \
	new->value = store; } while(0) 

List_T* List_pop_private(List_T* list, void** value) {
	// @todo should it be a runtime error to pop from an empty list?
	if (list == NULL) {
		return NULL;
	}
	debug("List_pop_private: list->value is %d", *(int*)list->value);
	*value = (void*)list->value;
	if (list->tail != NULL) {
		List_T* toreturn = list->tail;
		list->tail = NULL;
		debug("List_pop_private: freeing list element we just popped");
		List_free(&(list));
		return toreturn;
	} else {
		List_free(&(list));
		return NULL;
	}
}
List_T* List_push(List_T* list, void* value) {
	List_T* new;
       	List_new_node(new, value);
	new->value = value;
	new->tail = list;
	return new;
error:
	debug("failed to allocate memory");
	return NULL;
}

List_T* List_fromArray(void* in[]) {
	List_T* last = NULL;
	List_T* first = NULL;
	debug("List_fromArray: starting loop");
	for(int i=0; in[i] != NULL; i++) {
		//debug("List_fromArray: value of in[%d] is '%s'", i, (char*)in[i]);
		debug("List_fromArray: value of in[%d] is '%d'", i, *(int*)in[i]);
		List_T* new;
		List_new_node(new, in[i]);
		if (last != NULL) {
			last->tail = new;
		} else {
			first = new;
		}
		last = new;
	}
	return first;

error:
	/* In this case list was NULL, return it */
	debug("unable to allocate memory in List_list");
	return last;
}

void List_free(List_T** list) {
	if((*list) == NULL) {
		debug("List_free: returning right away because pointer was NULL");
		return;
	}
	if((*list)->tail != NULL) {
		debug("List_free: tail is not NULL, recursive call");
		List_free(&(*list)->tail);
	}
	debug("List_free: freeing this list");
	(*list)->value = NULL;
	(*list)->tail = NULL;
	free(*list);
	*list = NULL;
}

List_T* List_append(List_T* list, List_T* tail) {
	/* [list, tail := newlist || list, I]*/
	if (list == NULL) {
		list = tail;
	} else {
		List_T* iterator;
		iterator = list;
		while(iterator->tail != NULL) {
			iterator = iterator->tail;
		}
		iterator->tail = tail;
	}



	return list;
}

List_T* List_copy(List_T* list) {
	/* pointer to head of copied list */
	List_T* first = NULL;

	/* pointer to previous element in copied list */
	List_T* prev = NULL;

	/* iterator through the cloned list */
	List_T* iterator = NULL;


	if (list != NULL) {
		List_new_node(first, list->value);
		debug("List_copy: original list value is %d", *(int*)list->value);
		debug("List_copy: created *node %p", first);
		prev = first;
		/*
		 * Iteration 0
		 *
		 * prev = iterator_0
		 *
		 * Iteration 1
		 *
		 * iterator_0(prev)->tail = iterator_1
		 * prev = iterator_1
		 *
		 * Iteration 2
		 *
		 * iterator_1(prev)->tail = iterator_2
		 * prev = iterator_2
		 */
		if (list->tail != NULL) {
			/* skip first element since we already created it */
			list = list->tail;
			for(;list != NULL; list = list->tail) {
				debug("List_copy: original list value is %d", *(int*)list->value);
				List_new_node(iterator, list->value);
				debug("List_copy: created *node %p", iterator);
				debug("List_copy: prev != first, setting prev(%p)->tail = iterator(%p)", prev, iterator);
				prev->tail = iterator;

				prev = iterator;

			}
		}
	}

	return first;
error:
	debug("failed to allocate memory");
	return NULL;
}

int List_length(List_T* list) {
	debug("in list length");
	int count = 0;
	if (list != NULL) {
		count = 1;
		while (list->tail != NULL) {
			count++;
			list = list->tail;
		}
	} else {
		debug("list == NULL");
	}
	debug("returning %d", count);
	return count;
}
