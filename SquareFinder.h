//
//  SquareFinder.h
//  MagicSquares
//
//  Created by Timur Dzhalalov on 24.01.2022.
//
#pragma once

#include <vector>

#include "MagicSquare.h"

class SquareFinder
{
public:
    /**Find magic squares by known magic number*/
    template<typename T, uint16_t N, T min_value, T max_value, uint32_t take_limit>
    static std::vector<MagicSquare<T, N, min_value, max_value, take_limit>> magic_squares_by_number(T magic_number, int limit = std::numeric_limits<int>::max())
    {
        return MagicSquare<T, N, min_value, max_value, take_limit>().magic_squares_by_number(magic_number, limit);
    }
    /**Find magic squares without known magic number*/
    template<typename T, uint16_t N, T min_value, T max_value, uint32_t take_limit>
    static std::vector<MagicSquare<T, N, min_value, max_value, take_limit>> magic_squares(int limit = std::numeric_limits<int>::max())
    {
        T from_mult = (take_limit > N) ? N : take_limit;
        
        for (T magic_number = max_value * from_mult; magic_number > min_value * from_mult - 1; magic_number--)
        {
            std::vector<MagicSquare<T, N, min_value, max_value, take_limit>> v = MagicSquare<T, N, min_value, max_value, take_limit>().magic_squares_by_number(magic_number, limit);
            
            if (v.size() > 0)
            {
                return v;
            }
        }
        
        return std::vector<MagicSquare<T, N, min_value, max_value, take_limit>>();
    }
};
