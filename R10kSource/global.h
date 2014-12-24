//
//  global.h
//  R10K_Simulator
//
//  Created by Yan Zhang on 12/13/14.
//  Copyright (c) 2014 Yan Zhang. All rights reserved.
//

#ifndef R10K_Simulator_global_h
#define R10K_Simulator_global_h

#include <string>
#include <vector>

typedef enum {
    LoadInstruction,
    StoreInstruction,
    IntegerInstruction,
    FloatAddInstruction,
    FloatMulInstruction,
    BranchInstruction,
    NoneInstruction
} InstructionType;

typedef enum {
    TraceNew,
    TraceFetched,
    TraceDecoded,
    TraceExecuted,
    TraceCommitted,
} TraceStatus;

typedef enum {
    NewStatus,
    AddressCalculatedStatus,
    MemoryAccessedStatus
} AddressInstructionStatus;

class Trace {
public:
    char op_char;
    InstructionType _op;
    int _rs;
    int _rt;
    int _rd;
    int _ord;
    std::string _extra;
    int mispredicted;
    std::vector<int> stage_cycles;
    TraceStatus status;
    
    Trace(std::string content);
};

#endif


