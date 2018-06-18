/** @file
 * Implementacja klasy wektor charów
 */

#include "charVector.h"

/** @brief wytwarz pustą tablicę o długości @p startingCapacity
 * @return wskaźnik na tą tablicę
 */
static char *initializeArray() {
    char *arr = (char *) malloc(sizeof(char) * startingCapacity);

    if (arr != NULL) {
        for (int i = 0; i < startingCapacity; i++)
            arr[i] = '\0';
    }

    return arr;
}

Vector createEmptyVector() {
    Vector newVector = (Vector) malloc(sizeof(struct vectorNode));

    if(newVector != NULL){
        newVector->capacity = startingCapacity;
        newVector->elements = 0;
        newVector->arr = initializeArray();
        if(newVector->arr == NULL) {
            free(newVector);
            return NULL;
        }
    }

    return newVector;
}

/** @brief powiększa dwukrotnie tablicę wektora
 * @param[in] givenVector - dany wektor
 */
static bool biggerArray(Vector givenVector) {
    char *ptr = realloc(givenVector->arr, givenVector->capacity * 2);
    if(ptr == NULL)
        return false;

    givenVector->arr = ptr;
    for (unsigned int i = givenVector->capacity; i < givenVector->capacity * 2; i++)
        givenVector->arr[i] = '\0';
    givenVector->capacity *= 2;
    return true;
}

/** @brief pomniejsza dwukrotnie tablicę wektora
 * @param[in] givenVector - dany wektor
 */
static bool sizeDown(Vector givenVector) {
    char *ptr = realloc(givenVector->arr, givenVector->capacity / 2);
    if(ptr == NULL)
        return false;

    givenVector->arr = ptr;
    givenVector->capacity /= 2;
    int end = (givenVector->elements);
    for (int i = givenVector->capacity - 1; i >= end; i--)
        givenVector->arr[i] = '\0';

    return true;
}

void addElToVector(char node, Vector givenVector, bool *failure) {
    unsigned int current = givenVector->elements;

    if ((current + 1) >= givenVector->capacity){
        if(!biggerArray(givenVector)){
            *failure = true;
            return;
        }
    }

    givenVector->arr[current] = node;
    current++;
    givenVector->elements = current;
}

void delElFromVec(Vector givenVector, bool *failure) {
    if (givenVector->elements == 0)
        return;

    int coef = givenVector->capacity / (givenVector->elements + 1);
    if (givenVector->capacity >= 80 && coef >= 4)
        if(!sizeDown(givenVector)){
            *failure = true;
            return;
        }

    givenVector->arr[givenVector->elements - 1] = '\0';
    givenVector->elements -= 1;
}

char *giveString(Vector givenVector) {
    char *ret = malloc(sizeof(char) * (givenVector->elements + 1));

    if(ret != NULL) {
        for (int i = 0; i < (int) (givenVector->elements + 1); i++) {
            ret[i] = givenVector->arr[i];
        }
    }

    return ret;
}

void deleteVector(Vector vec){
    if(vec != NULL){
        free(vec->arr);
        free(vec);
    }
}