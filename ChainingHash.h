/*
 *  Separate chaining hashtable
 */

#ifndef __CHAINING_HASH_H
#define __CHAINING_HASH_H

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

//
// Separate chaining based hash table - derived from Hash
//
template <typename K, typename V>
class ChainingHash : public Hash<K, V>
{
private:
    vector<list<pair<K, V>>> table;
    int currentSize = 0; // number of elements total in the hash
    int bucketCount;     // number of linked lists off the table "table size"

public:
    ChainingHash(int n = 11)
    {
        bucketCount = findNextPrime(n);
        table.resize(bucketCount);
    }

    ~ChainingHash()
    {
        this->clear();
    }

    int size()
    {
        return currentSize;
    }

    V operator[](const K &key)
    {
        typename list<std::pair<K, V>>::iterator itr;
        for (itr = table[key].begin(); itr != table[key].end(); itr++)
        {
            std::pair<K, V> keyValuePair = *itr;
            if (keyValuePair.first == key)
            {
                return keyValuePair.second;
            }
        }
        return -1;
    }

    bool insert(const std::pair<K, V> &pair)
    {
        list<std::pair<K, V>> &whichList = table[pair.first];
        // Duplicate Check
        if (find(whichList.begin(), whichList.end(), pair) != whichList.end())
            return false;

        whichList.push_back(pair);

        ++currentSize;

        // Rehash
        if (load_factor() >= 0.75f)
            rehash();

        return true;
    }

    void rehash()
    {
        vector<list<pair<K, V>>> oldLists = table;

        table.resize(findNextPrime(2 * table.size()));
        bucketCount = table.size();
        for (int j = 0; j < table.size(); j++)
            table[j].clear();

        // copy table over
        currentSize = 0;
        for (int i = 0; i < oldLists.size(); i++)
        {
            typename list<pair<K, V>>::iterator itr = oldLists[i].begin();
            while (itr != oldLists[i].end())
                insert(*itr++);
            // for (typenalist<std::pair<K, V>>::iterator itr = oldLists[i].begin(); itr != oldLists[i].end(); ++itr)
            //     insert(*itr);
        }
    }

    int hashFunction(int x)
    {
        return (x % bucketCount);
    }

    void erase(const K &key)
    {
        typename list<std::pair<K, V>>::iterator itr;
        for (itr = table[key].begin(); itr != table[key].end(); itr++)
        {
            std::pair<K, V> keyValuePair = *itr;
            if (keyValuePair.first == key)
            {
                keyValuePair.first = NULL;
            }
        }
    }

    void clear()
    {
        for (int i = 0; i < table.size(); i++)
        {
            table[i].clear();
        }
    }

    int bucket_count()
    {
        return bucketCount;
    }

    float load_factor()
    {
        return (float)currentSize / (float)bucketCount;
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

#endif //__CHAINING_HASH_H
