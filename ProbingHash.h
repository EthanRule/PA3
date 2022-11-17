#ifndef __PROBING_HASH_H
#define __PROBING_HASH_H

// Standard library includes
#include <algorithm>
#include <iostream>
#include <vector>
#include <list>
#include <stdexcept>

// Custom project includes
#include "Hash.h"

// Namespaces to include
using namespace std;
using std::list;
using std::pair;
using std::vector;

using std::cout;
using std::endl;

// Can be used for tracking lazy deletion for each element in your table
enum EntryState
{
    EMPTY = 0,
    VALID = 1,
    DELETED = 2
};

template <typename K, typename V>
class ProbingHash : public Hash<K, V>
{ // derived from Hash
private:
    // Needs a table and a size.
    vector<pair<K, V>> table;
    int currentSize = 0; // number of elements total in the hash
    int bucketCount;     // total size of table vector
    enum EntryType
    {
        ACTIVE,
        EMPTY,
        DELETED
    };

public:
    ProbingHash(int n = 11)
    {

        table.resize(n);
    }
    ~ProbingHash()
    {
        this->clear();
    }

    int size()
    {
        return currentSize;
    }

    V operator[](const K &key)
    {
        return table[key].second;
    }

    int findPos(const K &key)
    {
        int offset = 1;
        int currentPos = key;

        while (table[currentPos].second != 0 && table[currentPos].second != key)
        {
            currentPos += offset;
            if (currentPos >= table.size())
                currentPos -= table.size();
        }
        return currentPos;
    }

    bool isActive(int currentPos) const { return table[currentPos].second > 0; }

    bool insert(const std::pair<K, V> &pair)
    {
        int currentPos = findPos(pair.first);
        if (isActive(currentPos))
            return false;

        table[currentPos] = pair;
        ++currentSize;

        // rehash
        if (load_factor() >= 0.75f)
            rehash();

        return true;
    }

    void rehash()
    {
        int tempSize = table.size();
        vector<pair<K, V>> oldLists = table;
        table.clear();
        table.resize(findNextPrime(2 * tempSize));

        // copy table over
        currentSize = 0;
        for (int i = 0; i < oldLists.size(); i++)
        {
            insert(oldLists[i]);
        }
    }
    bool contains(const K &key) const
    {
        return isActive(findPos(key));
    }

    void erase(const K &key)
    {
    }

    void clear()
    {
        table.clear();
    }

    int bucket_count()
    {
        return table.size();
    }

    float load_factor()
    {
        return (float)currentSize / (float)table.size();
    }

private:
    int findNextPrime(int n)
    {
        while (!isPrime(n))
        {
            n++;
        }
        return n;
    }

    int isPrime(int n)
    {
        for (int i = 2; i <= sqrt(n); i++)
        {
            if (n % i == 0)
            {
                return false;
            }
        }

        return true;
    }

    int hash(const K &key)
    {
        return 0;
    }
};

#endif //__PROBING_HASH_H
