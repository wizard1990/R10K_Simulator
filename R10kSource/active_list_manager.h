//
//  active_list_manager.h
//  R10K_Simulator
//
//  Created by Yan Zhang on 12/13/14.
//  Copyright (c) 2014 Yan Zhang. All rights reserved.
//

#ifndef __R10K_Simulator__active_list_manager__
#define __R10K_Simulator__active_list_manager__

#include <vector>
#include "global.h"

using namespace std;

class active_element {
public:
    InstructionType instrType;
    int dest;
    int old_dest;
    bool done_bit;
    int instr_index;
    
    active_element(InstructionType type, int dest_rid, int old_dest_rid, int index) :
    instrType(type), dest(dest_rid), old_dest(old_dest_rid), done_bit(false), instr_index(index) {};
    active_element(): instrType(NoneInstruction), instr_index(-1) {};
};

class active_list_manager {
public:
    int head;
    int tail;
    int count;
    vector<active_element> active_list;
    
    active_list_manager():
    head(0), tail(0), count(0), active_list(vector<active_element>(32, active_element())) {};
    bool is_full();
    int insert(active_element element);
    // return commited instructions
    vector<active_element> commit();
    // append decoded element
    void issue(active_element ele);
    void set_donebit(int index);
};

#endif /* defined(__R10K_Simulator__active_list_manager__) */