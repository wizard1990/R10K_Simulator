//
//  R10K_system_manager.h
//  R10K_Simulator
//
//  Created by Yan Zhang on 12/13/14.
//  Copyright (c) 2014 Yan Zhang. All rights reserved.
//

#ifndef __R10K_Simulator__R10K_system_manager__
#define __R10K_Simulator__R10K_system_manager__

#include "active_list_manager.h"
#include "instruction_queue_manager.h"
#include "ALU_manager.h"
#include "trace_reader.h"
#include <stack>
#include <map>

class R10K_system_manager {
public:
    active_list_manager active_manager;
    instruction_queue_manager queue_manager;
    ALU_manager alu_manager;
    register_manager reg_manager;
    int cycle;
    // (active_index, reg_mapping)
    stack<pair<int, vector<int> > > branch_stack;
    vector<vector<Trace> > aborted_traces;
    
    bool process(Trace *t, int trace_index, vector<Trace>& traces);
    void flush(int active_index, vector<Trace>& traces);
};

#endif /* defined(__R10K_Simulator__R10K_system_manager__) */
