/** @file
* Implementacja klasy implementującej bazę baz przekierowań
*/

#include "directoryBase.h"

/**
 * Trzyma bazy przekierowań
 */
static DirectoryBase dirBase;

void createDirectoryBase() {
    dirBase = (DirectoryBase) malloc(sizeof(struct dirBase));

    dirBase->father = dirBase;
    dirBase->brother = dirBase;
    dirBase->son = dirBase;
    dirBase->directory = NULL;
    dirBase->nodeVal = '!';
}

static DirectoryBase createDirBElment(char val) {
    DirectoryBase newBase = (DirectoryBase) malloc(sizeof(struct dirBase));

    if (newBase == NULL)
        return NULL;

    newBase->father = NULL;
    newBase->brother = newBase;
    newBase->son = newBase;
    newBase->directory = NULL;
    newBase->nodeVal = val;

    return newBase;
}

/**
 * Łączy przekierowania dwóch nodów tach że father staje się ojcem a son synem
 * @param father - stanie się ojcem
 * @param son - stanie się synem
 */
static void joinFatherSon(DirectoryBase father, DirectoryBase son) {
    father->son = son;
    son->father = father;
}

/**
 * Usuwa nody bez znaczenia do góry
 * @param base - node od którego zaczynamy usuwanie
 */
static void deleteWhileUseless(DirectoryBase base) {
    if (base == NULL)
        return;
    DirectoryBase tmp = base;
    while (base != NULL) {
        tmp = base;
        base = base->father;
        free(tmp);
    }
}

/**
 * Rekurencyjna funkcja pomocnicza do usuwania bazy przekierowań
 * @param base - node bazy przekierowań
 */
static void delDirBaseAux(DirectoryBase base) {
    if (base->son != base)
        delDirBaseAux(base->son);
    if (base->brother != base)
        delDirBaseAux(base->brother);
    if (base->directory != NULL)
        phfwdDelete(base->directory);
    free(base);
}


void delDirBase() {
    if(dirBase != NULL){
        delDirBaseAux(dirBase);
    }
}

/**
 * Usuwa noda bazy przekierowań
 * @param base - wskaźnik na node bazy przekierowań
 */
static void delDirBaseNode(DirectoryBase base) {
    if (dirBase != base && base != NULL) {
        if (base->directory != NULL)
            phfwdDelete(base->directory);
        free(base);
    }
}

/**
 * Funkcja która przechodzi do szukanej pozycji w bazie baz
 * @param str - String reprezentujący szukaną nazwę bazy przekierowań
 * @param idxPtr - wskaźnik na pozycję w str
 * @param lgh - długość str
 * @param finished - ustwaia true jak udało się dojść do liścia drzewa
 * @return szukany node w bazie bazie przekierowań
 */
static DirectoryBase goToPosition(const char *str, int *idxPtr, int lgh, bool *finished) {
    DirectoryBase curr = dirBase->son;
    DirectoryBase last = dirBase;

    if (curr == last)
        *finished = true;

    while ((*idxPtr) < lgh && last != curr) {
        do {
            if (curr->nodeVal == str[*idxPtr]) {
                ++*idxPtr;
                if ((*idxPtr) == lgh)
                    return curr;
                if (curr->son == curr)
                    *finished = true;   ///ADD BELOW
                last = curr;
                curr = curr->son;
                break;
            } else {
                last = curr;
                curr = curr->brother;
            }
        } while (last != curr);
    }

    return curr;
}

/**
 * Pomocnicza fukcja do rekurencyjnego usuwania niepotrzebnych nodów w bazie baz
 * Usuwa aż node będzie mieć syna lub jakąś bazę przekierowań
 * @param curr - node od którego należy zacząć usuwanie
 */
static void deleteDirectoryAux(DirectoryBase curr) {
    if (curr->son != curr)
        return;
    if(curr->father == curr)
        return;
    if(curr->directory != NULL)
        return;

    DirectoryBase father = curr->father;
    DirectoryBase brother = curr->brother;
    brother->father = father;

    if (curr->father->son == curr) {
        if (brother == curr)
            father->son = father;
        else
            father->son = brother;
    } else {
        if (brother == curr)
            father->brother = father;
        else
            father->brother = brother;
    }

    delDirBaseNode(curr);
    deleteDirectoryAux(father);
}

bool deleteDirectory(char *name, struct PhoneForward *dir, bool *isCurr) {
    int idx = 0;
    int lgh = (int) strlen(name);
    bool posFinished = false;
    DirectoryBase curr = goToPosition(name, &idx, lgh, &posFinished);

    if (idx != lgh)
        return false;
    if (curr->directory == NULL)
        return false;
    if(dir == curr->directory)
        *isCurr = true;
    if (curr->directory != NULL){
        phfwdDelete(curr->directory);
        curr->directory = NULL;
    }
    deleteDirectoryAux(curr);

    return true;
}

struct PhoneForward *addDirectory(char *name) {
    int idx = 0;
    int lgh = (int) strlen(name);
    bool posFinished = false;
    DirectoryBase curr = goToPosition(name, &idx, lgh, &posFinished);

    if (idx == lgh && curr->directory != NULL)
        return curr->directory;
    if (idx == lgh) {
        struct PhoneForward *newDirectory = phfwdNew();
        if (newDirectory == NULL)
            return NULL;
        curr->directory = newDirectory;
        return curr->directory;
    }

    bool first = true;
    DirectoryBase firstNew = NULL;
    DirectoryBase currNew = NULL;
    while (idx < lgh) {
        DirectoryBase new = createDirBElment(name[idx]);
        idx++;
        if (new == NULL)
            break;

        if (first) {
            firstNew = new;
            firstNew->father = firstNew;
            currNew = new;
            first = false;
        } else {
            joinFatherSon(currNew, new);
            currNew = new;
        }
    }
    struct PhoneForward *newDirectory = phfwdNew();
    if (idx < lgh || newDirectory == NULL) {
        deleteWhileUseless(currNew);
        phfwdDelete(newDirectory);
        return NULL;
    }

    currNew->directory = newDirectory;
    if (posFinished)
        curr->son = firstNew;
    else
        curr->brother = firstNew;
    firstNew->father = curr;

    return newDirectory;
}