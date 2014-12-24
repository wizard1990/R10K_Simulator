//
//  ALU_manager.h
//  R10K_Simulator
//
//  Created by Yan Zhang on 12/13/14.
//  Copyright (c) 2014 Yan Zhang. All rights reserved.
//

#ifndef __R10K_Simulator__ALU_manager__
#define __R10K_Simulator__ALU_manager__

#include "ALU.h"

class ALU_manager {
public:
    ALU fpadd_ALU;
    ALU fpmul_ALU;
    ALU integer_ALU1;
    ALU integer_ALU2;
    
    ALU_manager(): fpadd_ALU(ALU(true)), fpmul_ALU(ALU(true)), integer_ALU1(ALU(false)), integer_ALU2(ALU(false)) {};
    void flush(int active_index, register_manager& manager) {
        fpadd_ALU.flush(active_index, manager);
        fpmul_ALU.flush(active_index, manager);
        integer_ALU1.flush(active_index, manager);
        integer_ALU2.flush(active_index, manager);
    }
};

#endif /* defined(__R10K_Simulator__ALU_manager__) */
