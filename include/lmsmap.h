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
class LmsMap {
public:

    typedef typename K::size_type size_type;

    void insert(const K &key, const V &value);

    LmsIterator<K, V> begin() const {
        return LmsIterator<K, V>(this, 0);
    }

    LmsIterator<K, V> end() const {
        return LmsIterator<K, V>(this, keys.size());
    }

    size_type size() const {
        return keys.size();
    }

private:
    std::vector<K> keys;
    std::vector<V> values;
};

template<typename K, typename V>
class LmsIterator {
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
};

template<typename K, typename V>
void LmsMap<K, V>::insert(const K &key, const V &value) {
    auto key_location = std::lower_bound(keys.begin(), keys.end(), key);
    auto offset = key_location - keys.begin();
    auto value_location = values.begin() + offset;
    keys.insert(key_location, key);
    values.insert(value_location, value);
}

