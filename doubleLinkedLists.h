/** @file
 * Interfejs list dwukierunkowych
 *
 * @author Miołaj Grzywacz
 * @copyright Uniwersytet Warszawski
 * @date 08.05.2018
 */

#ifndef FIRST_DOUBLELINKEDLISTS_H
#define FIRST_DOUBLELINKEDLISTS_H

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>

/**
 * Struktura listy do przechowywania Stringów z atrapą z przodu
 * Ta dwustronność służy tylko temu aby usuwanie elementów było
 * w czasie stałym
 */
struct nodeLis {
    char *string; /**< String zawartość noda listy*/
    struct nodeLis *next; /**< Wskaźnik na następny element listy */
    struct nodeLis *prev; /**< Wskaźnik na poprzedni element listy */
};

typedef struct nodeLis *listD;

/** @brief tworzy pustą listę
 * @return wskażnik na tą listę
 */
listD createEmptyDList();

/** @brief usuwa cała listę
 * @param[in] lis - lista do usunięcia
 */
void delAllList(listD lis);

/** @brief usuwa element z listy
 *
 * @param[in] lis - lista do usunięcia
 */
void delNodeList(listD lis);

/** @brief dodaje String do listy
 *  dodaje String na początek listy
 * @param[in] lis - dana lista
 * @param[in] c - element do dodania
 * @return lista z dodanym elementem
 */
listD addFrontList(listD lis, const char * str);

/** @brief kopiuje String
 * alokuje pusty String długości i i przepisuje do niego wszystkie elementy z @p c
 * @param[in] i - dlugość Stringa do zaalokowania
 * @param[in] str - String do skopiowania
 * @return zwraca wskażnik na nowow zaalokowany String
 */
char *allocStr(int i, const char * str);

/** @brief sprawdza czy lista jest pusta
 * @param[in] lis - lista do sprawdzenia
 * @return @p true jeśli jest pusta, @p false wpp
 */
bool isEmpty(listD lis);

#endif //FIRST_DOUBLELINKEDLISTS_H