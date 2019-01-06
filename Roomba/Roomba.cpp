#include "pch.h"

#include "Roomba.h"

#include <iostream>
#include <sstream>
#include <stack>
#include <time.h>

namespace RoombaCleaner
{
	// Class Room

	// Constructors
	Room::Room(unsigned int width, unsigned int height)
		: width(width), height(height)
	{
		srand(time(0));
		for (int i = 0; i < width*height; i++)
		{
			v.push_back(FILL);
		}
		RandomlyPopulateRoom();
	}

	Room::Room()
		: Room(rand() % MAX_DIM, rand() % MAX_DIM)
	{}

	// Accessors
	int Room::GetWidth()
	{
		return width;
	}

	int Room::GetHeight()
	{
		return height;
	}

	// Basic Functions
	int Room::GetPos(int x, int y)
	{
		int index = x + y * width;
		if (index >= v.size())
		{
			// TODO Throw exception
			return -1;
		}

		return index;
	}

	int Room::GetX(int index)
	{
		return index % width;
	}

	int Room::GetY(int index)
	{
		return index / width;
	}

	int Room::Move(int index, Direction d)
	{
		int x = GetX(index);
		int y = GetY(index);
		switch (d)
		{
		case Direction::North:
			y--;
			break;
		case Direction::South:
			y++;
			break;
		case Direction::West:
			x--;
			break;
		case Direction::East:
			x++;
			break;
		}

		if (x < 0 || y < 0 || x >= width || y >= height)
		{
			return -1;
		}

		return GetPos(x, y);
	}

	int Room::IsClean(int index)
	{
		return v[index] == CLEAN;
	}

	char Room::GetMovesPossible(int index)
	{
		char possibleMoves = 0;
		for (int d = 0; d < Direction::NumDirections; d++)
		{
			int newIndex = Move(index, (Direction)d);
			// We can continue to build our room layout onto
			// currently filled spaces that aren't already a part
			// of another room, and obviously are off the edge
			// of the grid
			if (newIndex != -1 && v[newIndex] == FILL && GetNumNeighbors(newIndex) == 1)
			{
				possibleMoves |= (1 << d);
			}
		}
		return possibleMoves;
	}

	// Simplest, inefficient AreNeighbors impl
	Room::Direction Room::GetMovementDirection(int first, int second)
	{
		for (int d = 0; d < Direction::NumDirections; d++)
		{
			if (Move(first, (Direction)d) == second)
			{
				return (Direction)d;
			}
		}
		return Direction::Invalid;
	}

	int Room::GetNumNeighbors(int index)
	{
		int numNeighbors = 0;
		for (int d = 0; d < Direction::NumDirections; d++)
		{
			int newIndex = Move(index, (Direction)d);
			if (newIndex != -1 && v[newIndex] != FILL)
			{
				numNeighbors++;
			}
		}
		return numNeighbors;
	}

	// Utilities
	void Room::RandomlyPopulateRoom()
	{
		// Start at some random index start
		std::stack<int> toVisit;
		int start = rand() % (width * height);
		toVisit.push(start);

		while (!toVisit.empty())
		{
			int visitedIndex = toVisit.top();
			toVisit.pop();

			// After re-visiting this index, we
			// found that its neighors have been filled in,
			// and we cannot proceed
			if (GetNumNeighbors(visitedIndex) > 1)
			{
				continue;
			}

			v[visitedIndex] = DIRTY;

			// Choose direction of the valid directions, starting in a random one
			// of the valid directions
			char movesBitMask = GetMovesPossible(visitedIndex);
			int startingDir = rand() % Direction::NumDirections;
			for (int i = 0; i < Direction::NumDirections; i++)
			{
				int direction = (startingDir + i) % Direction::NumDirections;
				if (movesBitMask & (1 << direction))
				{
					toVisit.push(Move(visitedIndex, (Direction)direction));
				}
			}
		}
	}

	// TODO: Add the roomba to the room
	void Room::PrintRoom(Roomba* r)
	{
		std::stringstream ss;
		for (int i = 0; i < v.size(); i++)
		{
			// TODO: This is slow - should only check this once
			if (r && r->GetLocation() == i)
			{
				switch (r->GetDirection())
				{
				case Direction::North:
					ss << "^";
					break;
				case Direction::West:
					ss << "<";
					break;
				case Direction::East:
					ss << ">";
					break;
				case Direction::South:
					ss << "v";
					break;
				}
			}
			else
			{
				ss << v[i];
			}
			if ((i + 1) % width == 0)
			{
				ss << "\n";
			}
		}
		std::cout << ss.str();
	}

	Roomba Room::AddRoomba()
	{
		// Find a location that's not a wall
		int location = rand() % (GetWidth() * GetHeight());
		while (v[location] == FILL)
		{
			location = rand() % (GetWidth() * GetHeight());
		}
		v[location] = CLEAN;

		// Pick a random facing
		Direction direction = (Direction)(rand() % Direction::NumDirections);

		return Roomba(location, direction);
	}

	bool Room::MoveRoombaForward(Roomba& r)
	{
		int nextLocation = Move(r.location, r.direction);
		if (nextLocation == -1 || v[nextLocation] == FILL)
		{
			return false;
		}
		r.location = nextLocation;
		v[nextLocation] = CLEAN;

		if (VERBOSE)
		{
			std::cout << "Roomba Progress:\n";
			PrintRoom(&r);
		}
		return true;
	}

	void Room::TurnRoomba(Roomba& r, Direction d)
	{
		r.direction = d;
	}

	// Class Roomba
	Roomba::Roomba(int location, Room::Direction direction)
		: location(location), direction(direction)
	{
	}

	int Roomba::GetLocation()
	{
		return location;
	}

	Room::Direction Roomba::
		GetDirection()
	{
		return direction;
	}
}
