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

void insert_test(std::map<std::string, int> &m, const char *ifilename) {
    std::ifstream ifile(ifilename);
    std::string line;
    int linenum=0;
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

}

void query_test(const std::map<std::string, int> &m, const char *ifilename) {
    std::ifstream ifile(ifilename);
    std::string line;
    auto query_start = timestamp();
    int found = 0;
    int notfound = 0;
    while(!ifile.eof()) {
        std::getline(ifile, line);
        if(m.find(line) != m.end()) {
            found++;
        } else {
            notfound++;
        }
    }
    auto query_end = timestamp();
    auto query_speed = (found + notfound) / (query_end - query_start);
    std::cout << "Query speed: " << query_speed << " q/s\n";
}

int main(int argc, char **argv) {
    std::map<std::string, int> m;
    const char *datafile;
    const char *queryfile;
    if(argc > 1) {
        datafile = argv[1];
    } else {
        datafile = "/usr/share/dict/words";
    }

    if(argc > 2) {
        queryfile = argv[2];
    } else {
        queryfile = datafile;
    }
    std::cout << "Insert test\n";
    insert_test(m, datafile);
    std::cout << "\n\nQuery test\n";
    query_test(m, queryfile);
    return 0;
}
