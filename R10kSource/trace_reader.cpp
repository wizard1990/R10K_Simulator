//
//  trace_reader.cpp
//  R10K_Simulator
//
//  Created by Yan Zhang on 12/13/14.
//  Copyright (c) 2014 Yan Zhang. All rights reserved.
//

#include "trace_reader.h"
#include <fstream>

int power_int(int _unit, int _power) {
    int ans = 1;
    while (_power) {
        ans *= _unit;
        _power--;
    }
    return ans;
}

int hexToDec(string hex) {
    int dec = 0, unit, power;
    
    for (int i = (int)hex.length() - 1; i >= 0; i--) {
        if (hex[i] >= '0' && hex[i]<='9') {
            unit = hex[i] - '0';
            power = (int)hex.length() - 1 - i;
            dec += unit * power_int(16, power);
        } else if (hex[i] >= 'a' && hex[i] <= 'f') {
            unit = hex[i] - 'a' + 10;
            power = (int)hex.length() - 1 - i;
            dec += unit * power_int(16, power);
        } else if (hex[i] >= 'A' && hex[i] <= 'F') {
            unit = hex[i] - 'A' + 10;
            power = (int)hex.length() - 1 - i;
            dec += unit * power_int(16, power);
        }
    }
    return dec;
}

Trace::Trace(string content) {
    status = TraceNew;
    op_char = content[0];
    switch (op_char) {
        case 'I':
            _op = IntegerInstruction;
            break;
        case 'L':
            _op = LoadInstruction;
            break;
        case 'S':
            _op = StoreInstruction;
            break;
        case 'B':
            _op = BranchInstruction;
            break;
        case 'A':
            _op = FloatAddInstruction;
            break;
        case 'M':
            _op = FloatMulInstruction;
            break;
        default:
            _op = NoneInstruction;
            break;
    }
    _rs = hexToDec(content.substr(2, 2));
    _rt = hexToDec(content.substr(5, 2));
    _rd = _ord = hexToDec(content.substr(8, 2));
    
    if (_op == LoadInstruction || _op == StoreInstruction || _op == BranchInstruction) {
        _rd = -1;
    }
    _extra = "";
    if (content.length() > 10) {
        
        _extra = content.substr(11);
        if (_op == BranchInstruction) {
            mispredicted = _extra[0] == '1';
        } else {
            mispredicted = false;
        }
    } else {
        mispredicted = false;
    }
}

vector<Trace> trace_reader::read_from_file(string fileName) {
    vector<Trace> res;
    ifstream infile(fileName);
    string line;
    while (getline(infile, line))
    {
        res.push_back(Trace(line));
    }
    return res;
}



