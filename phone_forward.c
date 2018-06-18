/** @file
 * Implementacja klasy przechowującej numery telefonów
 */

#include "phone_forward.h"

/** @brief Alokuje pamieć na @p PhoneForward
 * @return wskaźnik na zalokowany element lub NULL
 */
struct PhoneForward *alloc() {
    struct PhoneForward *el = (struct PhoneForward *) malloc(sizeof(struct PhoneForward));

    return el;
}

/** @brief tworzy pusty node do @p PhoneForward
 * @return wskaźnik na zalokowany element lub NULL
 */
struct PhoneForward *createEmpty() {
    struct PhoneForward *newDirectory = alloc();

    if (newDirectory != NULL) {
        newDirectory->directedTo = NULL;
        newDirectory->directedFrom = NULL;
        newDirectory->ptrToListElem = NULL;
        for (int i = 0; i < numberOfKids; i++)
            newDirectory->children[i] = NULL;
        newDirectory->father = NULL;
        newDirectory->nodeDirectedFrom = NULL;
    }

    return newDirectory;
}

/** @brief określa czy node można usunąć
 * Jeśłi node ma wszystkie składowe równe NULL to można go usunąć
 * @param[in] node - wskaźnik na dany node
 * @return @p true jeśli można go usunąć, @p false w przeciwnym razie
 */
static bool uselessNode(struct PhoneForward *node) {
    bool p0 = node->father != NULL;
    bool p1 = node->ptrToListElem == NULL;
    bool p2 = node->directedFrom == NULL;
    bool p3 = node->directedTo == NULL;
    bool p4 = node->nodeDirectedFrom == NULL;
    bool p5 = true;
    for (int i = 0; i < numberOfKids; i++) {
        if (node->children[i] != NULL)
            p5 = false;
    }

    if (p0 && p1 && p2 && p3 && p4 && p5)
        return true;
    else
        return false;
}

/** @brief sprawdza czy drzewo jest puste
 * @param[in] node - wskaźnik na dany node
 * @return @p true jeśli jest puste, @p false w przeciwnym razie
 */
static bool endingPostition(struct PhoneForward *node) {
    bool p0 = node->father == NULL;
    bool p5 = true;
    for (int i = 0; i < numberOfKids; i++) {
        if (node->children[i] != NULL)
            p5 = false;
    }

    if (p0 && p5)
        return true;
    else
        return false;
}

/** @brief ustawia @p son na NULL
 * Przechodzi przez dzieci @p father i jeśli napotka @p son to ustawia go na NULL
 * @param[in] father - wskażnik na node ojca
 * @param[in] son - wskaźnik na node dziecka
 */
static void setSonToNull(struct PhoneForward *father, struct PhoneForward *son) {
    int i = 0;
    while (i < numberOfKids) {
        if (father->children[i] == son) {
            father->children[i] = NULL;
            break;
        }
        i++;
    }
}

/** @brief usuwa niepotrzebne nody do góry
 * Dopóki @ref uselessNode jest true usuwa node i powtarza to samo dla ojca
 * @param[in] node - wskażnik na początkowy node
 * @return zwraca @p true jeśli coś usunął @p false wpp
 */
bool deleteUntil(struct PhoneForward *node) {
    bool ret = false;
    if (node == NULL)
        return ret;
    struct PhoneForward *currNode = node;

    while (uselessNode(currNode)) {
        struct PhoneForward *nodeToDel = currNode;
        currNode = currNode->father;
        setSonToNull(currNode, nodeToDel);
        nodeToDel->father = NULL;
        free(nodeToDel);
        ret = true;
    }

    return ret;
}

struct PhoneForward *phfwdNew(void) {
    struct PhoneForward *newdirectory = createEmpty();

    return newdirectory;
}

/**
 * Struktura reprezentująca pary
 */
struct pair {
    struct PhoneForward *pf; /**< Pole @p  PhoneForward*/
    int num; /**< Pole @p int */
};

/** @brief przemieszcza się do noda @p str
 * Przechodzi do noda @p str zaczynając od początku struktury danych
 * @param[in] str - String do którego chcemy dojść
 * @param[in] pf - całe drzewo
 * @return wskażnik na node do którego prowadzi @p str
 */
static struct pair *moveToNode(char const *str, struct PhoneForward *pf) {
    struct PhoneForward **currKids = pf->children;
    int index = 0;

    while (str[index] != '\0' && currKids[str[index] - '0'] != NULL) {
        pf = currKids[str[index] - '0'];
        currKids = pf->children;
        index += 1;
    }

    struct pair *result = malloc(sizeof(struct pair));
    result->num = index;
    result->pf = pf;

    return result;
}

/** @brief dodaje element @p str do @p givenDirectory
 * Dodje nody ze @p str do @p givenDirectory jeśli trzeba jak nie to przechodzi po
 * dotychczasowych nodach, a na końcu zwraca końcowy node
 * @param[in] str - String do dodania
 * @param[in] givenDirectory - drzewo do którego mamy go dodać
 * @return wskażnik na końcowy node
 */
struct PhoneForward *addEl(char const *str, struct PhoneForward *givenDirectory) {
    struct pair *moveTo = moveToNode(str, givenDirectory);
    struct PhoneForward *currDirectory = moveTo->pf;
    int index = moveTo->num;
    free(moveTo);
    int currNum;

    while (str[index] != '\0') {
        currNum = str[index] - '0';
        struct PhoneForward *father = currDirectory;
        currDirectory = createEmpty();

        if (currDirectory == NULL) {
            deleteUntil(father);
            return NULL;
        }
        currDirectory->father = father;
        father->children[currNum] = currDirectory;
        index++;
    }

    return currDirectory;
}

/** @brief określa czy String jest liczbą
 * @param[in] num1 - String do rozpatrzenia
 * @return @p true jeśli jest liczbą, @p false wpp
 */
static bool isNum(char const *num1) {
    if (num1 == NULL)
        return false;
    if (num1[0] == '\0')
        return false;
    int index = 0;
    while (num1[index] != '\0') {
        if (num1[index] < 48 || num1[index] > 59)
            return false;
        index++;
    }

    return true;
}

/** @brief określa czy Stringi są takie same
 * @param[in] num1 - pierwszy String
 * @param[in] num2 - drugi String
 * @return @p true jeśli są, @p false wpp
 */
static bool sameStrings(char const *num1, char const *num2) {
    int index = 0;
    while (num1[index] != '\0' || num2[index] != '\0') {
        if (num1[index] != num2[index])
            return false;
        index++;
    }

    return true;
}

bool phfwdAdd(struct PhoneForward *pf, char const *num1, char const *num2) {
    if (!isNum(num1) || !isNum(num2) || sameStrings(num1, num2) || pf == NULL)
        return false;
    struct PhoneForward *fromNum1 = addEl(num1, pf);
    struct PhoneForward *fromNum2 = addEl(num2, pf);

    if (fromNum1 != NULL && fromNum2 != NULL) {
        if (fromNum2->directedFrom == NULL) {
            fromNum2->directedFrom = createEmptyDList();
        }
        if (fromNum1->directedTo != NULL) {
            free(fromNum1->directedTo);
            fromNum1->directedTo = NULL;
            delNodeList(fromNum1->ptrToListElem);
            fromNum1->ptrToListElem = NULL;
            if (isEmpty(fromNum1->nodeDirectedFrom->directedFrom) && fromNum1->nodeDirectedFrom != fromNum2) {
                delAllList(fromNum1->nodeDirectedFrom->directedFrom);
                fromNum1->nodeDirectedFrom->directedFrom = NULL;
            }
            deleteUntil(fromNum1->nodeDirectedFrom);
        }
        fromNum1->nodeDirectedFrom = fromNum2;
        fromNum1->directedTo = allocStr((int) strlen(num2), num2);
        fromNum1->ptrToListElem = addFrontList(fromNum2->directedFrom, num1);
        return true;
    } else {
        deleteUntil(fromNum1);
        deleteUntil(fromNum2);
        return false;
    }
}

/** @brief zwraca pierwsze dziecko @p pf
 * @param[in] pf - node którego dziecko ma byc zwrócone
 * @return wskaźnik na pierwsze dziecko @p pf
 */
static struct PhoneForward *getFirstSon(struct PhoneForward *pf) {
    for (int i = 0; i < numberOfKids; i++) {
        if (pf->children[i] != NULL)
            return pf->children[i];
    }

    return NULL;
}

/** @brief zwalnia pamięć ze składowych @p pf
 * Usuwa String i listę elementów
 * @param[in] pf - drzewo do usunięcia
 */
static void freeNodeElements(struct PhoneForward *pf) {
    if (pf == NULL)
        return;
    if (pf->directedFrom != NULL)
        delAllList(pf->directedFrom);
    if (pf->directedTo != NULL)
        free(pf->directedTo);
}

void phfwdDelete(struct PhoneForward *pf) {
    if (pf == NULL)
        return;
    struct PhoneForward *currDirectory = pf;
    while (!endingPostition(currDirectory)) {
        struct PhoneForward *firstSon = getFirstSon(currDirectory);
        if (firstSon == NULL) {
            struct PhoneForward *nodeToDel = currDirectory;
            currDirectory = currDirectory->father;
            setSonToNull(currDirectory, nodeToDel);
            freeNodeElements(nodeToDel);
            free(nodeToDel);
        } else
            currDirectory = firstSon;
    }
    free(currDirectory);
}

/** @brief Określa czy @pf ma dzieci po @p num
 * Jeśli istnieje dziecko @p pf, po @p num to zwraca @p true, jak nie to zwraca @p false
 * @param[in] pf - struktura przekowująca przekierowania
 * @param[in] num - numer dziecka, liczymy od następnego
 * @return - @p true lub @p false w zależności czy dziecko istnieje
 */
static bool lastChild(struct PhoneForward *pf, int num) {
    bool res = true;
    while (num < numberOfKids) {
        if (pf->children[num] != NULL) {
            res = false;
            break;
        }
        num++;
    }

    return res;
}

/** @brief rekurencyjna funkcja pomocnicza do @ref phfwdRemove
 * Usuwa wszystkie składowe @p pf jak również usuwa nic nie wnoszące
 * ścieżki przekierowań za pomacą @ref deleteUntil a następnie odpala
 * się rekurencyjnie dla swoich dzieci
 * @param[in] pf - node w drzewie
 */
static void aux(struct PhoneForward *pf) {
    ///Usuwa elementy z noda oraz z noda na którego przekierowuje
    if (pf->directedTo != NULL) {
        free(pf->directedTo);
        pf->directedTo = NULL;
    }
    if (pf->ptrToListElem != NULL) {
        delNodeList(pf->ptrToListElem);
        pf->ptrToListElem = NULL;
        if (isEmpty(pf->nodeDirectedFrom->directedFrom)) {
            delAllList(pf->nodeDirectedFrom->directedFrom);
            pf->nodeDirectedFrom->directedFrom = NULL;
            deleteUntil(pf->nodeDirectedFrom);
        }
        pf->nodeDirectedFrom = NULL;
    }

    if (deleteUntil(pf))
        return;

    int i = 0;
    while (i < numberOfKids) {
        if (pf->children[i] != NULL) {
            if (lastChild(pf, i + 1)) {
                aux(pf->children[i]);
                break;
            } else {
                struct PhoneForward *father = pf->father;
                pf->father = NULL;
                aux(pf->children[i]);
                pf->father = father;
            }
        }
        i++;
    }
}


void phfwdRemove(struct PhoneForward *pf, char const *num) {
    if (!isNum(num) || pf == NULL)
        return;
    struct pair *moveTo = moveToNode(num, pf);
    struct PhoneForward *currDir = moveTo->pf;
    int index = moveTo->num;
    free(moveTo);

    if (num[index] == '\0') {
        struct PhoneForward *father = currDir->father;
        currDir->father = NULL;
        aux(currDir);
        currDir->father = father;
    }
    deleteUntil(currDir);
}

/** @brief tworzy pustą strukturę @p PhoneNumbers
 * @return wskaźnik tę strukturę
 */
static struct PhoneNumbers *createEmptyPhoneNumbers() {
    struct PhoneNumbers *ret = (struct PhoneNumbers *) malloc(sizeof(struct PhoneNumbers));
    ret->length = 0;
    ret->numbersArr = NULL;

    return ret;
}

/** @brief tworzy jednoelementową strukturę @p PhoneNumbers
 * @return wskaźnik tę strukturę
 */
static struct PhoneNumbers *createOnePhoneNumber(const char *str) {
    struct PhoneNumbers *ret = (struct PhoneNumbers *) malloc(sizeof(struct PhoneNumbers));
    ret->length = 1;
    ret->numbersArr = malloc(sizeof(char *));
    ret->numbersArr[0] = (char *) str;

    return ret;
}

/** @brief tworzy wieloelementową strukturę @p PhoneNumbers
 * Z otrzymaną tablicę stringów wraz z jej długościa wkłada do struktury
 * @param[in] str - tablica Stringów
 * @param[in] num - ilość elementów w tablicy @p str
 * @return wskaźnika na tę strukturę
 */
static struct PhoneNumbers *createMultiplePhoneNumber(char **str, int num) {
    struct PhoneNumbers *ret = (struct PhoneNumbers *) malloc(sizeof(struct PhoneNumbers));
    ret->length = num;
    ret->numbersArr = str;

    return ret;
}

/** @brief łączy dwa Stringi w jeden
 * Alokuje pamieć na String o długości takiej jak suma dwóch danych i
 * kopiuje do niego dane
 * @param[in] strL - String lewy
 * @param[in] srtR - String prawy
 * @return wskaźnik do nowego Stringa
 */
static const char *strCombine(const char *strL, const char *srtR) {
    int lghL = (int) strlen(strL);
    int lghR = (int) strlen(srtR);
    int newLgh = lghL + lghR + 1;
    char *newStr = malloc(sizeof(char) * newLgh);
    int i = 0;

    while (i < lghL) {
        newStr[i] = strL[i];
        i++;
    }
    i = 0;
    while (i < lghR) {
        newStr[i + lghL] = srtR[i];
        i++;
    }
    newStr[lghL + lghR] = '\0';

    return newStr;
}

struct PhoneNumbers const *phfwdGet(struct PhoneForward *pf, char const *num) {
    if (!isNum(num) || pf == NULL)
        return createEmptyPhoneNumbers();
    struct pair *moveTo = moveToNode(num, pf);
    struct PhoneForward *currDir = moveTo->pf;
    int index = moveTo->num;
    free(moveTo);

    while (currDir->directedTo == NULL && currDir->father != NULL) {
        currDir = currDir->father;
        index--;
    }
    if (index == 0)
        return createOnePhoneNumber(strCombine("", num));
    return createOnePhoneNumber(strCombine(currDir->directedTo, num + index));
}

char const *phnumGet(struct PhoneNumbers const *pnum, size_t idx) {
    if (pnum == NULL || idx >= (size_t) pnum->length)
        return NULL;
    return pnum->numbersArr[idx];
}

struct PhoneNumbers const *phfwdReverse(struct PhoneForward *pf, char const *num) {
    if (!isNum(num) || pf == NULL)
        return createEmptyPhoneNumbers();

    struct PhoneForward *currDir = pf;
    struct PhoneForward **currKids = currDir->children;
    int index = 0;

    SortTrie sortingTrie = createEmptySortingTrie();

    ///Przechodzi po nodach na które jest przekierowanie i wstawia elementy z list do drzewa
    while (num[index] != '\0' && currKids[num[index] - '0'] != NULL) {
        currDir = currKids[num[index] - '0'];
        currKids = currDir->children;
        index += 1;
        if (currDir->directedFrom != NULL) {
            listD currEl = currDir->directedFrom;
            while (currEl->next != currEl) {
                currEl = currEl->next;
                addStrings(sortingTrie, currEl->string, num + index);
            }
        }
    }

    addStrings(sortingTrie, "", num);
    int tmpArrLgh = sortingTrie->elements;
    char **tmpArr = createSortedArrFromTrie(sortingTrie);
    deleteSortingTrie(sortingTrie);

    return createMultiplePhoneNumber(tmpArr, tmpArrLgh);
}

/**
 * Zwraca tablicę przechowującą numery elementów występujących w napisie,
 * oraz ustawia elements na długość tej tablicy
 * @param set - dany napis
 * @param elements - wskaźnik na int (równy 0)
 * @return tablice róznych cyfr w napisie, lub NULL gdy elements = 0
 */
static size_t *getSetSize(char const *set, size_t *elements) {
    size_t idx = 0;
    bool hasEl[12];
    if (hasEl == NULL)
        return NULL;
    for (size_t i = 0; i < numberOfKids; i++)
        hasEl[i] = false;
    while (set[idx] != '\0') {
        if (set[idx] >= '0' && set[idx] <= ';' && !hasEl[set[idx] - '0']) {
            (*elements)++;
            hasEl[set[idx] - '0'] = true;
            if ((*elements) >= numberOfKids)
                break;
        }
        idx++;
    }
    if((*elements) == 0)
        return NULL;
    size_t *ret = malloc(sizeof(size_t) * (*elements));
    if (ret == NULL)
        return NULL;
    size_t idxRet = 0;
    for (size_t i = 0; i < numberOfKids; i++) {
        if (hasEl[i]) {
            ret[idxRet] = i;
            idxRet++;
        }
    }

    return ret;
}

/**
 * Funkcja wykonująca szybkie potęgowanie
 * @param depth - głębokość rekursji aktualna
 * @param lookForLgh - dlugość nietrywialnych numerów
 * @param base - podstawa potęgi
 * @return Otrzymana liczba po potęgowaniu
 */
static size_t raiseToPower(size_t depth, size_t lookForLgh, size_t base) {
    size_t inCalculation = 1;
    if(lookForLgh == 0 || lookForLgh < depth)
        return 0;
    size_t power = lookForLgh - depth;
    while (power > 0) {
        if (power % 2 == 1)
            inCalculation *= base;
        base *= base;
        power /= 2;
    }
    return inCalculation;
}

/**
 * Pomocnicz funkcja chodząca po drzewie i sumuje wszystkie nietrywialne numery
 * @param node - aktualny node
 * @param depth - głębokość rekursji
 * @param values - tablica z cyframi do wykoszystania
 * @param valLength - ilość cyfr do wykorzystania
 * @param lookForLgh - nietrywialne numery mają mieć tą długość
 * @param toAdd - zmienna do której dodjemy nietrywialne numery
 */
static void
auxTravel(struct PhoneForward *node, size_t depth, size_t *values, size_t valLength, size_t lookForLgh, size_t *toAdd) {
    if (node == NULL || depth > lookForLgh)
        return;
    if (node->directedFrom == NULL || isEmpty(node->directedFrom)) {
        for (size_t i = 0; i < valLength; i++)
            auxTravel(node->children[values[i]], depth + 1, values, valLength, lookForLgh, toAdd);
    } else {
        (*toAdd) += raiseToPower(depth, lookForLgh, valLength);
    }
}


size_t phfwdNonTrivialCount(struct PhoneForward *pf, char const *set, size_t len) {
    size_t elementsCount = 0;
    size_t *values = getSetSize(set, &elementsCount);

    if (values == NULL || pf == NULL || set == NULL)
        return 0;
    size_t ret = 0;
    auxTravel(pf, 0, values, elementsCount, len, &ret);
    free(values);

    return ret;
}