/*
File Name: MazeSolver.cpp
Name:Jinxiu Liu
Date:10/30/2018
Assignment: CSCI 235, Fall 2018, Project 4

This file is an implementation for MazeSolver.h, -add the header #include "MazeSolver.h"
This file will contain all the implementation for all functions (methods) in MazeSolver.h

*/
#include"MazeSolver.h"
using namespace std;

/**Parameterized constructor
@post  If successful, intput_file will be read: sets maze_rows_ and maze_columns_ from 
       the first two values in input file, and initialize the two 2-d arrays
       Otherwise, outputs "Cannot read from input_file"
@param input_file, given by the user
**/
MazeSolver::MazeSolver(std::string input_file)
{
	ifstream in_stream;
	in_stream.open(input_file);
	if (in_stream.fail())
	{
		cerr << "Cannot read from " << input_file << endl;
		exit(1);
	}
	int row, col;
	in_stream >> row;
	in_stream >> col;

	initializeMaze(row, col);
	fillMaze(in_stream);
	initializeSolution();
	maze_ready = mazeIsReady();
	in_stream.close();
}

/**
@post  allocates maze_ with rows and columns
@param rows and columns read from files.
**/

void MazeSolver::initializeMaze(int rows, int columns)
{
	maze_rows_ = rows;
	maze_columns_ = columns;

	maze_ = new char*[maze_rows_];
	for (int i = 0; i < maze_rows_; i++)
		maze_[i] = new char[maze_columns_];
}

/**
@post fills in maze_ with characters read from input file
@param input_stream
**/
void MazeSolver::fillMaze(std::ifstream& input_stream)
{
	char chara;
	for (int i = 0; i < maze_rows_; i++)
	{
		for (int j = 0; j < maze_columns_; j++)
		{
			input_stream >> chara;
			maze_[i][j] = chara;
		}
	}
}

/**
@post fills in solution_ with characters read from input file, 
      and push first position into stack
@param no param
**/
void MazeSolver::initializeSolution()
{
	
	copyMazetoSolution();
	Position an = { 0,0 };
	backtrack_stack_.push(an);
	solution_[0][0] = '>';
}

/**
@post allocates solution_ with rows and columns, 
      and fills in solution_ with characters read from input file
@param no param
**/
void MazeSolver::copyMazetoSolution()
{
	solution_ = new char*[maze_rows_];
	for (int i = 0; i < maze_rows_; i++)
		solution_[i] = new char[maze_columns_];
	for (int i = 0; i < maze_rows_; i++)
	{
		for (int j = 0; j < maze_columns_; j++)
			solution_[i][j] = maze_[i][j];
	}
}

/** Destructor
@post deletes maze_ and solution_
@param no param
**/
MazeSolver::~MazeSolver()
{
	if (maze_rows_ != 0)
	{
		for (int i = 0; i < maze_rows_; i++)
			delete[] solution_[i];
		delete[] solution_;
		solution_ = nullptr;
	}
	else {}
}

/** 
@post return: true if maze has been initialized, false otherwise
@param no param
**/
bool MazeSolver::mazeIsReady()
{
	return maze_rows_ > 0;
}

/**
@post return: true if there is solution, false otherwise
@param no param
**/
bool MazeSolver::solveMaze()
{
	int x = backtrack_stack_.top().row;
	int y = backtrack_stack_.top().column;
	while (!backtrack_stack_.empty())
	{
		if (solution_[x][y] == '$')
		{
			cout << "Found the exit!!!" << endl;
			return true;
		}
		else if (extendPath(backtrack_stack_.top()))
		{
			solution_[x][y] = '>';
			x = backtrack_stack_.top().row;
			y = backtrack_stack_.top().column;
		}
		else
		{
			maze_[x][y] = 'X';
			solution_[x][y] = '@';
			backtrack_stack_.pop();
			if (!backtrack_stack_.empty())
			{
				x = backtrack_stack_.top().row;
				y = backtrack_stack_.top().column;;
			}
			else
			{
				cout << "This maze has no solution." << endl;
				return false;
			}
		}
	}
	return false;
}

/**
@post return: true if current position can be extend, false otherwise
      and push the new positions on stack
@param current_position 
**/
bool MazeSolver::extendPath(Position current_position)
{
	bool extended = false;

	
	if (isExtensible(current_position, SOUTH))
	{
		Position newP = getNewPosition(current_position, SOUTH);
		backtrack_stack_.push(newP);
		extended = true;
	}
	if (isExtensible(current_position, EAST))
	{
		Position newP = getNewPosition(current_position, EAST);
		backtrack_stack_.push(newP);
		extended = true;
	}
	return extended;
}

/**
@post return: a new position, either to east or south of the old position
@param old_position, dir
**/
Position MazeSolver::getNewPosition(Position old_position, direction dir)
{
	Position newP;
	if (dir == SOUTH)
	{
		newP.row = old_position.row + 1;
		newP.column = old_position.column;
	}
	else
	{
		newP.row = old_position.row;
		newP.column = old_position.column + 1;
	}
	return newP;
}

/**
@post return: true the current position can be extend, false otherwise
@param current_position, dir
**/
bool MazeSolver::isExtensible(Position current_position, direction dir)
{
	int r = current_position.row;
	int c = current_position.column;
	if (dir == SOUTH)
	{
		if (r + 1 < maze_rows_ && (solution_[r + 1][c] == '_' || solution_[r + 1][c] == '$'))
			return true;
	}
	else
	{
		if (c + 1 < maze_columns_ && (solution_[r][c + 1] == '_' || solution_[r][c + 1] == '$'))
			return true;
		return false;
	}
	return false;
}

/**
@post print the solution_
@param no param
**/
void MazeSolver::printSolution()
{
	cout << "The solution to this maze is: " << endl;
	for (int i = 0; i < maze_rows_; i++)
	{
		for (int j = 0; j < maze_columns_; j++)
		{
			cout << solution_[i][j] << ' ';
		}
		cout << endl;
	}
}