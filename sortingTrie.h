/** @file
 * Interfejs klasy trie do sortowania lekykograficznego dla getReverse
 *
 * @author Miołaj Grzywacz
 * @copyright Uniwersytet Warszawski
 * @date 08.05.2018
 */

#ifndef SND_SORTINGTRIE_H
#define SND_SORTINGTRIE_H

#include <stdbool.h>
#include <stdlib.h>
#include <memory.h>

#include "charVector.h"

/**
 * Ilość dzieci danego noda
 */
#define numberOfKids 12

/**
 * Tire służacy do sortowania leksykograficznego dla @p pfwdReverse
 */
struct trieNode{
    bool isSequence; /**< określa czy node reprezentuje String */
    struct trieNode *kids[numberOfKids]; /**< dzieci noda */
};

typedef struct trieNode * TrieNode;

/**
 * Struktura obudowująca drzewo składająca się
 * z ilości elementów w drzewie i korzenia
 */
struct sortTrie{
    int elements; /**< Ilość elementów w drzewie */
    TrieNode node; /**< Korzeń drzewa */
};

typedef struct sortTrie *SortTrie;

/** @brief tworzy pustą strukturę @p SortTrie
 * @return wskaźnik na tą strukturę
 */
SortTrie createEmptySortingTrie();

/**@brief usuwa strukturę @p SortTrie
 * @param[in] st - struktura do usunięcia
 */
void deleteSortingTrie(SortTrie st);

/** @brief dodaje String do drzewa
 * Dodaje String stworzony z połączenia @p sl i @p sr oraz zwiększa
 * @p elements o 1 jeśli nie było jeszcze takiego Stringa
 * @param[in] st - lewy String
 * @param[in] sl - prawy String
 * @param[in] sr - wskaźnik na drzewo Sortujące
 */
void addStrings(SortTrie st, const char * sl, const char * sr);

/** @brief wykonuje sortowanie leksykograficzne na drzewie
 * wykonuje sortowanie leksykograficzne na drzewie przechodząc je w porządku
 * infiksowym i zwraca tablicę Stringów posortowanych elementów
 * @param[in] st - drzewo sortujące
 * @return wskaźnik na początek tablicy Stringów
 */
char **createSortedArrFromTrie(SortTrie st);

#endif //SND_SORTINGTRIE_H