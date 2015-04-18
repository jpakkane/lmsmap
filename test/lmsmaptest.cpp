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

#include"lmsmap.h"
#include<cassert>
#include<iostream>

void default_test() {
    LmsMap<std::string, int> m;
}

void insert_test() {
    LmsMap<std::string, int> m;
    std::string key("foo");
    int value = 3;
    int value2 = 6;

    assert(m.size() == 0);
    assert(m.begin() == m.end());
    assert(!(m.begin() != m.end()));

    m.insert(key, value);

    assert(m.size() == 1);
    assert(!(m.begin() == m.end()));
    assert(m.begin() != m.end());

    auto yes = m.find(key);
    auto no = m.find("notthere");
    assert(yes != m.end());
    assert(no == m.end());

    assert(yes.key() == key);
    assert(yes.value() == value);

    m.insert(key, value2);
    assert(m.size() == 1);
    assert(m.find(key).value() == value2);

}

int main(int argc, char **argv) {
#ifdef NDEBUG
    std::cout << "NDEBUG defined, tests won't work.\n";
    return 1;
#else
    default_test();
    insert_test();
    return 0;
#endif
}
