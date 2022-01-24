//
//  main.cpp
//  MagicSquares
//
//  Created by Timur Dzhalalov on 24.01.2022.
//

#include <iostream>
using namespace std;

#include "MagicSquareTemplate.h"

int main(int argc, const char * argv[]) {
    
    auto ms = MagicSquareTemplate<int, 3, 1, 9, 1>();
    
    cout << ms;
    
    return 0;
}
