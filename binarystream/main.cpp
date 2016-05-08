//
//  main.cpp
//  binarystream
//
//  Created by 聪宁陈 on 16/5/7.
//  Copyright © 2016年 ccnyou. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "binarystream.h"
using namespace std;

int main(int argc, const char * argv[]) {
    cc::binarystream bss;
    bss << "ok" << endl;
	bss << 0x3131 << endl;
    const vector<char> &v = bss.vector();
    for (auto it = v.begin(); it != v.end(); ++it) {
        cout << (int)*it << ":(" << (char)*it << ")  ";
    }
    cout << endl;
    
    return 0;
}
