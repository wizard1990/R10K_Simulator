//
//  arithmetic_queue.cpp
//  R10K_Simulator
//
//  Created by Yan Zhang on 12/13/14.
//  Copyright (c) 2014 Yan Zhang. All rights reserved.
//

#include "arithmetic_queue.h"

bool arithmetic_queue::is_full() {
    if (branch_queue.size() + arith_queue.size() == 16) {
        return true;
    }
    return false;
}

void arithmetic_queue::insert_instruction(instruction new_instruction) {
    if (new_instruction.type == BranchInstruction) {
        branch_queue.push_back(new_instruction);
    } else {
        arith_queue.push_back(new_instruction);
    }
}

instruction arithmetic_queue::ready_for_ALU1(register_manager &manager) {
    instruction res;
    for (int i = 0; i < branch_queue.size(); i++) {
        if (branch_queue[i].is_ready ||
            (manager.get_valid_bit(branch_queue[i].rs) && manager.get_valid_bit(branch_queue[i].rt))) {
            branch_queue[i].is_ready = true;
            res = branch_queue[i];
            branch_queue.erase(branch_queue.begin() + i);
            break;
        }
    }
    if (res.type == NoneInstruction) {
        for (int i = 0; i < arith_queue.size(); i++) {
            if (arith_queue[i].is_ready ||
                (manager.get_valid_bit(arith_queue[i].rs) && manager.get_valid_bit(arith_queue[i].rt))) {
                arith_queue[i].is_ready = true;
                res = arith_queue[i];
                arith_queue.erase(arith_queue.begin() + i);
                break;
            }
        }
    }
    return res;
}

instruction arithmetic_queue::ready_for_ALU2(register_manager &manager) {
    instruction res;
    for (int i = 0; i < arith_queue.size(); i++) {
        if (arith_queue[i].is_ready ||
            (manager.get_valid_bit(arith_queue[i].rs) && manager.get_valid_bit(arith_queue[i].rt))) {
            arith_queue[i].is_ready = true;
            res = arith_queue[i];
            arith_queue.erase(arith_queue.begin() + i);
            break;
        }
    }
    return res;
}

instruction arithmetic_queue::ready_for_FP(InstructionType type, register_manager &manager) {
    instruction res;
    for (int i = 0; i < arith_queue.size(); i++) {
        if ((arith_queue[i].type == type && arith_queue[i].is_ready) ||
            (manager.get_valid_bit(arith_queue[i].rs) && manager.get_valid_bit(arith_queue[i].rt))) {
            arith_queue[i].is_ready = true;
            res = arith_queue[i];
            arith_queue.erase(arith_queue.begin() + i);
            break;
        }
    }
    return res;
}

void arithmetic_queue::update_operands(int rid, register_manager& manager) {
    for (int i = 0; i < arith_queue.size(); i++) {
        if (!arith_queue[i].is_ready) {
            if (rid == arith_queue[i].rs) {
                if (arith_queue[i].rt == rid || manager.get_valid_bit(arith_queue[i].rt)) {
                    arith_queue[i].is_ready = true;
                }
            }
            else if (rid == arith_queue[i].rt) {
                if (arith_queue[i].rs == rid || manager.get_valid_bit(arith_queue[i].rs)) {
                    arith_queue[i].is_ready = true;
                }
            }
        }
    }
    for (int i = 0; i < branch_queue.size(); i++) {
        if (!branch_queue[i].is_ready) {
            if (rid == branch_queue[i].rs) {
                if (manager.get_valid_bit(branch_queue[i].rt)) {
                    branch_queue[i].is_ready = true;
                }
            }
            else if (rid == branch_queue[i].rt) {
                if (manager.get_valid_bit(branch_queue[i].rs)) {
                    branch_queue[i].is_ready = true;
                }
            }
        }
    }
}

void arithmetic_queue::flush(int active_index, register_manager& manager) {
    for (int i = 0; i < branch_queue.size(); i++) {
        if (branch_queue[i].index == active_index) {
            branch_queue.erase(branch_queue.begin() + i);
            return;
        }
    }
    for (int i = 0; i < arith_queue.size(); i++) {
        if (arith_queue[i].index == active_index) {
            manager.add_free_register(arith_queue[i].rd);
            manager.reset_valid_bit(arith_queue[i].rd);
            arith_queue.erase(arith_queue.begin() + i);
            return;
        }
    }
}