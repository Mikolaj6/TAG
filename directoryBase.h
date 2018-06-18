/** @file
 * Interfejs klasy implementującej bazę baz przekierowań
 *
 * @author Mikołaj Grzywacz
 * @date 08.05.2018
 */

#ifndef SND_DIRECTORYBASE_H
#define SND_DIRECTORYBASE_H

#include "phone_forward.h"

/**
 * Struktura noda bazy baz
 * Jest to struktura Left-Child Right-Sibling
 * Node zerowy to '!' i wszystki kolejne to jego dzieci
 * W wypadku braku dzieci lub braci wskźniki są na siebie
 * Node zero ma wskaźnik na ojca równy sobie
 */
struct dirBase{
    struct PhoneForward *directory; /**< przekierowanie dla noda */
    char nodeVal; /**< Char reprezentujący noda */
    struct dirBase * father; /**< Wskźnik na ojca */
    struct dirBase * brother; /**< Wskaźnik na brata */
    struct dirBase * son; /**< Wskaźnik na syna */
};

typedef struct dirBase *DirectoryBase;

/**
 * Tworzy nową bazę baz (tylko raz)
 */
void createDirectoryBase();

/**
 * Usuwa bazę baz (atexit)
 */
void delDirBase();

/**
 * @brief Dodaje bazę przekierowań
 * Dodaje baze przekierowań o reprezentacji Stringa (tworzy pustą),
 * lub jeśli taka była wcześniej dodana to zwraca na nią wskaźnik
 * @param[in] name - String reprezentujący nową bazę
 * @return wskaźnik do bazy przekierowań
 */
struct PhoneForward *addDirectory(char *name);

/**
 * @brief Usuwa bazę przekierowań
 * Usuwa bazę przekierowań gdy taka istnieje
 * @param[in] name - String reprezentujący bazę do usunięcia
 * @param[in] dir - aktualna baza przekierowań
 * @param[out] isCurr - wskaźnik na bool który ma określić czy usunięto akt przekierowanie
 * @return true usunięcie się powiodało, wpp false
 */
bool deleteDirectory(char *name, struct PhoneForward *dir, bool *isCurr);

#endif //SND_DIRECTORYBASE_H
