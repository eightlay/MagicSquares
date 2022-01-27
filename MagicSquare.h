//
//  MagicSquare.h
//  MagicSquares
//
//  Created by Timur Dzhalalov on 24.01.2022.
//
#pragma once

#include <cstdint>
#include <type_traits>
#include <vector>

#include "Utils.h"

constexpr int FRACTIONAL_SIZE = 2;

template<typename T, uint16_t N, T min_value, T max_value, uint32_t take_limit>
class MagicSquare
{
private:
    /**Square itself*/
    T square[N][N];
    /**Values range*/
    uint16_t domain_size = max_value - min_value + 1;
    /**Container to track if specific value from domain can be placed in square*/
    uint32_t taken[max_value - min_value + 1];
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
    
private:
    /**Take one place from domain's value limit. Needs to be overrided*/
    void take(T val)
    {
        this->taken[val - min_value] += 1;
    }
    
    /**Untake one place from domain's value limit. Needs to be overrided*/
    void untake(T val)
    {
        this->taken[val - min_value] -= 1;
    }
    
    /**Is number taken or not. Needs to be overrided*/
    bool is_taken(T val)
    {
        return this->taken[val - min_value] == take_limit;
    }
    
    /**NOT IMPLEMENTED*/
    uint32_t get_magic_number()
    {
        return 0;
    }
    
private:
    /**Send class to output stream*/
    friend std::ostream& operator<<(std::ostream& out, const MagicSquare& ms)
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
        for (uint32_t k = 0; k < domain_size; k++)
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
        if (std::is_signed<T>::value || min_value - (T)1 >= 0)
        {
            placeholder = min_value - (T)1;
        }
        else
        {
            placeholder = max_value + (T)1;
        }
    }
    
    /**Set value to chosen position*/
    void set(int x, int y, T val)
    {
        if ((val < min_value || val > max_value) && val != placeholder)
        {
            throw std::invalid_argument("Candidate value is out of bounds");
        }
        
        if (square[x][y] != placeholder)
        {
            untake(square[x][y]);
        }
        
        if (val != placeholder)
        {
            take(val);
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
    
    /**Fill taken*/
    void rezero_taken()
    {
        for (uint16_t k = 0; k < domain_size; k++)
        {
            taken[k] = 0;
        }
    }
    
public:
    MagicSquare()
    {
        calculate_placeholder();
        fill_domain();
        rezero_taken();
        rezero();
    }
    
    MagicSquare(T array[N][N])
    {
        calculate_placeholder();
        fill_domain();
        read(array);
    }
    
    MagicSquare<T, N, min_value, max_value, take_limit> copy()
    {
        MagicSquare<T, N, min_value, max_value, take_limit> ms(square);
        return ms;
    }
    
    /**Read square from array NxN of type T*/
    void read(T array[N][N])
    {
        rezero();
        rezero_taken();
        
        for (uint16_t i = 0; i < N; i++)
        {
            for (uint16_t j = 0; j < N; j++)
            {
                set(i, j, array[i][j]);
            }
        }
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
    
public:
    std::vector<MagicSquare<T, N, min_value, max_value, take_limit>> magic_squares_by_number(T magic_number, int limit = std::numeric_limits<int>::max())
    {
        std::vector<MagicSquare<T, N, min_value, max_value, take_limit>> v;
        
        uint32_t level = 0;
        
        while (level < depth && square[level / N][level % N] != placeholder)
        {
            level++;
        }
        
        construct_magic_squares(&v, magic_number, level, limit);
        
        return v;
    }
    
private:
    /**Try to construct magic squares*/
    bool construct_magic_squares(
                                 std::vector<MagicSquare<T, N, min_value, max_value, take_limit>>* result,
                                 T sum, int level, int limit
                                 )
    {
        while (level < depth && square[level / N][level % N] != placeholder)
        {
            level++;
        }
        
        if (level >= depth)
        {
            result->push_back(this->copy());
            return false;
        }
        
        if (result->size() >= limit)
        {
            return true;
        }
        
        int x = level / N;
        int y = level % N;
        
        for (int p = min_value; p <= max_value; p++)
        {
            if (is_taken(p))
            {
                continue;
            }
            
            if (x == ultimate && y == ultimate)
            {
                T last = sum;
                
                for (int k = 0; k < ultimate; k++)
                {
                    last -= square[k][y];
                }
                
                if (last != p)
                {
                    continue;
                }
            }
            
            set(x, y, p);
            
            // If it is penultimate value in column
            if (x == penultimate)
            {
                T last = sum;
                
                for (int k = 0; k < ultimate; k++)
                {
                    last -= square[k][y];
                }
                
                if (check_setability(last))
                {
                    set(ultimate, y, last);
                }
                else
                {
                    set(x, y, placeholder);
                    continue;
                }
            }
            
            // If it is penultimate value in row
            if (y == penultimate)
            {
                T last = sum;
                
                for (int k = 0; k < ultimate; k++)
                {
                    last -= square[x][k];
                }
                
                if (check_setability(last))
                {
                    set(x, ultimate, last);
                }
                else
                {
                    set(ultimate, y, placeholder);
                    set(x, y, placeholder);
                    continue;
                }
            }
            
            // If it is penultimate value in row and column
            if (x == penultimate && y == penultimate)
            {
                T last_main = sum;
                T last_sub = sum;
                T last_row = sum;
                
                for (int k = 0; k < ultimate; k++)
                {
                    last_main -= square[k][k];
                    last_sub -= square[k][N - k - 1];
                    last_row -= square[ultimate][k];
                }
                
                if (check_setability(last_main) && last_sub == square[ultimate][0] && last_row == last_main)
                {
                    set(ultimate, ultimate, last_main);
                }
                else
                {
                    set(x, ultimate, placeholder);
                    set(ultimate, y, placeholder);
                    set(x, y, placeholder);
                    continue;
                }
            }
            
            if (construct_magic_squares(result, sum, level + 1, limit))
            {
                return true;
            }
            
            set(ultimate, ultimate, placeholder);
            set(x, ultimate, placeholder);
            set(ultimate, y, placeholder);
            set(x, y, placeholder);
        }
        
        return false;
    }
};
