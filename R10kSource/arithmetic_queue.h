//
//  arithmetic_queue.h
//  R10K_Simulator
//
//  Created by Yan Zhang on 12/13/14.
//  Copyright (c) 2014 Yan Zhang. All rights reserved.
//

#ifndef __R10K_Simulator__arithmetic_queue__
#define __R10K_Simulator__arithmetic_queue__

#include "global.h"
#include "instruction.h"

class arithmetic_queue {
public:
    vector<instruction> branch_queue;
    vector<instruction> arith_queue;
    
    bool is_full();
    // new_instruction must be branch/integer instruction (for integer queue)
    // or float instruction (for float queue)
    void insert_instruction(instruction new_instruction);
    instruction ready_for_ALU1(register_manager &manager);
    instruction ready_for_ALU2(register_manager &manager);
    instruction ready_for_FP(InstructionType type, register_manager& manager);
    void update_operands(int rid, register_manager& manager);
    void flush(int active_index, register_manager& manager);
};

#endif /* defined(__R10K_Simulator__arithmetic_queue__) */
