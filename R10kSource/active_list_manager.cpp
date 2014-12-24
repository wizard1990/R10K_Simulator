//
//  active_list_manager.cpp
//  R10K_Simulator
//
//  Created by Yan Zhang on 12/13/14.
//  Copyright (c) 2014 Yan Zhang. All rights reserved.
//

#include "active_list_manager.h"

static const int MAX_COMMIT_LIMIT = 4;

bool active_list_manager::is_full() {
    return count == 32;
}

vector<active_element> active_list_manager::commit() {
    vector<active_element> res;
    if (count == 0) {
        return res;
    }
    for (int i = head; i < head + min(count, MAX_COMMIT_LIMIT); ++i) {
        int index = i % (int)active_list.size();
        if (!active_list[index].done_bit || res.size() == MAX_COMMIT_LIMIT) {
            break;
        }
        res.push_back(active_list[index]);
    }
    head = (head + (int)res.size()) % active_list.size();
    count -= res.size();
    return res;
}

int active_list_manager::insert(active_element element) {
    count++;
    int res = tail;
    active_list[tail] = element;
    tail = (tail + 1) % active_list.size();
    return res;
}

void active_list_manager::issue(active_element ele) {
    active_list[tail] = ele;
    active_element res;
    if (count != 0) {
        int index = (tail - 1) < 0 ? ((tail - 1) + (int)active_list.size()) : (tail - 1);
        res = active_list[index];
    }
    count++;
    tail = (tail + 1) % active_list.size();
}

void active_list_manager::set_donebit(int index) {
    active_list[index].done_bit = true;
}


