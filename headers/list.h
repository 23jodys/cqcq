#ifndef LIST_INCLUDED
#define LIST_INCLUDED

/**
 * @mainpage
 *
 * A basic list operation. Functions that can create new List_T objects
 * are noted. NULL represents an empty list and, for instance, pushing
 * to an empty list creates a new List_T object which must be freed by
 * the caller.
 *
 * Examples:
 *
 * Create a new List of strings
 *
 * List_t list = List_list("test1", "test2");
 *
 * Push an element onto a list
 *
 * List_t list = List_list("test1", "test2");
 * List_t list = List_push(list, "test3);
 *
 * Pop the first element from the list
 *
 * char* holder; 
 * List_t list = List_list("test1", "test2");
 * List_t list = List_pop(list, &holder);
 *
 */

typedef struct List_T {
	struct List_T* tail;
	void *value;
} List_T;

/** 
 * @brief Create new list using provided argument
 * @param first requires at least list item should be NULL if you want an empty list
 */
#define List_list(...) List_fromArray((void*[]){__VA_ARGS__, NULL})

/**
 * @brief Pop an element from the List_T
 * @param list to pop from
 * @param [out] value to retrieve
 * @returns New List_T
 */
#define List_pop(list, value) List_pop_private(list, (void*)&value)

/**
 * @brief Append an element to the tail of the list
 * @param [in] list
 * @param [in] tail element to add to end
 */
extern List_T* List_append (List_T* list, List_T* tail);

/**
 * @brief Copy a List_T
 * @param [in] list
 * @returns copied list, this is a "shallow" copy and nested lists would not be copied
 */
extern List_T* List_copy   (List_T* list);

/**
 * @brief Create new list using provided arguments, this is hidden because you _must_ set the last one to NULL, see List_list for a more convenient version
 */
extern List_T* List_fromArray(void*in[]);
extern List_T* List_pop_private(List_T* list, void **x);
extern List_T* List_push(List_T* list, void *x);
extern List_T* List_reverse(List_T* list);
extern int    List_length(List_T* list);
extern void   List_free(List_T** list);
extern void   List_map(List_T* list, void apply(void **x, void *cl), void *cl);
extern void **List_toArray(List_T* list, void *end);

#define INT(x) &(int){x}

#endif
