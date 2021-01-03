/*
 * author: Hongshuo Wang
 * email: hkw5146@psu.edu
 * dict.c for CMPSC 311 Fall 2019
 * last updated: 10/15/2019
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dict.h"

// count number of keys in a dict.
// since we have already had the dictlist pointer
// as the input.
// all we need is going through the list by
// updating current node 
// to the reference that its next points to.
// In each iteration, I use a conditional statement
// to check whether the key is null. IF it is, skip it.
// If not update the count. Finally output is stored 
// in the variable count which is the number
// of all the none null key in the list.
//
int countKeys(const dictNode *dict) {
  int count = 0;
  while (dict != NULL) {
    if (dict->key == NULL) {
      count = 0;
    }
    else {
      count = count + 1; 
    }
    dict = dict->next;
  }
  return count;
}

// given a key, look up its corresponding value in the
// dictionary, returns -1 if the value is not in the dictionary.
// your search for key should end when the key in the next node
// is bigger than the lookup key or you reached the end of the
// list.
//
// Just like I did in the previous question. I use a while loop
// to go through each node. In each iteration, I use the strcmp
// function to compare the current node's key with the input key
// which we need to look up.
// The strcmp is a function that take two const string and compare
// them bit by bit.
// If the first string is bigger than next string then the value
// it return > 0
// If the first string is equal to the second string it will return 0
// Otherwise, the value it return is always < 0
// When the function strcmp return 0. That mean the key is found.
// Just return the current node's value.
// If the output < 0, that means the key in the next node is bigger
// than the lookup key return -1
// IF we go through the end of our node, that mean the key is not in
// out link list, return -1.
// The update current node is as same as the previous function
// Just let current node point to its next points to
int lookupKey(const dictNode *dict, const char *key) {
  int output = 0;
  const char *temp = "";
  while (dict != NULL) {
    temp = strdup(dict->key);
    output = strcmp(temp, key);
    if (output > 0){
      return -1;
    } else if (output == 0) {
      return dict->value;
    }

    dict = dict->next;
  }
  return -1;
}

// delete the node in a dict with given key, return the value of
// the deleted node if it was found, return -1 if it wasn't found.
//
// Just like what I did before, we need to go through the node
// to check whether the key is in the linklist
// The difference is instead passing the dict node pointer
// as parameter
// passing a pointer that point to the dict node pointer
// Hence, we need assigned the *dictPtr which is the pointer
// that the pointer points to to be the first node of out list
// By the way, *t is the dictNode pointer that store the temporary
// value
// *temp is used to store temporary string
// r is used to store the value of the node
// output is to store the output of strcmp
// First, I use a if statement to handout when we need to delete
// the
// first node of out list.
// I just update the *dictPtr to the second node which is the next node
// of the first node,
// free the node and return the first node's value
//
// Now I use a while loop to handle when key which we want to
// delete is found in the middle.
// I defined a dictNode pointer *pre in advance.
// pre pointer always point to the node we last visited.
// I let the previous node points to the next node that
// current node's pointed to.
// Then free the node, and return the current's node value
// If the key in next node is bigger than lookupkey return -1
// If the key is not found return -1
// This time, we need both update previous node to the
// current node and update current node to the next node 
// in each iteration  
//
int deleteKey(dictNode **dictPtr, const char *key) {
  dictNode *current = NULL;
  dictNode *pre = NULL;
  dictNode *t = NULL;
  current = *dictPtr;
  const char *temp = "";
  int r = 0;
  int output = 0;

  if ((current != NULL) && (strcmp(strdup(current->key),key) == 0)) {
    r = current->value;
    *dictPtr = current->next;
    freeNode(current);
    return r;
  }

  while (current != NULL) {
    temp = strdup(current->key);
    output = strcmp(temp,key);
    if (output == 0) {
      t = current->next;
      pre->next = t;
      r = current->value;
      freeNode(current);
      return r; 
    } else if (output > 0){
      return -1;
    }
    pre = current; 
    current = current->next;
  } 
  return -1;

}

// given a key/value pair, first lookup the key in the dictionary,
// if it is already there, update the dictionary with the new
// value; if it is not in the dictionary, insert a new node into
// the dictionary, still make sure that the key is in alphabetical
// order.
// IMPORTANT: When creating a new node, make sure you dynamically
// allocate memory to store a copy of the key in the memory. You
// may use strdup function. DO NOT STORE the input key from the 
// argument directly into the node. There is no guarantee that key
// pointer's value will stay the same. 
// YOU MUST KEEP THE ALPHABETICAL ORDER OF THE KEY in the dictionary.
// 
// The function is almost the same.
// The difference is this time we need to define a new node and
// insert it into list if the key is not found.
// the variable I defined below are most as same as previous function.
// I define a newNode and use strdup function to copy the input key
// set the copied string to the newNode's key.
// If we didn't do this, there will be two pointer points to the same
// string. One of them change the other will also change.
// use malloc to create the memory to locate out newnode
// set input value as newNode's value
//
// Now, we need to insert our node in the list
// If our list is empty, just let *dictPtr points to the newNode
// If we need to insert our node at the end of list, just let the last
// previous node which points to the last node point to our newNode
// If we need to insert out node between two node,
// I use the while loop to check each node's key, if I found the key,
// then just update its value and return. If not, go through the 
// alphabetical order, just like we did before if out strcmp's output >0
// That means the next node is larger than input key. 
// We need to insert node before the current node
// just let the newNode points to the current node
// let the current node points to the newNode
// If we need to insert the node to the head of our list,
// just let *dictPtr points our newNode and let newNode points to the
// current node which is used to be first node.
// If the input key is null, just return
// do not insert the node in the list. 
// The rest things remain the same.
//
void addKey(dictNode **dictPtr, const char *key, int value) {
  dictNode *current = NULL;
  current = *dictPtr;
  dictNode *pre = NULL;
  const char *temp = "";
  int output = 0;
  
  if (key == NULL) {
    return;
  }

  dictNode *newNode = NULL;
  newNode = malloc(sizeof(dictNode));
  newNode->key = strdup(key);
  newNode->value = value;
  newNode->next = NULL;

  if (current == NULL) {
    *dictPtr = newNode;
    return;
  } 
  
  while (current != NULL) {
    temp = strdup(current->key);
    output = strcmp(temp,key);
    if (output > 0) {
      if ((pre == NULL) && (current == *dictPtr)){
        newNode->next = current;
        *dictPtr = newNode;
        return;
      } 
      pre->next = newNode;
      newNode->next = current;
      return;
    } else if (output == 0) {
      current->value = value;
      return;
    }
    pre = current;
    current = current->next;
  }
  pre->next = newNode;
  return;
}
