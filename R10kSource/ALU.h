//
//  ALU.h
//  R10K_Simulator
//
//  Created by Yan Zhang on 12/13/14.
//  Copyright (c) 2014 Yan Zhang. All rights reserved.
//

#ifndef __R10K_Simulator__ALU__
#define __R10K_Simulator__ALU__

#include "arithmetic_queue.h"
#include "active_list_manager.h"
using namespace std;

class ALU {
public:
    vector<pair<int, int> > current_instruction;
    bool float_flag;
    ALU(bool is_float);
    
    // process the ALU, start a new instruction, pass -1 if no new instruction
    void process(int new_index, int dest_rid, arithmetic_queue& queue, active_list_manager& manager, register_manager& r_manager, vector<Trace>& traces, int cycle);
    void flush(int active_index, register_manager& manager);
};

#endif /* defined(__R10K_Simulator__ALU__) */