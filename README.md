# MagicSquares

## Requirements
C++11 or newer

## Usage
MagicSquare class is a template class that takes:
- T - data type of possible square values
- N - length of square's side
- min_value - minimal value in square's possible values
- max_value - maximal value in square's possible values
- take_limit - how much times you can take specific value in square's possible values

FindSquare class has some functions to find magic squares.

## Example

Consider classical magic square with side of length 3 and possible values from 1 to 9 without repetitions:

`MagicSquare<unsigned int, 3, 1, 9, 1>`

To find first 4 such magic squares:

`SquareFinder::magic_squares<unsigned int, 3, 1, 9, 1>(4)`
