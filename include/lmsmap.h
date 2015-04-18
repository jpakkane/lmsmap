/*
 * lmsmap : Low memory string map
 * Copyright (c) 2015 Jussi Pakkanen
 *
 * Lmsmap is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 3 as published by the Free Software Foundation
 *
 * Lmsmap is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Libav; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#pragma once

#include<string>
#include<vector>
#include<algorithm>

template<typename K, typename V>
class LmsIterator;

template<typename K, typename V>
class LmsMap final {
public:

    typedef typename K::size_type size_type;

    LmsIterator<K, V> begin() const {
        return LmsIterator<K, V>(this, 0);
    }

    LmsIterator<K, V> end() const {
        return LmsIterator<K, V>(this, keys.size());
    }

    size_type size() const {
        return keys.size();
    }

    void insert(const K &key, const V &value);
    LmsIterator<K, V> find(const K &key) const;

    friend class LmsIterator<K, V>;

private:
    std::vector<K> keys;
    std::vector<V> values;
};

template<typename K, typename V>
class LmsIterator final {
private:
    typedef typename K::size_type index_type;
    const LmsMap<K, V> *map;
    index_type index = 0;

public:

    LmsIterator(const LmsMap<K, V> *m, index_type index) : map(m), index(index) {
    }

    LmsIterator(const LmsIterator &other) = default;
    LmsIterator(LmsIterator &&other) = default;
    LmsIterator& operator=(const LmsIterator &other) = default;

    ~LmsIterator() {}

    bool operator==(const LmsIterator &other) {
        return index == other.index;
    }

    bool operator!=(const LmsIterator &other) {
        return !(*this == other);
    }

    const K key() const {
        return map->keys[index];
    }

    const V value() const {
        return map->values[index];
    }
};

template<typename K, typename V>
void LmsMap<K, V>::insert(const K &key, const V &value) {
    auto key_location = std::lower_bound(keys.begin(), keys.end(), key);
    auto offset = key_location - keys.begin();
    auto value_location = values.begin() + offset;
    if(static_cast<size_type>(offset) < keys.size() && keys[offset] == key) {
        values[offset] = value;
    } else {
        keys.insert(key_location, key);
        values.insert(value_location, value);
    }
}

template<typename K, typename V>
LmsIterator<K, V> LmsMap<K, V>::find(const K &key) const {
    auto res = std::lower_bound(keys.begin(), keys.end(), key);
    if(res != keys.end() && *res == key) {
        return LmsIterator<K, V>(this, res - keys.begin());
    }
    return end();
}
