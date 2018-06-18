/** @file
 * Interfejs klasy wektor charów
 *
 * @author Miołaj Grzywacz
 * @copyright Uniwersytet Warszawski
 * @date 08.05.2018
 */

#ifndef SND_VHARVECTOR_H
#define SND_VHARVECTOR_H

#include <stdbool.h>
#include <stdlib.h>
#include <memory.h>

/**
 * Początkowa pojemność wektora
 */
#define startingCapacity 20

/**
 * Struktura wektora przechowującego chary
 * Zawiera tablicę charów aktualną ilość elementów i maksymalną
 * pojemność (po której będzie go trzeba przeskalować)
 * Struktura działa mimo wszstko jak skalujący się stos
 *
 */
struct vectorNode {
    unsigned int capacity; /**< Aktualna max pjemność */
    unsigned int elements; /**< Aktualna ilość elementów */
    char *arr; /**< Tablica czarów*/
};

typedef struct vectorNode *Vector;

/**
 * Tworzy pusty wektor
 * @return wskaźnik na pusty wektor
 */
Vector createEmptyVector();

/** @brief Dodaje element do wektora
 * Dodaje element @p c do wektora @p v w razie potrzeby go powiększając
 * @param[in] c - char do dodania
 * @param[in] v - wektor do którego mamy dodać char
 * @param[in] failure - ustawia bool na który dostaje wskąźnik na fals w przypdaku błędu
 */
void addElToVector(char c, Vector v, bool *failure);

/** @brief Usuwa ostatni element z wektora
 * @param[in] v - wektor z którego element ma być usunięty
 * @param[in] failure - ustawia bool na który dostaje wskąźnik na fals w przypdaku błędu
 */
void delElFromVec(Vector v, bool *failure);

/** @brief Usuwa wektor
 * @param[in] v - wektor do usunięcia
 */
void deleteVector(Vector v);

/** @brief zwraca String z wektora
 * Zwraca string przechowywany w tablicy @p arr wektora
 * @param[in] v - wektor z którego bierzemy string
 * @return zwracany String
 */
char *giveString(Vector v);

#endif //SND_VHARVECTOR_H