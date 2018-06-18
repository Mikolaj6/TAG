/** @file
 * Implementacja klasy drzewo sortujące
 */

#include "sortingTrie.h"

/** @brief alokuje pamieć na node drzewa @p TrieNode
 * @return wskaźnik na node drzewa
 */

static TrieNode allocTrieNode() {
    TrieNode result = malloc(sizeof(struct trieNode));
    result->isSequence = false;
    for (int i = 0; i < numberOfKids; i++)
        result->kids[i] = NULL;

    return result;
}

SortTrie createEmptySortingTrie() {
    SortTrie result = malloc(sizeof(struct sortTrie));
    result->elements = 0;
    result->node = allocTrieNode();

    return result;
}

/** @brief usuwa całe drzewo
 * @param[in] nodeTrie korzeń drzewa do usunięcia
 */
static void deleteTrieNodes(TrieNode nodeTrie) {
    for (int i = 0; i < numberOfKids; i++) {
        if (nodeTrie->kids[i] != NULL)
            deleteTrieNodes(nodeTrie->kids[i]);
    }
    free(nodeTrie);
}

void deleteSortingTrie(SortTrie trie) {
    deleteTrieNodes(trie->node);
    free(trie);
}

void addStrings(SortTrie trie, const char *strL, const char *srtR) {
    int lghL = (int) strlen(strL);
    int lghR = (int) strlen(srtR);
    TrieNode currNode = trie->node;
    int index = 0;

    while (index < lghL) {
        if (currNode->kids[strL[index] - '0'] == NULL)
            currNode->kids[strL[index] - '0'] = allocTrieNode();
        currNode = currNode->kids[strL[index] - '0'];
        index++;
    }
    index = 0;
    while (index < lghR) {
        if (currNode->kids[srtR[index] - '0'] == NULL)
            currNode->kids[srtR[index] - '0'] = allocTrieNode();
        currNode = currNode->kids[srtR[index] - '0'];
        index++;
    }
    if (currNode->isSequence != true)
        trie->elements += 1;
    currNode->isSequence = true;
}

/** @brief rekurencyjnie przechodzi drzewo @p TrieNode
 * Przechodzi rekurencyjnie drzewo urzywając wektora jako stosu charów (status
 * stosu reprezentuje aktualny String) i jeśli napotka String będący jakimś przekierowaniem
 * (@p isSequence bedzie wtedy @p true) to dodaje go do tablicy i zwiększ @p indexptr o 1
 * @param[in] nodeTrie - drzewo do zapełnienia
 * @param[in] arr - tablica Stringów występujących w drzewie do zapełnienia
 * @param[out] indexptr - wskażnik na elementw w tablicy
 * @param[in] vec - wektor do przechowywania Stringa
 */
static void traverseSortingTrie(TrieNode nodeTrie, char **arr, int *indexptr, Vector vec, bool *failure) {
    if (nodeTrie->isSequence) {
        arr[*indexptr] = giveString(vec);
        if(arr[*indexptr] == NULL){
            *failure = true;
            return;
        }
        *indexptr += 1;
    }
    for (int i = 0; i < numberOfKids; i++) {
        if(*failure){
            return;
        }

        if (*failure == false && nodeTrie->kids[i] != NULL) {
            addElToVector(i + '0', vec, failure);
            if(*failure)
                return;
            traverseSortingTrie(nodeTrie->kids[i], arr, indexptr, vec, failure);
        }
    }
    delElFromVec(vec, failure);
}

char **createSortedArrFromTrie(SortTrie trie) {
    char **arr = malloc(sizeof(char *) * trie->elements);
    int index = 0;
    bool error = false;

    Vector vec = createEmptyVector();
    traverseSortingTrie(trie->node, arr, &index, vec, &error);
    deleteVector(vec);

    return arr;
}