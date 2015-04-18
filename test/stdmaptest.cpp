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

#include<map>
#include<string>
#include<iostream>

int main(int argc, char **argv) {
    std::map<std::string, int> x;
    x["foo"] = 3;
    auto res = x.find("foo");
    if(res == x.end()) {
        std::cout << "Lookup failed.\n";
        return 1;
    }
    if(res->second != 3) {
        std::cout << "Lookup wrong, got " << res->second << " expected " << 3 << ".\n";
        return 2;
    }
    return 0;
}
