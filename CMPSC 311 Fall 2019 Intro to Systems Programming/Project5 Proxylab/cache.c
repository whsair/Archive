#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csapp.h"
#include "cache.h"

// StudentID: hkw5146
// Author: Hongshuo Wang

/* cache_init initializes the input cache linked list. */
// This function will initialize the list to an empty list by setting size to 0,
// and set first and last to NULL;
void cache_init(CacheList *list) {

  list->size = 0;
  list->first = NULL;
  list->last = NULL;
}

/* cache_URL adds a new cached item to the linked list. It takes the
 * URL being cached, a link to the content, the size of the content, and
 * the linked list being used. It creates a struct holding the metadata
 * and adds it at the beginning of the linked list.
 */

// This function will add a new node to the front of the doubly linked list.
// The hardest part is when wedon’t have enough space to cache this item, and we will have to evict stale items in our linked list.We keep the stale items at the tail of the list, we will keep evicting items from the last node in the list,and upate the cache size accordingly until we make enough room to add this new node with contentof given size.

void cache_URL(const char *URL, const char *headers, void *item, size_t size, CacheList *list) {

// Your code should also check the size of this item to see if it exceeded MAXOBJECTSIZE
// and if it does, free up the pointer item and don’t allocate this node.
  if (size > MAX_OBJECT_SIZE) {

    free(item);
    return;
  }

// strdup to allocateits own space to store the URL and headers
// store and own the pointer pointing to the dynamically
// allocated memory that stores the binary content of the file in item
// set the size of the node and update the overall size of the whole CacheList accordingly
  struct CachedItem *cached_item = malloc(MAX_OBJECT_SIZE);
  cached_item->url = strdup(URL);
  cached_item->headers = strdup(headers);
  cached_item->item_p = item;
  cached_item->size = size;
  cached_item->prev = NULL;
  cached_item->next = NULL;


// If the list is empty just update the list's first and last pointer
// to pointer to our create node
  if (list->size == 0) {
    list->size = cached_item->size;
    list->first = cached_item;
    list->last = cached_item;

  } else {

// Else we need to compare the size:
// if the sum of list size and the your created item's size is greater than
// MAX_CACHE_SIZE you should doing the evictation
    if ((list->size + cached_item->size) > MAX_CACHE_SIZE) {


// evicted_size is the variable that holds the size we plan to evicted_size
// which is also the size of last item of our node
      CachedItem *last_item;
      int evicted_size = 0;

// if last item in the list is null, some error happen just free up every memory
// and return
      if ((last_item = list->last) == NULL) {

        cache_destruct(list);
        free(cached_item->url);
        free(cached_item->headers);
        free(cached_item);
        return;

// Else, everything is fine
      } else {

// If there is only one item in the list update the first and last pointer
        if ((last_item->prev) == NULL) {

          cache_destruct(list);
          list->first = cached_item;
          list->last = cached_item;
          list->size = cached_item->size;
        } else {
// Else, (MAX_CACHE_SIZE - ((list->size) - evicted_size) denotes the size of free space
// after we evicted the last item of our list. If the free space's size is large than the
// created item's size, just get rid of the last item and add the created item to the front

          evicted_size = last_item->size;


          if ((MAX_CACHE_SIZE - ((list->size) - evicted_size)) >= cached_item->size) {

            free(last_item->url);
            free(last_item->headers);
            free(last_item->item_p);
            list->size -= last_item->size;
            last_item = last_item->prev;

            free(last_item->next);
            last_item->next = NULL;

// Else, the free space after one eviction is still not enough so we need to keep eviction until
// the free space is large enough or greater than our created item's size.
          } else {
            while ((MAX_CACHE_SIZE - ((list->size) - evicted_size)) < cached_item->size) {

              free(last_item->url);
              free(last_item->headers);
              free(last_item->item_p);
              list->size -= last_item->size;
              last_item = last_item->prev;
              evicted_size = last_item->size;


              free(last_item->next);
              last_item->next = NULL;

            }

// evict the last node

            free(last_item->url);
            free(last_item->headers);
            free(last_item->item_p);
            list->size -= last_item->size;
            last_item = last_item->prev;
            evicted_size = last_item->size;



            free(last_item->next);
            last_item->next = NULL;
          }


// Now, after we allocate enough free space in our list; we need to update the size.
          list->size += cached_item->size;

// after eviction, if the list is empty, just update our created items
          if (list->first == NULL) {
            list->first = cached_item;
            list->last = cached_item;
          } else {

// Else, if the list still contains node, just add our new node to the front
            cached_item->next = list->first;
            ((list->first)->prev) = cached_item;
            list->first = cached_item;

            list->last = last_item;

          }


        }




      }


// if our list have enough free space, add the created item normally to the front
    } else {

      cached_item->next = list->first;
      ((list->first)->prev) = cached_item;
      list->first = cached_item;
      list->size += cached_item->size;

    }

  }

// addition ended


  return;

}


/* find iterates through the linked list and returns a pointer to the
 * struct associated with the requested URL. If the requested URL is
 * not cached, it returns null.
 */
CachedItem *find(const char *URL, CacheList *list) {

  CachedItem* current;

// if the current node is NULL return null
  current = list->first;
  if (current == NULL) {
    return NULL;
  }

// if current node is the first node of our list, just return this node
// without update any location
  if (strcasecmp(current->url, URL) == 0) {
    return current;
  }

// go through each node in the list in order to look for the corresponding node
// depending on the url (case insensitively)
  while (current != NULL) {


    if (strcasecmp(current->url, URL) == 0) {

// if the node we wanted is the last node, update the list last pointer
// move it to the front then update the first pointer
      if (current == list->last) {

        list->last = ((list->last)->prev);
        (list->last)-> next = NULL;
        list->first->prev = current;
        current->prev = NULL;
        current->next = list->first;

        list->first = current;

      } else {

// Else, we find out node in the middle, we should update our pre and next node first
// move our node to the front of list then update the first node
        ((current->prev)->next) = current->next;
        ((current->next)->prev) = current->prev;
        current->next = list->first;
        ((list->first)->prev) = current;
        current->prev = NULL;
        list->first = current;

      }
      return current;

    }

// go to the next node
    current = current->next;

  }

// if nothing founded return null

  return NULL;
}


/* frees the memory used to store each cached object, and frees the struct
 * used to store its metadata. */

//This last function is to clean up the whole linked list and free all memory.
// It will not be used in your proxylab code since proxy server never terminates and you should never clean the cache,
void cache_destruct(CacheList *list) {


// if there nothing in the list, just return
  if (list->size == 0) {
    return;
  }

// create two pointers which point to current node and next node
  struct CachedItem *current = list->first;
  struct CachedItem *next = current->next;

// if current node is none return
  if (current == NULL) {
    return;

// Else, if there is only one node in the list free this node and its meta
  } else if ((next == NULL) && (current != NULL)){

    free(current->url);
    free(current->headers);
    free(current->item_p);
    list->size -= current->size;
    free(current);
    list->size = 0;
    list->first = NULL;
    list->last = NULL;

    return;
  }

// if there are 2 >= nodes in the list, we need to go through the list
// and free the node and their meta one by one.
  while (next != NULL) {

// we need to free there meta first, update the size and then free the node

    free(current->url);
    free(current->headers);
    free(current->item_p);

    list->size -= current->size;
    free(current);

// update current and next node
    current = next;
    next = current->next;

  }

// free the last node in the list

  free(current->url);
  free(current->headers);
  free(current->item_p);
  list->size -= current->size;

  free(current);

// free the list data and init it

  list->size = 0;
  list->first = NULL;
  list->last = NULL;



}
