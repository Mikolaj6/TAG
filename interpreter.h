/** @file
 * Interfejs klasy pinterpreter
 *
 * @author Mikołaj Grzywacz
 * @date 08.05.2018
 */

#ifndef SND_INTERPRETER_H
#define SND_INTERPRETER_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "directoryBase.h"
#include "phone_forward.h"
#include "parser.h"

/**
 * Typ enum do określania możliwych typów stanu interpretera
 */
enum state {
    StateNEW, StateDEL, StateAwating, StateWrite, StateNumber, StateNumberDirect, StateTCount
};

typedef enum state State;

/**
 * Czyści pamięć w ze Stringa i aktualnego Tokena
 */
void clearMemStringToken();

/**
 * Funkcja do interpretacji wejścia
 */
void run();

#endif //SND_INTERPRETER_H
