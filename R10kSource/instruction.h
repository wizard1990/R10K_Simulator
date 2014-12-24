//
//  instruction.h
//  R10K_Simulator
//
//  Created by Yan Zhang on 12/17/14.
//  Copyright (c) 2014 Yan Zhang. All rights reserved.
//

#ifndef __R10K_Simulator__instruction__
#define __R10K_Simulator__instruction__

#include "global.h"
#include <string>
#include "register_manager.h"
using namespace std;

class instruction {
public:
    InstructionType type;
    int rs;
    int rt;
    int rd;
    int index;
    bool is_ready;
    int trace_index;
    string address;
    
    // for float and integer
    instruction(InstructionType instr_type,
                int rsid,
                int rtid,
                int rdid,
                int active_index,
                int trace_instr_index,
                register_manager& manager):
    type(instr_type),
    rs(rsid),
    rt(rtid),
    rd(rdid),
    index(active_index),
    trace_index(trace_instr_index),
    is_ready(manager.get_valid_bit(rsid) && manager.get_valid_bit(rtid)) {};
    
    // for load & store & branch
    instruction(InstructionType instr_type,
                int rsid,
                int rtid,
                int active_index,
                int trace_instr_index,
                register_manager& manager):
    type(instr_type),
    rs(rsid),
    rt(rtid),
    index(active_index),
    trace_index(trace_instr_index),
    is_ready(manager.get_valid_bit(rsid)) {
        if (instr_type == StoreInstruction) {
            is_ready = is_ready && manager.get_valid_bit(rtid);
        }
    };
    
    instruction(): type(NoneInstruction), trace_index(-1) {};
};

#endif /* defined(__R10K_Simulator__instruction__) */
