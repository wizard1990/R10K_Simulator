//
//  instruction_queue_manager.h
//  R10K_Simulator
//
//  Created by Yan Zhang on 12/13/14.
//  Copyright (c) 2014 Yan Zhang. All rights reserved.
//

#ifndef __R10K_Simulator__instruction_queue_manager__
#define __R10K_Simulator__instruction_queue_manager__

#include "arithmetic_queue.h"
#include "address_queue.h"

class instruction_queue_manager {
public:
    arithmetic_queue integer_queue;
    arithmetic_queue float_queue;
    address_queue addr_queue;
    
    bool can_insert_instruction_type(InstructionType type);
    void insert_instruction(instruction new_instruction);
    void flush(int active_index, register_manager& manager);
};

#endif /* defined(__R10K_Simulator__instruction_queue_manager__) */