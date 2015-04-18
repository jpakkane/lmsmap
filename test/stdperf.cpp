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
#include<fstream>
#include<iostream>
#include<sys/time.h>

double timestamp() {
    struct timeval t;
    gettimeofday(&t, nullptr);
    return t.tv_sec + t.tv_usec/(1000000.0);
}


int main(int argc, char **argv) {
    std::map<std::string, int> m;
    int linenum = 0;
    const char *datafile;
    if(argc >= 1) {
        datafile = argv[1];
    } else {
        datafile = "/usr/share/dict/words";
    }

    std::ifstream ifile(datafile);
    std::string line;
    auto insertion_start = timestamp();
    while(!ifile.eof()) {
        std::getline(ifile, line);
        ++linenum;
        if(line.find('\'') != std::string::npos) {
            continue;
        }
        m[line] = linenum;
    }
    auto insertion_end = timestamp();
    auto insert_speed = m.size() / (insertion_end - insertion_start);
    std::cout << "Insertion speed: " << insert_speed << " elems/s\n";
    return 0;
}
