#include "silgle_linked_list.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>



int add_item_to_begin(single_linked_list_t* p_list, void* item) 
{
    node_t* new_node = NULL;
    if (p_list == NULL)
    {
        return -1;
    }

    new_node = (node_t*)malloc(sizeof(node_t));
    if (new_node == NULL)
    {
        return -2;
    }

    new_node->data = item;
    new_node->next = p_list->head;
    p_list->head = new_node;
    if (p_list->tail == NULL)
    {
        p_list->tail = new_node;
    }

    return 0;
}

int add_item_to_end(single_linked_list_t* p_list, void* item) 
{
    node_t* new_node = NULL;
    if (p_list == NULL)
    {
        return -1;
    }

    if (p_list->tail == NULL)
    {
        return add_item_to_begin(p_list, item);
    }

    new_node = (node_t*)malloc(sizeof(node_t));
    if (new_node == NULL)
    {
        return -2;
    }

    new_node->data = item;
    new_node->next = NULL;
    p_list->tail->next = new_node;
    p_list->tail = new_node;
    return 0;
}

int initialize_list(single_linked_list_t** pp_list, const int i_count, ...) 
{
    single_linked_list_t* p_list = NULL;
    va_list args = NULL;

    if (pp_list == NULL)
    {
        return -1;
    }

    p_list = (single_linked_list_t*)malloc(sizeof(single_linked_list_t));
    if (p_list == NULL)
    {
        fwprintf(stderr, L"Error: Failed to allocate memory for linked list.\n");
        return -1;
    }
    p_list->head = p_list->tail = NULL;

    va_start(args, i_count);
    for (int i = 0; i < i_count; i++)
    {
        void* item = va_arg(args, void*);
        add_item_to_end(p_list, item);
    }
    va_end(args);

    *pp_list = p_list;
    return 0;
}

int delete_list(single_linked_list_t** pp_list) 
{
    single_linked_list_t* p_list = NULL;
    node_t* current = NULL;

    if (pp_list == NULL || *pp_list == NULL)
    {
        return -1;
    }

    p_list = *pp_list;
    current = p_list->head;
    while (current)
    {
        node_t* next = current->next;
        free(current);
        current = next;
    }
    free(p_list);
    *pp_list = NULL;
    return 0;
}

bool is_list_empty(const single_linked_list_t* list) 
{
    return list->head == NULL;
}

void* get_list_element_at(const single_linked_list_t* list, const uint32_t index) 
{
    node_t* current = NULL;
    uint32_t current_index = 0;

    if (list->head == NULL) 
    {
        return NULL;
    }

    current = list->head;
    while (current != NULL && current_index < index) 
    {
        current = current->next;
        current_index++;
    }

    return (current != NULL) ? current->data : NULL;
}
