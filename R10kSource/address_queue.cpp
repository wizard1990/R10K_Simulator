//
//  address_queue.cpp
//  R10K_Simulator
//
//  Created by Yan Zhang on 12/13/14.
//  Copyright (c) 2014 Yan Zhang. All rights reserved.
//

#include "address_queue.h"

bool address_queue::is_full() {
    return count == mem_queue.size();
}

bool address_queue::can_reg_resolve(int instr_index, register_manager& r_manager) {
    if (mem_queue[instr_index].second != NewStatus || !r_manager.get_valid_bit(mem_queue[instr_index].first.rs)) {
        return false;
    }
    if (mem_queue[instr_index].first.type == StoreInstruction) {
        return r_manager.get_valid_bit(mem_queue[instr_index].first.rt);
    } else {
        return true;
    }
}

bool address_queue::can_mem_resolve(int instr_index, register_manager& r_manager) {
    if (mem_queue[instr_index].second != AddressCalculatedStatus) {
        return false;
    }
    if (mem_queue[instr_index].first.type == StoreInstruction) {
        for (int i = head; i < head + count; i++) {
            int cur_index = i % mem_queue.size();
            if (cur_index == instr_index) {
                break;
            }
            if (mem_queue[cur_index].second != MemoryAccessedStatus) {
                return false;
            }
        }
    } else {
        for (int i = head; i < head + count; i++) {
            int cur_index = i % mem_queue.size();
            if (cur_index == instr_index) {
                break;
            }
            if (mem_queue[cur_index].first.type == StoreInstruction
                && mem_queue[cur_index].first.address == mem_queue[instr_index].first.address) {
                return false;
            }
        }
    }
    return true;
}

void address_queue::insert_instruction(instruction new_instruction) {
    count++;
    mem_queue[tail].first = new_instruction;
    mem_queue[tail].second = NewStatus;
    tail = (tail + 1) % mem_queue.size();
}

int address_queue::addr_cal(register_manager &r_manager) {
    for (int i = head; i < head + count; i++) {
        int cur_index = i % mem_queue.size();
        if (can_reg_resolve(cur_index, r_manager)) {
            mem_queue[cur_index].second = AddressCalculatedStatus;
            return mem_queue[cur_index].first.trace_index;
        }
    }
    return -1;
}

int address_queue::mem_access(register_manager& r_manager, active_list_manager& a_manager) {
    for (int i = head; i < head + count; i++) {
        int cur_index = i % mem_queue.size();
        if (can_mem_resolve(cur_index, r_manager)) {
            mem_queue[cur_index].second = MemoryAccessedStatus;
            if (mem_queue[cur_index].first.type == LoadInstruction) {
                int rid = mem_queue[cur_index].first.rt;
                r_manager.set_valid_bit(rid);
            }
            int active_index = mem_queue[cur_index].first.index;
            a_manager.set_donebit(active_index);
            return mem_queue[cur_index].first.trace_index;
        }
    }
    return -1;
}

void address_queue::commit() {
    if (mem_queue[head].second == MemoryAccessedStatus) {
        head = (head + 1) % mem_queue.size();
        count--;
    }
}

void address_queue::flush(int active_index, register_manager& manager) {
    int index = tail - 1;
    if (index < 0) {
        index += mem_queue.size();
    }
    if (mem_queue[index].first.index == active_index) {
        if (mem_queue[index].first.type == LoadInstruction) {
            manager.add_free_register(mem_queue[index].first.rt);
            manager.reset_valid_bit(mem_queue[index].first.rt);
        }
        tail--;
        count--;
    }
}