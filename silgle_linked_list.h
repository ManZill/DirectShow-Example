#ifndef SINGLE_LINKED_LIST_H
#define SINGLE_LINKED_LIST_H

#include <stdbool.h>
#include <stdint.h>



/**
 * @brief A node in a singly linked list.
 *
 * This structure represents a single node in a singly linked list, containing
 * a pointer to the data and a pointer to the next node in the list.
 *
 * @param data Pointer to the data stored in the node.
 * @param next Pointer to the next node in the list, or NULL if this is the last node.
 */
typedef struct node_s {
	void *data;
	struct node_s *next;
} node_t;

/**
 * @brief A singly linked list.
 *
 * This structure represents a singly linked list, maintaining pointers to the
 * head and tail nodes of the list for efficient access and modification.
 *
 * @param head Pointer to the first node in the list, or NULL if the list is empty.
 * @param tail Pointer to the last node in the list, or NULL if the list is empty.
 */
typedef struct single_linked_list_s {
	node_t *head;
	node_t *tail;
} single_linked_list_t;

/**
 * @brief Adds an item to the beginning of a singly linked list.
 * @param p_list Pointer to the singly linked list.
 * @param item Pointer to the data item to be added.
 * @return 0 on success, -1 if p_list is NULL, -2 if memory allocation fails.
 */
int add_item_to_begin(single_linked_list_t* p_list, void* item);

/**
 * @brief Adds an item to the end of a singly linked list.
 * @param p_list Pointer to the singly linked list.
 * @param item Pointer to the data item to be added.
 * @return 0 on success, -1 if p_list is NULL, -2 if memory allocation fails.
 */
int add_item_to_end(single_linked_list_t* p_list, void* item);

/**
 * @brief Initializes a singly linked list with a variable number of items.
 * @param pp_list Pointer to a pointer to the singly linked list to be initialized.
 * @param i_count Number of items to initialize the list with.
 * @param ... Variable arguments of type void* representing the items to add.
 * @return 0 on success, -1 if pp_list is NULL or memory allocation fails.
 */
int initialize_list(single_linked_list_t** pp_list, const int i_count, ...);

/**
 * @brief Deletes a singly linked list and frees all associated memory.
 * @param pp_list Pointer to a pointer to the singly linked list to be deleted.
 * @return 0 on success, -1 if pp_list or *pp_list is NULL.
 */
int delete_list(single_linked_list_t** pp_list);

/**
 * @brief Checks if a singly linked list is empty.
 * @param list Pointer to the singly linked list.
 * @return true if the list is empty, false otherwise.
 */
bool is_list_empty(const single_linked_list_t* list);

/**
 * @brief Retrieves the data item at a specified index in the singly linked list.
 * @param list Pointer to the singly linked list.
 * @param index The index of the item to retrieve (0-based).
 * @return Pointer to the data item if found, NULL if the list is empty or the index is out of bounds.
 */
void* get_list_element_at(const single_linked_list_t* list, uint32_t index);

#endif // !SINGLE_LINKED_LIST_H

