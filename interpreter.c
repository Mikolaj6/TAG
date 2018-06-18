/** @file
* Implementacja klasy interpreter
*/

#include "interpreter.h"
#include <stdbool.h>

/*
 * Uwaga dotycząca użycia tych zmiennych jako globalne
 * Wszystki funkcje do obsługi przypadków korzystają z tych zmiennych
 * (są one tak naprawdę przypadkami dla funkci run, żeby ta nie była za długa)
 * Przekazywanie tych zmiennych jako wskaźników było by brzydkim rozwiązaniem
 * Ponadto zmienna są widocznie jedynie w pliku i jedynie
 * funkcje które z nich korzystają mają do nich dostęp
 */

static struct PhoneForward *curDir; //Aktualna baza przekierowań
static State currState;             //Aktualny stan
static int lastOperatorId;          //Id ostatniego operatora
static Token currToken;             //Aktualny token
static char *activeNumber;          //Numer przechowywany jako string(do przekierowań)


void clearMemStringToken() {
    if (currToken != NULL)
        deleteToken(currToken);
    if (activeNumber != NULL)
        free(activeNumber);
}


/**
 * Sytuacja gdzie wczytany został Token reprezentuje ID
 * Możliwe porzednie stany to NEW i DEL
 */
static void handleCaseId() {
    if (currState != StateNEW && currState != StateDEL) {
        fprintf(stderr, "ERROR %d\n", (int) currToken->position);
        exit(1);
    } else {
        if (currState == StateNEW) {
            curDir = addDirectory(currToken->string);
            if (curDir == NULL) {
                fprintf(stderr, "ERROR NEW %d\n", lastOperatorId);
                exit(1);
            } else {
                deleteToken(currToken);
                currToken = NULL;
                currState = StateAwating;
            }
        } else {
            bool isCurr = false;
            if (!deleteDirectory(currToken->string, curDir, &isCurr)) {
                fprintf(stderr, "ERROR DEL %d\n", lastOperatorId);
                exit(1);
            } else {
                if (isCurr)
                    curDir = NULL;
                currState = StateAwating;
                deleteToken(currToken);
                currToken = NULL;
            }
        }
    }
}

/**
 * Sytuacja gdzie wczytany został Token reprezentuje NEW lub DEL
 * Jedyny możliwy poprzedni stan to stan pusty
 * @param[in] t - typ wczytanego tokenu (NEW lub DEL)
 */
static void handleCaseNEWDEL(TokenType t) {
    if (currState != StateAwating) {
        fprintf(stderr, "ERROR %d\n", (int) currToken->position);
        exit(1);
    } else {
        currState = (t == NEW) ? StateNEW : StateDEL;
        lastOperatorId = (int) currToken->position;
        deleteToken(currToken);
        currToken = NULL;
    }
}

/**
 * Sytuacja gdzie wczytany został Token reprezentujący przekierowanie (>)
 * Jedyny możliwy poprzedni stan to numer
 */
static void handleCaseDirectTo() {
    if (currState != StateNumber) {
        fprintf(stderr, "ERROR %d\n", (int) currToken->position);
        exit(1);
    } else {
        currState = StateNumberDirect;
        lastOperatorId = (int) currToken->position;
        deleteToken(currToken);
        currToken = NULL;
    }
}

/**
 * Sytuacja gdzie wczytany został Token reprezentujący (?) (get lub rev)
 * Możliwe porzednie stany to numer i pusty
 */
static void handleCaseWrite() {
    if (currState == StateAwating) {
        currState = StateWrite;
        lastOperatorId = (int) currToken->position;
        deleteToken(currToken);
        currToken = NULL;
    } else if (currState == StateNumber) {
        if (curDir == NULL) {
            fprintf(stderr, "ERROR ? %d\n", (int) currToken->position);
            exit(1);
        }

        struct PhoneNumbers const *numToWrite = phfwdGet(curDir, activeNumber);
        if (numToWrite == NULL) {
            fprintf(stderr, "ERROR ? %d\n", (int) currToken->position);
            exit(1);
        }
        fprintf(stdout, "%s\n", phnumGet(numToWrite, 0));

        free(activeNumber);
        activeNumber = NULL;
        phnumDelete(numToWrite);
        deleteToken(currToken);
        currToken = NULL;
        currState = StateAwating;
    } else {
        fprintf(stderr, "ERROR %d\n", (int) currToken->position);
        exit(1);
    }
}

/**
 * Sytuacja gdzie wczytany został Token reprezentujący numer
 * mógł zostać wczytany poprawnie w 5 sytuacjach:
 * Gdy aktualny stan był pusty, aktualny stan był przepisaniem (num>)
 * Aktualny stan to DEL, aktualny stan to pisanie (?)
 * Aktualny stan to (@)
 * Każdy stan jest odpowiednio obsłużony
 */
static void handleCaseNum() {
    if (currState == StateAwating) {
        currState = StateNumber;
        activeNumber = currToken->string;
        deleteTokenWithoutStr(currToken);
        currToken = NULL;
    } else if (currState == StateNumberDirect) {
        if (curDir == NULL) {
            fprintf(stderr, "ERROR > %d\n", lastOperatorId);
            exit(1);
        }
        if (!phfwdAdd(curDir, activeNumber, currToken->string)) {
            fprintf(stderr, "ERROR > %d\n", lastOperatorId);
            exit(1);
        }

        free(activeNumber);
        activeNumber = NULL;
        currState = StateAwating;
        deleteToken(currToken);
        currToken = NULL;
    } else if (currState == StateWrite) {
        if (curDir == NULL) {
            fprintf(stderr, "ERROR ? %d\n", lastOperatorId);
            exit(1);
        }

        const struct PhoneNumbers *numToWrite = phfwdReverse(curDir, currToken->string);
        if (numToWrite == NULL) {
            fprintf(stderr, "ERROR ? %d\n", lastOperatorId);
            exit(1);
        }
        for (size_t i = 0; i < (size_t) numToWrite->length; i++) {
            const char *numb = phnumGet(numToWrite, i);
            fprintf(stdout, "%s\n", numb);
        }

        phnumDelete(numToWrite);
        deleteToken(currToken);
        currToken = NULL;
        currState = StateAwating;
    } else if (currState == StateDEL) {
        if (curDir == NULL) {
            fprintf(stderr, "ERROR DEL %d\n", lastOperatorId);
            exit(1);
        }

        phfwdRemove(curDir, currToken->string);
        deleteToken(currToken);
        currToken = NULL;
        currState = StateAwating;
    } else if (currState == StateTCount){
        size_t lgh = strlen(currToken->string);
        size_t finalLgh;
        if(12 > lgh)
            finalLgh = 12;
        else
            finalLgh = lgh;
        size_t res = phfwdNonTrivialCount(curDir, currToken->string, (finalLgh - 12));
        fprintf(stdout, "%zu\n", res);
        deleteToken(currToken);
        currToken = NULL;
        currState = StateAwating;
    } else {
        fprintf(stderr, "ERROR %d\n", (int) currToken->position);
        exit(1);
    }
}

/**
 * Sytuacja gdzie wczytany został Token reprezentujący TrivialCount (@)
 * Jedyny możliwy poprzedni stan to stan oczekiwania
 */
static void handleCaseTCounter(){
    if (currState != StateAwating) {
        fprintf(stderr, "ERROR %d\n", (int) currToken->position);
        exit(1);
    } else {
        currState = StateTCount;
        lastOperatorId = (int) currToken->position;
        deleteToken(currToken);
        currToken = NULL;
    }
}

void run() {
    curDir = NULL;
    currState = StateAwating;
    lastOperatorId = 0;
    currToken = NULL;
    activeNumber = NULL;

    //Wczytujemy tokeny tak długo aż napottkamy błąd lub poprawny EOF(normal EOF)
    while (true) {
        currToken = getToken();
        if (currToken == NULL) {
            fprintf(stderr, "ERROR %d\n", lastOperatorId);
            exit(1);
        }
        //Switch po typie tokena
        switch (currToken->type) {
            case ERROR:
                fprintf(stderr, "ERROR %d\n", (int) currToken->position);
                exit(1);
            case errorEOF:
                fprintf(stderr, "ERROR EOF\n");
                exit(1);
            case normalEOF: //Normalne zakończenie wejścia
                if (currState == StateAwating)
                    exit(0);
                else {
                    fprintf(stderr, "ERROR EOF\n");
                    exit(1);
                }
            case tCounter:
                handleCaseTCounter();
                break;
            case id:
                handleCaseId();
                break;
            case NEW:
                handleCaseNEWDEL(NEW);
                break;
            case DEL:
                handleCaseNEWDEL(DEL);
                break;
            case directTo:
                handleCaseDirectTo();
                break;
            case write:
                handleCaseWrite();
                break;
            case num:
                handleCaseNum();
                break;
        }
    }
}