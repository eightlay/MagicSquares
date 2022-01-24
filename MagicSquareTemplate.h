//
//  MagicSquareTemplate.h
//  MagicSquares
//
//  Created by Timur Dzhalalov on 24.01.2022.
//
#pragma once

#include <cstdint>
#include <type_traits>

#include "Utils.h"

constexpr int FRACTIONAL_SIZE = 2;

template<typename T, uint16_t N, T min_value, T max_value, uint32_t take_limit>
class MagicSquareTemplate
{
protected:
    /**Square itself*/
    T square[N][N];
    /**Domain of the square*/
    T domain[max_value - min_value + 1];
    /**Placeholder for square*/
    mutable T placeholder;
    /**Penultimate row or column for square*/
    const uint16_t penultimate = N - 2;
    /**Ultimate row or column for square*/
    const uint16_t ultimate = N - 1;
    /**Depth of the squre: number of elements to fill square*/
    const uint32_t depth = N * N;
    /**Container to track if specific value from domain can be placed in square*/
    uint32_t taken[max_value - min_value + 1];
    
protected:
    /**Fill taken. Needs to be overrided*/
    virtual void rezero_taken()
    {
        
    }
    
    /**Take one place from domain's value limit. Needs to be overrided*/
    virtual void take(uint16_t i, uint16_t j)
    {
        
    }
    
    /**Untake one place from domain's value limit. Needs to be overrided*/
    virtual void untake(uint16_t i, uint16_t j)
    {
        
    }
    
    /**Is number on position p taken or not. Needs to be overrided*/
    virtual bool is_taken(int p)
    {
        return false;
    }
    
    /**Check if square is magic one. Needs to be overrided*/
    virtual bool get_magic_number()
    {
        return false;
    }
    
private:
    /**Send class to output stream*/
    friend std::ostream& operator<<(std::ostream& out, const MagicSquareTemplate& ms)
    {
        unsigned long size = 0;
        
        T col_sums[N];
        
        for (int j = 0; j < N; j++)
        {
            T col_sum = 0;
            
            for (int i = 0; i < N; i++)
            {
                col_sum += ms.square[i][j];
            }
            
            col_sums[j] = col_sum;
            
            unsigned long current_size = std::to_string((int)col_sum).length();
            if (current_size > size)
            {
                size = current_size;
            }
        }
        
        if (std::is_floating_point<T>::value)
        {
            size += FRACTIONAL_SIZE;
        }
        
        print_add_dashes(&out, N, size);
        
        out << std::endl;
        
        for (int i = 0; i < N; i++)
        {
            T row_sum = 0;
            
            for (int j = 0; j < N; j++)
            {
                row_sum += ms.square[i][j];
                
                std::string val = std::to_string(ms.square[i][j]);
                val = val.substr(0, size);
                
                for (int k = 0; k < size - val.length(); k++)
                {
                    out << " ";
                }
                
                out << " " << val << " ";
            }
            
            out << " " << row_sum << std::endl;
        }
        
        print_add_dashes(&out, N, size);
        
        out << std::endl;
        
        T diag_sum = 0;
        T sub_diag_sum = 0;
        
        for (int j = 0; j < N; j++)
        {
            diag_sum += ms.square[j][j];
            sub_diag_sum += ms.square[j][N - j - 1];
            
            std::string val = std::to_string(col_sums[j]);
            val = val.substr(0, size);
            
            for (int k = 0; k < size - val.length(); k++)
            {
                out << " ";
            }
            
            out << " " << col_sums[j]  << " ";
        }
        
        std::string val = std::to_string(diag_sum);
        val = val.substr(0, size);
        
        for (int k = 0; k < size - val.length(); k++)
        {
            out << " ";
        }
        
        out << " " << diag_sum << "|" << sub_diag_sum;
        
        return out << std::endl;
    }
    
    /**Fill domain*/
    void fill_domain()
    {
        for (uint32_t k = 0; k < depth; k++)
        {
            domain[k] = k + 1;
        }
    }
    
    /**Set all elements to zero*/
    void rezero()
    {
        for (uint16_t i = 0; i < N; i++)
        {
            for (uint16_t j = 0; j < N; j++)
            {
                square[i][j] = placeholder;
            }
        }
    }
    
    /**Calculate placeholder depending on data type*/
    void calculate_placeholder()
    {
        if (std::is_signed<T>::value)
        {
            placeholder = min_value - (T)1;
        }
        else
        {
            placeholder = max_value + (T)1;
        }
    }
    
    /**Mark currently taken numbers. Needs to be overrided*/
    void mark_taken()
    {
        for (uint16_t i = 0; i < N; i++)
        {
            for (uint16_t j = 0; j < N; j++)
            {
                if (square[i][j] != placeholder)
                {
                    take(i, j);
                }
            }
        }
    }
    
    /**Set value to chosen position*/
    void set(int x, int y, T val)
    {
        if (val == placeholder)
        {
            untake(x, y);
        }
        else
        {
            take(x, y);
        }
        square[x][y] = val;
    }
    
    /**Get value from chosen position*/
    bool check_setability(T val)
    {
        if (val >= min_value && val <= max_value)
        {
            return !is_taken(val);
        }
        
        return false;
    }
    
public:
    MagicSquareTemplate()
    {
        calculate_placeholder();
        fill_domain();
        rezero_taken();
        rezero();
    }
    
    MagicSquareTemplate(T array[N][N])
    {
        calculate_placeholder();
        fill_domain();
        read(array);
    }
    
    /**Read square from array NxN of type T*/
    void read(T array[N][N])
    {
        rezero_taken();
        
        for (uint16_t i = 0; i < N; i++)
        {
            for (uint16_t j = 0; j < N; j++)
            {
                square[i][j] = array[i][j];
            }
        }
        
        mark_taken();
    }
    
    /**Check if sum of row or column elements equals required*/
    bool check(T sum, int k, bool check_row = true)
    {
        if (check_row)
        {
            for (uint16_t j = 0; j < N; j++)
            {
                sum -= square[k][j];
            }
        }
        else
        {
            for (uint16_t i = 0; i < N; i++)
            {
                sum -= square[i][k];
            }
        }
        
        return sum == 0;
    }
    
    /**Check if sum of main (d == True) or side (d == False) diagonal elements equals required*/
    bool check(T sum, bool main_d)
    {
        if (main_d)
        {
            for (uint16_t k = 0; k < N; k++)
            {
                sum -= square[k][k];
            }
        }
        else
        {
            for (uint16_t k = 0; k < N; k++)
            {
                sum -= square[k][N - k - 1];
            }
        }
        
        return sum == 0;
    }
    
    /**Check if square is magic regarding the supposed magic number*/
    bool check(T sum)
    {
        bool flag = true;
        
        for (uint16_t k = 0; k < N; k++)
        {
            flag &= check(sum, k) & check(sum, k, false);
        }
        
        flag &= check(sum, true) & check(sum, false);
        
        return flag;
    }
    
    /**Check if square magic or not*/
    bool is_magic()
    {
        T sum = get_magic_number();
        
        return check(sum);
    }
};
