/** @file
* Implementacja klasy parser
*/

#include "parser.h"

/**
 * Macro na kod ASCII komentarzy
 */
#define comment 36

/**
 * Macro na sprawdzanie czy char jest liczbą
 */
#define testNum(x) ((x) >= '0' && (x) <= ';')
/**
 * Macro na sprawdzanie czy char jest literą
 */
#define testLetter(x) (((x) >= 'a' && (x) <= 'z') || ((x) >= 'A' && (x) <= 'Z'))
/**
 * Macro na sprawdzanie czy char jest literą lub liczbą
 */
#define testEither(x) (testNum(x) || testLetter(x))
/**
 * Macro na sprawdzanie czy char jest białym znakiem
 */
#define useless(x) ((x) == 32 || (x) == 13 || (x) == 10 || (x) == 9 || (x) == 36 || (x) == '\f' || (x) == '\v')

/**
 * Pozycja aktualnie parsowanego chara
 */
static long long characterNum = 0;

/**
 * @brief Sprawdzająca czy stdin jest pusty
 * Funkcja sprawdzająca czy stdin jest pusty, i gdy nie jest oddaje char z powrotem
 * na wejście
 * @return true gdy jest pusty, false wpp
 */
static bool inputEmpty() {
    int test = getc(stdin);
    if(test == EOF)
        return true;
    else{
        ungetc(test, stdin);
        return false;
    }
}

/**
 * @brief Tworzy pusty token
 * @return wskaźnik na zaalokowany token, lub null wp błędnej alokacji
 */
static Token createEmptyToken() {
    Token newToken = malloc(sizeof(struct token));

    return newToken;
}

/**
 * Ustawia tokenowi prametry
 * @param[in] t - token do ustawienia
 * @param[in] type - nowy typ tokenu
 * @param[in] str - nowy String dla tokenu
 * @param[in] begin - pozycja pierwszego znaku tokenu
 */
static void setToken(Token t, TokenType type, char *str, long long begin) {
    if (t == NULL)
        return;
    t->string = str;
    t->type = type;
    t->position = begin;
}

void deleteToken(Token givenToken) {
    if (givenToken != NULL) {
        if (givenToken->string != NULL)
            free(givenToken->string);
        free(givenToken);
    }
}

void deleteTokenWithoutStr(Token givenToken){
    if (givenToken != NULL)
        free(givenToken);
}

/**
 * @brief Wczytuje stdin do końca komentarza
 * Funkcja jest wywoływana po tym jak gdzieś zostanie otwarty komentarz &&
 * i będzie wczytywała wejście do znalezienia sekwencji && lub EOF
 * @return zwraca true jeśli komentarz ma zkończenie, false wpp
 */
static bool finishComment() {
    int curr;
    bool commentFinished = false;

    while (!inputEmpty()) {
        curr = getc(stdin);
        characterNum++;
        if (curr == comment) {
            if(inputEmpty())
                return false;
            curr = getc(stdin);
            characterNum++;
            if (curr == comment) {
                commentFinished = true;
                break;
            }
        }
    }

    if (commentFinished)
        return true;
    else
        return false;
}


/**
 * Funkcja wczytuje wszystkie białe znaki i komentarze do czasu napotkania czegoś innego lub EOF
 * w razie napotkania błędu ustawia to na co error wskazuje na true
 * @param[out] error - wskaźnik na false (do ew zmiany na true)
 * @return - false gdy wejście niespodziewanie się zkończyło true wpp
 */
static bool readTillToken(bool *error) {
    int curr;

    while (!inputEmpty()) {
        curr = getc(stdin);
        characterNum++;
        if(!(useless(curr))){
            ungetc(curr, stdin);
            characterNum--;
            return true;
        }
        if (curr == comment) {
            if (inputEmpty()) {
                *error = true;
                return true;
            }
            int helper = getc(stdin);
            characterNum++;
            if (helper != comment) {
                *error = true;
                characterNum--;
                return true;
            }
            if (!finishComment())
                return false;
        }
    }

    return true;
}

/**
 * @brief wczytuje String rerezentujący token
 * wczytuje String rerezentujący token dostając pierwszy string tokenu,
 * oraz informacje jakiego tokenu oczekiwaać numeru czy liter i numerów
 * @param first - pierwszy znak tokenu
 * @param which - gdzy true to funkcja wczyta najdłuższy możliwy numer,
 *  wpp wczyta najdłuźszy możliwy cią składający się z liter i cyfr
 * @return - zwraca wczytany ciąg znaków
 */
static char *readRestOfToken(char first, bool which) {
    Vector vec = createEmptyVector();
    bool error = false;
    char curr;

    if (vec == NULL)
        return NULL;
    addElToVector(first, vec, &error);
    if (error) {
        deleteVector(vec);
        return NULL;
    }

    while (!inputEmpty()) {
        curr = (char) getc(stdin);
        characterNum++;
        if (which ? testNum(curr) : testEither(curr)) {
            addElToVector(curr, vec, &error);
            if (error) {
                deleteVector(vec);
                return NULL;
            }
        } else {
            ungetc(curr, stdin);
            characterNum--;
            break;
        }
    }

    char *ret = giveString(vec);
    deleteVector(vec);
    if (ret == NULL)
        return NULL;
    return ret;
}

Token getToken() {
    Token returnToken = createEmptyToken();
    bool error = false;
    if (returnToken == NULL)
        return NULL;

    if (!readTillToken(&error)) {
        setToken(returnToken, errorEOF, NULL, -1);
        return returnToken;
    }

    if(error){
        setToken(returnToken, ERROR, NULL, characterNum);
        return returnToken;
    }

    if (inputEmpty()) {
        setToken(returnToken, normalEOF, NULL, characterNum);
        return returnToken;
    }

    char first = (char) getc(stdin);
    characterNum++;
    long long firstIdx = characterNum;

    ///Rozpartuje przypadki i określa jaki token zwrócić
    if (first == '?' || first == '>' || first == '@') {
        TokenType t;
        switch (first){
            case '?':
                t = write;
                break;
            case '>':
                t = directTo;
                break;
            default:
                t = tCounter;
                break;
        }
        setToken(returnToken, t, NULL, characterNum);
        return returnToken;
    } else if (testNum(first)) {
        char *number = readRestOfToken(first, true);
        if (number == NULL)
            return NULL;
        setToken(returnToken, num, number, firstIdx);
        return returnToken;
    } else if (testLetter(first)) {
        char *textId = readRestOfToken(first, false);
        if (textId == NULL)
            return NULL;
        if (strcmp(textId, "NEW") == 0) {
            free(textId);
            setToken(returnToken, NEW, NULL, firstIdx);
            return returnToken;
        } else if (strcmp(textId, "DEL") == 0) {
            free(textId);
            setToken(returnToken, DEL, NULL, firstIdx);
            return returnToken;
        } else {
            setToken(returnToken, id, textId, firstIdx);
            return returnToken;
        }
    } else {
        setToken(returnToken, ERROR, NULL, firstIdx);
        return returnToken;
    }
}

