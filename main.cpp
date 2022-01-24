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
    
    int array[3][3] = {{2, 7, 6}, {9, 5, 1}, {4, 3, 8}};
    auto ms = MagicSquareTemplate<int, 3, 1, 9, 1>(array);
    
    cout << ms << ms.is_magic() << endl;
    
    return 0;
}
