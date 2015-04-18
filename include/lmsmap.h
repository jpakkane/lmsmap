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
class LmsMap {
public:

    void insert(const K &key, const V &value);

private:
    std::vector<K> keys;
    std::vector<V> values;
};

template<typename K, typename V>
void LmsMap<K, V>::insert(const K &key, const V &value) {
    auto key_location = std::lower_bound(keys.begin(), keys.end(), key);
    auto offset = key_location - keys.begin();
    auto value_location = values.begin() + offset;
    keys.insert(key_location, key);
    values.insert(value_location, value);
}

