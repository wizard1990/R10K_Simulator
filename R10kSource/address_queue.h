//
//  address_queue.h
//  R10K_Simulator
//
//  Created by Yan Zhang on 12/13/14.
//  Copyright (c) 2014 Yan Zhang. All rights reserved.
//

#ifndef __R10K_Simulator__address_queue__
#define __R10K_Simulator__address_queue__

#include "instruction.h"
#include "active_list_manager.h"

class address_queue {
public:
    vector<pair<instruction, AddressInstructionStatus> > mem_queue;
    int head;
    int count;
    int tail;
    
    address_queue(): mem_queue(vector<pair<instruction, AddressInstructionStatus> >(16, make_pair(instruction(), NewStatus))), head(0), tail(0), count(0) {};
    
    bool is_full();
    bool can_reg_resolve(int instr_index, register_manager& r_manager);
    bool can_mem_resolve(int instr_index, register_manager& r_manager);
    // new_instruction must be load/store instruction
    void insert_instruction(instruction new_instruction);
    // return the trace index. -1 for no action available
    int addr_cal(register_manager& r_manager);
    int mem_access(register_manager& r_manager, active_list_manager& a_manager);
    void commit();
    void flush(int active_index, register_manager& manager);
};

#endif /* defined(__R10K_Simulator__address_queue__) */
