//
//  main.cpp
//  MagicSquares
//
//  Created by Timur Dzhalalov on 24.01.2022.
//

//#include <iostream>
//using namespace std;

#include "MagicSquare.h"
#include "SquareFinder.h"

#include <iostream>
using namespace std;

int main(int argc, const char * argv[])
{
    
//    uint32_t array[3][3] = {{2, 7, 6}, {9, 5, 1}, {4, 3, 8}};

    // MagicSquare
    
//    auto squares = SquareFinder::magic_squares_by_number<int, 3, 5, 13, 4>(36, 1);
    auto squares = SquareFinder::magic_squares<int, 4, 5, 13, 4>(3);
    
    for (auto square : squares)
    {
        cout << square << endl;
    }
    
    return 0;
}
