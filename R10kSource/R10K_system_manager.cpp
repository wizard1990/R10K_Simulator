//
//  R10K_system_manager.cpp
//  R10K_Simulator
//
//  Created by Yan Zhang on 12/13/14.
//  Copyright (c) 2014 Yan Zhang. All rights reserved.
//

#include "R10K_system_manager.h"

bool R10K_system_manager::process(Trace *t, int trace_index, vector<Trace>& traces) {
    bool stall_flag = true;
    vector<active_element> commited = active_manager.commit();
    
    // instruction queue process
    instruction alu2_instr = queue_manager.integer_queue.ready_for_ALU2(reg_manager);
    instruction alu1_instr = queue_manager.integer_queue.ready_for_ALU1(reg_manager);
    instruction fpadd_instr = queue_manager.float_queue.ready_for_FP(FloatAddInstruction, reg_manager);
    instruction fpmul_instr = queue_manager.float_queue.ready_for_FP(FloatMulInstruction, reg_manager);
    
    // memory units process
    int mem_trace_index = queue_manager.addr_queue.mem_access(reg_manager, active_manager);
    if (mem_trace_index > -1) {
        traces[mem_trace_index].stage_cycles.push_back(cycle);
    }
    int addr_trace_index = queue_manager.addr_queue.addr_cal(reg_manager);
    if (addr_trace_index > -1) {
        traces[addr_trace_index].stage_cycles.push_back(cycle);
    }

    // push the result into ALU
    if (alu1_instr.type == NoneInstruction) {
        alu_manager.integer_ALU1.process(-1, -1, queue_manager.integer_queue, active_manager, reg_manager, traces, cycle);
    } else {
        if (alu1_instr.type == BranchInstruction) {
            alu1_instr.rd = -1;
        }
        alu_manager.integer_ALU1.process(alu1_instr.index, alu1_instr.rd, queue_manager.integer_queue, active_manager, reg_manager, traces, cycle);
        traces[alu1_instr.trace_index].stage_cycles.push_back(cycle);
    }
    if (alu2_instr.type == NoneInstruction) {
        alu_manager.integer_ALU2.process(-1, -1, queue_manager.integer_queue, active_manager, reg_manager, traces, cycle);
    } else {
        alu_manager.integer_ALU2.process(alu2_instr.index, alu2_instr.rd, queue_manager.integer_queue, active_manager, reg_manager, traces, cycle);
        traces[alu2_instr.trace_index].stage_cycles.push_back(cycle);
    }
    if (fpadd_instr.type == NoneInstruction) {
        alu_manager.fpadd_ALU.process(-1, -1, queue_manager.float_queue, active_manager, reg_manager, traces, cycle);
    } else {
        alu_manager.fpadd_ALU.process(fpadd_instr.index, fpadd_instr.rd, queue_manager.float_queue, active_manager, reg_manager, traces, cycle);
        traces[fpadd_instr.trace_index].stage_cycles.push_back(cycle);
    }
    if (fpmul_instr.type == NoneInstruction) {
        alu_manager.fpmul_ALU.process(-1, -1, queue_manager.float_queue, active_manager, reg_manager, traces, cycle);
    } else {
        alu_manager.fpmul_ALU.process(fpmul_instr.index, fpmul_instr.rd, queue_manager.float_queue, active_manager, reg_manager, traces, cycle);
        traces[fpmul_instr.trace_index].stage_cycles.push_back(cycle);
    }
    
    // insert instruction into active list and instruction queues
    if (t && !active_manager.is_full() && queue_manager.can_insert_instruction_type(t->_op)) {
        stall_flag = false;
        t->stage_cycles.push_back(cycle);
        t->status = TraceExecuted;
        int new_reg = -1;
        int old_reg = -1;

        if (t->_op != StoreInstruction && t->_op != BranchInstruction) {
            if (t->_rd == 0 || (t->_op == LoadInstruction && t->_rt == 0)) {
                new_reg = 0;
                old_reg = 0;
            } else {
                if (t->_op == LoadInstruction) {
                    new_reg = reg_manager.get_free_register();
                    old_reg = reg_manager.get_phy_rid(t->_rt);
                } else {
                    new_reg = reg_manager.get_free_register();
                    old_reg = reg_manager.get_phy_rid(t->_rd);
                }
            }
        }
        
        active_element new_elem(t->_op, t->_rd, old_reg, trace_index);
        if (t->_op == LoadInstruction) {
            new_elem.dest = t->_rt;
        }
        
        int active_index = active_manager.insert(new_elem);
        instruction new_instr;
        switch (t->_op) {
            case FloatAddInstruction:
            case FloatMulInstruction:
            case IntegerInstruction:
                new_instr = instruction(t->_op, reg_manager.get_phy_rid(t->_rs), reg_manager.get_phy_rid(t->_rt), new_reg, active_index, trace_index, reg_manager);
                queue_manager.insert_instruction(new_instr);
                reg_manager.set_register_mapping(t->_rd, new_reg);
                break;
            case BranchInstruction:
                new_instr = instruction(t->_op, reg_manager.get_phy_rid(t->_rs), reg_manager.get_phy_rid(t->_rt), active_index, trace_index, reg_manager);
                branch_stack.push(make_pair(active_index, reg_manager.register_mapping));
                queue_manager.insert_instruction(new_instr);
                break;
            case LoadInstruction:
            case StoreInstruction:
                new_instr = instruction(t->_op, reg_manager.get_phy_rid(t->_rs), new_reg, active_index, trace_index, reg_manager);
                queue_manager.insert_instruction(new_instr);
                if (t->_op == LoadInstruction) {
                    reg_manager.set_register_mapping(t->_rt, new_reg);
                }
                break;
            default:
                break;
        }
    }
    
    for (int i = 0; i < commited.size(); i++) {
        if (commited[i].old_dest != 0) {
            reg_manager.reset_valid_bit(commited[i].old_dest);
            reg_manager.add_free_register(commited[i].old_dest);
        }
        
        if (commited[i].instrType == LoadInstruction
            || commited[i].instrType == StoreInstruction) {
            queue_manager.addr_queue.commit();
        }
        
        traces[commited[i].instr_index].status = TraceCommitted;
        traces[commited[i].instr_index].stage_cycles.push_back(cycle);
    }

    
    if (!t && !active_manager.is_full()) {
        stall_flag = false;
    }
    return stall_flag;
}

void R10K_system_manager::flush(int active_index, vector<Trace>& traces) {
    while (branch_stack.top().first != active_index) {
        branch_stack.pop();
    }
    int branch_trace_index = active_manager.active_list[active_index].instr_index;
    reg_manager.register_mapping = branch_stack.top().second;
    aborted_traces.push_back(vector<Trace>());
    
    for (int i =(int)traces.size() - 1; i > branch_trace_index; i--) {
        if (traces[i].status == TraceFetched || traces[i].status == TraceDecoded) {
            traces[i].status = TraceNew;
            traces[i].stage_cycles.clear();
            aborted_traces.back().push_back(traces[i]);
        }
    }
    
    while (active_manager.tail != active_index) {
        active_manager.tail = active_manager.tail - 1;
        if (active_manager.tail < 0) {
            active_manager.tail += active_manager.active_list.size();
        }
        if (active_manager.tail == active_index) {
            active_manager.tail = (active_manager.tail + 1) % active_manager.active_list.size();
            break;
        }
        traces[active_manager.active_list[active_manager.tail].instr_index].status = TraceNew;
        traces[active_manager.active_list[active_manager.tail].instr_index].stage_cycles.clear();
        active_manager.count--;
        aborted_traces.back().push_back(traces[active_manager.active_list[active_manager.tail].instr_index]);
        queue_manager.flush(active_manager.tail, reg_manager);
        alu_manager.flush(active_manager.tail, reg_manager);
    }
}