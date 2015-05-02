#include "publicreadwritelock.h"

QReadWriteLock PublicReadWriteLock::sRWLock;

QReadWriteLock* PublicReadWriteLock::getLock()
{
    return &sRWLock;
}

PublicReadWriteLock::PublicReadWriteLock()
{

}

PublicReadWriteLock::~PublicReadWriteLock()
{

}

