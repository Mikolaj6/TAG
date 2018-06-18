/** @file
 * Interfejs klasy parser
 *
 * @author Mikołaj Grzywacz
 * @date 08.05.2018
 */

#ifndef SND_PARSER_H
#define SND_PARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "charVector.h"

/**
 * Typ enum do określania możliwych typów Tokenu
 */
enum tokenType {
    NEW, DEL, num, directTo, write, id, tCounter, errorEOF, normalEOF, ERROR
};

typedef enum tokenType TokenType;

/**
 * Struktura Tokenu
 */
struct token {
    TokenType type; /**< Typ tokenu */
    char *string; /**< String który określa token */
    long long position; /**< pozycja pierwszego znaku tokenu */
};

typedef struct token *Token;

/**
 * @brief Funkcja zwraca wczytany token
 * Funkcja korzysta z poprzednich funkci do wczytania Stringa reprezentującego
 * token a następnie określa jaki to Token i go zwraca
 * @return - token do zwrotu NULL gdy zabraknie pamięci
 */
Token getToken();

/**
 * Usuwa token i jego string jeśli ten jest różny od null
 * @param[in] givenToken - przyjmuje token do usunięcia
 */
void deleteToken(Token givenToken);

/**
 * Usuwa token bez Stringa
 * @param[in] givenToken - przyjmuje token do usunięcia
 */
void deleteTokenWithoutStr(Token givenToken);

#endif //SND_PARSER_H
