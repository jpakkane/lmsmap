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
#include<map>

namespace lms {
class LmsTester final {
public:
    void default_test() {
        LmsMap<std::string, int> m;
    }

    void insert_test() {
        LmsMap<std::string, int> m;
        const std::string key("foo");
        int value = 3;
        int value2 = 6;

        print(m);
        assert(m.size() == 0);
        assert(m.begin() == m.end());
        assert(!(m.begin() != m.end()));

        m.insert(key, value);
        print(m);

        assert(m.size() == 1);
        assert(!(m.begin() == m.end()));
        assert(m.begin() != m.end());

        assert(m.contains(key));
        assert(!m.contains("notthere"));

        auto yes = m.find(key);
        auto no = m.find("notthere");
        assert(yes != m.end());
        assert(no == m.end());

        std::string f = yes.key();
        std::cout << "Key: " << f << "\n";
        assert(yes.key() == key);
        assert(yes.value() == value);

        m.insert(key, value2);
        assert(m.size() == 1);
        assert(m.find(key).value() == value2);
    }

    void check_equal(std::map<std::string, int> &truth, LmsMap<std::string, int> &trial) {
        assert(truth.size() == trial.size());
        auto trial_iter = trial.begin();
        for(auto truth_iter = truth.begin(); truth_iter != truth.end(); ++truth_iter, ++trial_iter) {
            if (!(truth_iter->first == trial_iter.key())) {
                std::cout << "Key mismatch, truth " << truth_iter->first << " trial "
                        << trial_iter.key() << "\n";
                assert(0);
            }
            if(!(truth_iter->second == trial_iter.value())) {
                std::cout << "Value mismatch, truth " << truth_iter->second << " trial "
                        << trial_iter.value() << "\n";
                assert(0);
            }
        }
    }

    void multi_insert_test() {
        std::map<std::string, int> truth;
        LmsMap<std::string, int> trial;
        std::vector<std::string> data{{"bbb"}, {"aaa"}, {"ccc"},
        {"aouvawenJaauigasdnfkmj0n34nma0"}, {"abc"}};

        check_equal(truth, trial);
        int v=0;
        std::string::size_type totalsize = 0;
        for(const auto &i : data) {
            truth[i] = v;
            trial.insert(i, v);
            v++;
            totalsize += i.size();
            print(trial);
            check_equal(truth, trial);
            assert(totalsize == trial.keyblock_size());
        }

        truth["ccc"] = -1;
        trial.insert("ccc", -1);
        print(trial);
        check_equal(truth, trial);
    }

private:
    void print(LmsMap<std::string, int> &m) {
        std::cout << "Keys size: " << m.keys.size() << ".\n";
        std::cout << "Keyblock size: " << m.keyblock_size() << ".\n";
        std::cout << "Values size: " << m.values.size() << ".\n";
        std::cout << "Flat data size: " << m.keys.data.size() << ".\n";
        std::cout << "Flat offsets: ";
        for(const auto &i : m.keys.offsets) {
            std::cout << i << " ";
        }
        std::cout << "\n";
        std::cout << "Flat data: ";
        for(const auto &i : m.keys.data) {
            std::cout << i;
        }
        std::cout << "\n";
    }
};
}




int main(int argc, char **argv) {
#ifdef NDEBUG
    std::cout << "NDEBUG defined, tests won't work.\n";
    return 1;
#else
    lms::LmsTester t;
    std::cout << "Default test\n";
    t.default_test();
    std::cout << "\n\nInsert test\n";
    t.insert_test();
    std::cout << "\n\nMulti-insert test\n";
    t.multi_insert_test();
    return 0;
#endif
}
