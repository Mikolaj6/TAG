/** @file
* Plik main
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "interpreter.h"

/**
 * Funkcja do czyszczenia pamięci dla atexit
 */
void freeAtExit(){
    clearMemStringToken();
    delDirBase();
}

int main() {

    createDirectoryBase();
    atexit(freeAtExit);
    run();

}