#ifndef PUBLICREADWRITELOCK_H
#define PUBLICREADWRITELOCK_H

#include <QReadWriteLock>

class PublicReadWriteLock
{
public:
    static QReadWriteLock* getLock();

    PublicReadWriteLock();
    ~PublicReadWriteLock();

private: // data
    static QReadWriteLock sRWLock;
};

#endif // PUBLICREADWRITELOCK_H
