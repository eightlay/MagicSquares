//
//  Utils.h
//  MagicSquares
//
//  Created by Timur Dzhalalov on 24.01.2022.
//
#pragma once

#include <iostream>

void print_add_dashes(std::ostream* out, int N, unsigned long size)
{
    for (int k = 0; k < N; k++)
    {
        *out << "--";
        
        for (int p = 0; p < size; p++)
        {
            *out << "-";
        }
    }
}

