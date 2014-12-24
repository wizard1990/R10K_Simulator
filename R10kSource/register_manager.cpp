//
//  register_manager.cpp
//  R10K_Simulator
//
//  Created by Yan Zhang on 12/13/14.
//  Copyright (c) 2014 Yan Zhang. All rights reserved.
//

#include "register_manager.h"

register_manager::register_manager(): register_mapping(vector<int>(64, 0)) {
    for (int i = 0; i < 32; i++) {
        register_mapping[i] = i;
        valid_bits[i] = true;
        free_list[i] = false;
    }
    for (int i = 32; i < 64; i++) {
        valid_bits[i] = false;
        free_list[i] = true;
    }
}

int register_manager::get_free_register() {
    for (int i = 0; i < 64; i++) {
        if (free_list[i] == true) {
            free_list[i] = false;
            return i;
        }
    }
    return -1;
}

void register_manager::add_free_register(int rid) {
    free_list[rid] = true;
}

void register_manager::set_valid_bit(int rid) {
    valid_bits[rid] = true;
}

void register_manager::reset_valid_bit(int rid) {
    valid_bits[rid] = false;
}

bool register_manager::get_valid_bit(int rid) {
    return valid_bits[rid];
}

void register_manager::set_register_mapping(int logical_rid, int physical_rid) {
    if (logical_rid == 0) {
        return;
    }
    register_mapping[logical_rid] = physical_rid;
}

int register_manager::get_phy_rid(int logical_rid) {
    return register_mapping[logical_rid];
}