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

namespace lms {

template<typename K>
class FlatHolder;

template<typename K>
class FlatProxy final {
private:
    typedef typename FlatHolder<K>::value_type value_type;
    typedef typename FlatHolder<K>::offset_type offset_type;

    const FlatHolder<K> *holder;
    value_type start;
    value_type end;


public:
    FlatProxy(const FlatHolder<K> *holder, offset_type start, offset_type end) :
    holder(holder), start(start), end(end) {
    }

    offset_type size() const {
        return end - start;
    }

    bool operator==(const K &other) const {
        return std::equal(holder->data.begin()+start, holder->data.begin()+end,
                other.begin());
    }

    bool operator!=(const K &other) const {
        return !(*this == other);
    }

    bool operator<(const K &other) const {
        return std::lexicographical_compare(holder->data.begin()+start,
                holder->data.begin() + end, other.begin(), other.end());
    }

};

template<typename Container, typename K>
class FlatIterator final {
public:
    typedef typename Container::value_type value_type;
    typedef typename Container::offset_type offset_type;

    typedef std::random_access_iterator_tag iterator_category;
    typedef typename Container::difference_type difference_type;
    typedef value_type* pointer;
    typedef value_type& reference;

private:
    Container *fh;
    offset_type index;

public:
    FlatIterator(Container *fh, offset_type index) : fh(fh), index(index) {
    }

    FlatIterator(const FlatIterator<Container, K> &other) = default;
    ~FlatIterator() = default;

    bool operator==(const FlatIterator<Container, K> &other) const {
        return index == other.index;
    }

    bool operator!=(const FlatIterator<Container, K> &other) const {
        return !(*this == other);
    }

    offset_type operator-(const FlatIterator<Container, K> &other) const {
        return index - other.index;
    }

    FlatProxy<K> operator*() const {
        return FlatProxy<K>(fh, fh->offsets[index], fh->end_of(index));
    }

    FlatIterator<Container, K>& operator++() {
        index++;
        return *this;
    }

    FlatIterator<Container, K> operator++(int) {
        FlatIterator old(*this);
        index++;
        return old;
    }

    FlatIterator<Container, K> operator+=(const difference_type increment) {
        index += increment;
        return *this;
    }

    bool operator<(const FlatIterator<Container, K> &other) const {
        return index < other.index;
    }

    bool operator<(const K &value) const {
        return **this < value;
    }

    bool operator==(const K &value) const {
        return **this == value;
    }
};

template<typename K>
class FlatHolder final {
public:
    typedef typename K::value_type value_type;
    typedef typename std::vector<K>::size_type offset_type;
    typedef typename std::vector<K>::difference_type difference_type;

private:
    std::vector<value_type> data;
    std::vector<offset_type> offsets;

    friend class FlatIterator<FlatHolder<K>, K>;
    friend class FlatIterator<const FlatHolder<K>, K>;
    friend class FlatProxy<K>;
    friend class LmsTester;

public:

    FlatHolder() {}
    ~FlatHolder() {}

    const std::vector<value_type>& get_data() const { return data; }

    FlatIterator<FlatHolder, K> begin(){
        return FlatIterator<FlatHolder, K>(this, 0);
    }
    FlatIterator<FlatHolder, K> end() {
        return FlatIterator<FlatHolder, K>(this, offsets.size());
    }

    FlatIterator<const FlatHolder, K> cbegin() const {
        return FlatIterator<const FlatHolder, K>(this, 0);
    }

    FlatIterator<const FlatHolder, K> cend() const {
        return FlatIterator<const FlatHolder, K>(this, offsets.size());
    }

    void insert(const FlatIterator<FlatHolder, K> &it, const K &value) {
        auto increase = value.size();
        auto offset_insertion_point = it - begin();
        auto data_insertion_point = offset_insertion_point < offsets.size() ?
                offsets[offset_insertion_point] : data.size();

        data.insert(data.begin() + data_insertion_point, value.begin(), value.end());
        bool need_adjustment = offset_insertion_point < offsets.size();
        if(offset_insertion_point < offsets.size()) {
            auto oldoffset = offsets[offset_insertion_point];
            offsets.insert(offsets.begin()+offset_insertion_point, oldoffset);

            for(offset_type i=offset_insertion_point+1; i<offsets.size(); i++) {
                offsets[i] += increase;
            }
        } else {
            offsets.push_back(data_insertion_point);
        }
    }

    offset_type size() const { return offsets.size(); }

    offset_type end_of(const offset_type &off) const {
        return off < offsets.size()-1 ? offsets[off+1] : data.size();
    }

    K operator[](const offset_type &off) const {
        auto start = offsets[off];
        auto end = end_of(off);
        return K(&data[start], end-start);
    }
};


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

    size_type keyblock_size() const {
        return keys.get_data().size();
    }
    void insert(const K &key, const V &value);
    LmsIterator<K, V> find(const K &key) const;
    bool contains(const K &key) const {
        return find(key) != end();
    }

    friend class LmsIterator<K, V>;
    friend class LmsTester;

private:
    FlatHolder<K> keys;
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

    bool operator==(const LmsIterator<K, V> &other) {
        return index == other.index;
    }

    bool operator!=(const LmsIterator<K, V> &other) {
        return !(*this == other);
    }

    const K key() const {
        return map->keys[index];
    }

    const V value() const {
        return map->values[index];
    }

    LmsIterator<K, V> operator++() {
        index++;
        return *this;
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
    auto res = std::lower_bound(keys.cbegin(), keys.cend(), key);
    if(res != keys.cend() && *res == key) {
        return LmsIterator<K, V>(this, res - keys.cbegin());
    }
    return end();
}

}
