//
//  instruction_queue_manager.cpp
//  R10K_Simulator
//
//  Created by Yan Zhang on 12/13/14.
//  Copyright (c) 2014 Yan Zhang. All rights reserved.
//

#include "instruction_queue_manager.h"

bool instruction_queue_manager::can_insert_instruction_type(InstructionType type) {
    switch (type) {
        case BranchInstruction:
        case IntegerInstruction:
            return !integer_queue.is_full();
        case FloatAddInstruction:
        case FloatMulInstruction:
            return !float_queue.is_full();
        case LoadInstruction:
        case StoreInstruction:
            return !addr_queue.is_full();
        default:
            break;
    }
    return false;
}

void instruction_queue_manager::insert_instruction(instruction new_instruction) {
    switch (new_instruction.type) {
        case BranchInstruction:
        case IntegerInstruction:
            integer_queue.insert_instruction(new_instruction);
            break;
        case FloatAddInstruction:
        case FloatMulInstruction:
            float_queue.insert_instruction(new_instruction);
            break;
        case LoadInstruction:
        case StoreInstruction:
            addr_queue.insert_instruction(new_instruction);
            break;
        default:
            break;
    }
}

void instruction_queue_manager::flush(int active_index, register_manager& manager) {
    integer_queue.flush(active_index, manager);
    float_queue.flush(active_index, manager);
    addr_queue.flush(active_index, manager);
}