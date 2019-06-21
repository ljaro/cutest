#include "cachelookup.h"

CacheLookup &CacheLookup::instance()
{
    static CacheLookup lookup;
    return lookup;
}
