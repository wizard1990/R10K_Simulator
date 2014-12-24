//
//  main.cpp
//  R10K_Simulator
//
//  Created by Yan Zhang on 12/13/14.
//  Copyright (c) 2014 Yan Zhang. All rights reserved.
//

#include <iostream>
#include <cstdio>
#include "trace_reader.h"
#include "R10K_system_manager.h"

bool traces_all_done(vector<Trace>& traces) {
    for (int i = 0; i < traces.size(); i++) {
        if (traces[i].status != TraceCommitted) {
            return false;
        }
    }
    return true;
}

int next_decoded_trace(vector<Trace>& traces) {
    for (int i = 0; i < traces.size(); i++) {
        if (traces[i].status == TraceDecoded) {
            return i;
        }
    }
    return -1;
}

void decode_trace(vector<Trace>& traces, int cycle) {
    for (int i = 0; i < traces.size(); i++) {
        if (traces[i].status == TraceFetched) {
            traces[i].status = TraceDecoded;
            traces[i].stage_cycles.push_back(cycle);
            return;
        }
    }
}

void fetch_trace(vector<Trace>& traces, int cycle) {
    for (int i = 0; i < traces.size(); i++) {
        if (traces[i].status == TraceNew) {
            traces[i].status = TraceFetched;
            traces[i].stage_cycles.push_back(cycle);
            return;
        }
    }
}

int main(int argc, const char * argv[]) {
    // insert code here...
    if (argc != 2) {
        cout << "Input error!\n" << endl;
        return -1;
    }
    trace_reader reader;
    
    vector<Trace> traces = reader.read_from_file(argv[1]);
    
    R10K_system_manager manager;
    manager.cycle = 0;
    
    pair<bool, int> flush_flag = make_pair(false, -1);
    
    while (!traces_all_done(traces)) {
        if (flush_flag.first) {
            flush_flag.first = false;
            manager.flush(flush_flag.second, traces);
            Trace new_branch = traces[manager.active_manager.active_list[flush_flag.second].instr_index];
            new_branch.mispredicted = false;
            new_branch.status = TraceNew;
            new_branch.stage_cycles.clear();
            traces.insert(traces.begin() + manager.active_manager.active_list[flush_flag.second].instr_index + 1, new_branch);
        } else {
            int instr_index = next_decoded_trace(traces);
            bool stall;
            if (instr_index > -1) {
                stall = manager.process(&traces[instr_index], instr_index, traces);
            } else {
                stall = manager.process(nullptr, -1, traces);
            }
            if (!stall) {
                decode_trace(traces, manager.cycle);
                fetch_trace(traces, manager.cycle);
            }
            int alu1_active_index = manager.alu_manager.integer_ALU1.current_instruction.front().first;
            if (manager.active_manager.active_list[alu1_active_index].instrType == BranchInstruction &&
                manager.active_manager.active_list[alu1_active_index].done_bit &&
                traces[manager.active_manager.active_list[alu1_active_index].instr_index].mispredicted) {
                flush_flag = make_pair(true, alu1_active_index);
            }
        }
        manager.cycle++;
    }
    
    for (int i = 0; i < traces.size(); i++) {
        printf("%c %02X %02X %02X: ", traces[i].op_char, traces[i]._rs, traces[i]._rt, traces[i]._ord);
        switch (traces[i]._op) {
            case IntegerInstruction:
            case BranchInstruction: {
                int last = traces[i].stage_cycles.back();
                int cur = 0;
                for (int j = 0; j <= last; j++) {
                    if (traces[i].stage_cycles[cur] == j) {
                        switch (cur) {
                            case 0:
                                cout << "|F";
                                break;
                            case 1:
                                cout << "|D";
                                break;
                            case 2:
                                cout << "|I";
                                break;
                            case 3:
                                cout << "|E";
                                break;
                            case 4:
                                cout << "|C|" << endl;
                            default:
                                break;
                        }
                        cur++;
                    } else {
                        if (cur == 0) {
                            cout << "  ";
                        } else {
                            cout << "| ";
                        }
                    }
                }
                if (traces[i]._op == BranchInstruction && traces[i].mispredicted) {
                    vector<Trace> aborted = manager.aborted_traces.front();
                    for (int i = (int)aborted.size() - 1; i >= 0; i--) {
                        printf("%c %02X %02X %02X: XXX aborted.\n", aborted[i].op_char, aborted[i]._rs, aborted[i]._rt, aborted[i]._ord);
                    }
                    manager.aborted_traces.erase(manager.aborted_traces.begin());
                }
                break;
            }
            case FloatAddInstruction:
            case FloatMulInstruction: {
                int last = traces[i].stage_cycles.back();
                int cur = 0;
                for (int j = 0; j <= last; j++) {
                    if (traces[i].stage_cycles[cur] == j) {
                        switch (cur) {
                            case 0:
                                cout << "|F";
                                break;
                            case 1:
                                cout << "|D";
                                break;
                            case 2:
                                cout << "|I";
                                break;
                            case 3:
                                cout << "|E";
                                break;
                            case 4:
                                cout << "|F";
                                break;
                            case 5:
                                cout << "|C|" << endl;
                            default:
                                break;
                        }
                        cur++;
                    } else {
                        if (cur == 0) {
                            cout << "  ";
                        } else {
                            cout << "| ";
                        }
                    }
                }
                break;
            }
            case LoadInstruction:
            case StoreInstruction: {
                int last = traces[i].stage_cycles.back();
                int cur = 0;
                for (int j = 0; j <= last; j++) {
                    if (traces[i].stage_cycles[cur] == j) {
                        switch (cur) {
                            case 0:
                                cout << "|F";
                                break;
                            case 1:
                                cout << "|D";
                                break;
                            case 2:
                                cout << "|I";
                                break;
                            case 3:
                                cout << "|A";
                                break;
                            case 4:
                                cout << "|E";
                                break;
                            case 5:
                                cout << "|C|" << endl;
                            default:
                                break;
                        }
                        cur++;
                    } else {
                        if (cur == 0) {
                            cout << "  ";
                        } else {
                            cout << "| ";
                        }
                    }
                }
                break;
            }
            default:
                break;
        }
    }
    cout << "Total cycles: " << traces.back().stage_cycles.back() + 1 << endl;
    return 0;
}
