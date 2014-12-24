//
//  ALU.cpp
//  R10K_Simulator
//
//  Created by Yan Zhang on 12/13/14.
//  Copyright (c) 2014 Yan Zhang. All rights reserved.
//

#include "ALU.h"

ALU::ALU(bool is_float) {
    float_flag = is_float;
    if (is_float) {
        current_instruction = vector<pair<int, int> >(3, make_pair(-1, -1));
    }
    else {
        current_instruction = vector<pair<int, int> >(1, make_pair(-1, -1));
    }
}

void ALU::process(int new_index,
                  int dest_rid,
                  arithmetic_queue& queue,
                  active_list_manager& manager,
                  register_manager& r_manager,
                  vector<Trace>& traces,
                  int cycle) {
    if (!float_flag) {
        current_instruction[0] = make_pair(new_index, dest_rid);
        if(current_instruction[0].first != -1) {
            manager.set_donebit(current_instruction[0].first);
        }
        if (current_instruction[0].second != -1) {
            queue.update_operands(current_instruction[0].second, r_manager);
            r_manager.set_valid_bit(current_instruction[0].second);
        }
    } else {
        current_instruction[2] = current_instruction[1];
        current_instruction[1] = current_instruction[0];
        current_instruction[0] = make_pair(new_index, dest_rid);

        if (current_instruction[2].first != -1) {
            manager.set_donebit(current_instruction[2].first);
            traces[manager.active_list[current_instruction[2].first].instr_index].stage_cycles.push_back(cycle);
        }
        if (current_instruction[1].second != -1) {
            queue.update_operands(current_instruction[1].second, r_manager);
            r_manager.set_valid_bit(current_instruction[1].second);
        }
    }
}

void ALU::flush(int active_index, register_manager& manager) {
    for (int i = 0; i < current_instruction.size(); i++) {
        if (current_instruction[i].first == active_index) {
            manager.add_free_register(current_instruction[i].second);
            manager.reset_valid_bit(current_instruction[i].second);
            current_instruction[i] = make_pair(-1, -1);
        }
    }
}