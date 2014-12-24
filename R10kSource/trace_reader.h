//
//  trace_reader.h
//  R10K_Simulator
//
//  Created by Yan Zhang on 12/13/14.
//  Copyright (c) 2014 Yan Zhang. All rights reserved.
//

#ifndef __R10K_Simulator__trace_reader__
#define __R10K_Simulator__trace_reader__

#include "global.h"
using namespace std;

class trace_reader {
public:
    vector<Trace> read_from_file(string fileName);
};

#endif /* defined(__R10K_Simulator__trace_reader__) */
