/** @file
 * Implementacja klasy listy dwukierunkowej
 */


#include "doubleLinkedLists.h"

/** @brief alokuje node listy
 * @return wskaźnik na ten node
 */
static listD alloc() {
    listD el = malloc(sizeof(struct nodeLis));

    return el;
}

listD createEmptyDList() {
    listD dummy = alloc();

    dummy->prev = dummy;
    dummy->next = dummy;
    dummy->string = NULL;

    return dummy;
}

char *allocStr(int lgh, const char *str) {
    char *newStr = (char *) malloc((lgh + 1) * sizeof(char));

    if(newStr != NULL){
        strcpy(newStr, str);
    }

    return newStr;
}

/** @brief zwalnia pamieć ze Stringa
 *
 * @param[in] str - String do zwolninia
 */
static void freeStr(char *str) {
    free(str);
}

void delNodeList(listD lis) {
    if(lis == NULL)
        return;

    listD nextNode = lis->next;
    listD prevNode = lis->prev;

    nextNode->prev = lis->prev;
    if (nextNode == lis)
        prevNode->next = prevNode;
    else
        prevNode->next = lis->next;

    freeStr(lis->string);
    free(lis);
}

void delAllList(listD lis) {
    listD firstEl = lis->next;

    while (firstEl->next != firstEl) {
        listD elToDelete = firstEl;
        firstEl = firstEl->next;
        delNodeList(elToDelete);
    }
    if (firstEl->prev != firstEl)
        delNodeList(firstEl);

    delNodeList(lis);
}

listD addFrontList(listD lis, const char *str) {
    if(lis == NULL){
        return NULL;
    }

    listD newNode = alloc();
    if(newNode == NULL){
        return NULL;
    }

    listD nextEl = lis->next;
    newNode->string = allocStr((int)strlen(str), str);
    if(newNode->string == NULL){
        free(newNode);
        return NULL;
    }

    newNode->prev = lis;
    lis->next = newNode;
    if (nextEl != lis) {
        newNode->next = nextEl;
        nextEl->prev = newNode;
    } else
        newNode->next = newNode;
    return newNode;
}

bool isEmpty(listD lis){
    if(lis->next == lis && lis->prev == lis)
        return true;
    else
        return false;
}