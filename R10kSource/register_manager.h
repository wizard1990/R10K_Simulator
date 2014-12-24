//
//  register_manager.h
//  R10K_Simulator
//
//  Created by Yan Zhang on 12/13/14.
//  Copyright (c) 2014 Yan Zhang. All rights reserved.
//

#ifndef __R10K_Simulator__register_manager__
#define __R10K_Simulator__register_manager__

#include <vector>
using namespace std;

class register_manager {
public:
    vector<int> register_mapping;
    // true for free, false for used
    bool free_list[64];
    // true for ready, false for busy
    bool valid_bits[64];
    
    register_manager();
    
    int get_free_register();
    void add_free_register(int rid);
    
    void set_valid_bit(int rid);
    void reset_valid_bit(int rid);
    bool get_valid_bit(int rid);
    
    void set_register_mapping(int logical_rid, int physical_rid);
    int get_phy_rid(int logical_rld);
};

#endif /* defined(__R10K_Simulator__register_manager__) */