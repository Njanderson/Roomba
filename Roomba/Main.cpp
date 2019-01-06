#include "pch.h"
#include <iostream>
#include <stack>
#include <unordered_set>

#include "Roomba.h"

using namespace RoombaCleaner;

Room::Direction Reverse(Room::Direction d)
{
	return (Room::Direction) (((int)d + 2) % Room::Direction::NumDirections);
}

void CleanRoom(Room& r, Roomba& roomba)
{
	std::unordered_set<int> visited;
	std::stack<int> toExplore;
	std::stack<Room::Direction> moves;
	toExplore.push(roomba.GetLocation());
	

	while (!toExplore.empty())
	{
		int toVisit = toExplore.top();
		toExplore.pop();

		// Can we move there directly/are we already there?
		// If not, we need to undo our movements until
		// we can.

		while (roomba.GetLocation() != toVisit && r.GetMovementDirection(roomba.GetLocation(), toVisit) == Room::Direction::Invalid)
		{
			Room::Direction direction = moves.top();
			moves.pop();

			r.TurnRoomba(roomba, Reverse(direction));
			r.MoveRoombaForward(roomba);
		}

		// If we aren't already there, move there...
		if (roomba.GetLocation() != toVisit)
		{
			Room::Direction directionToMove = r.GetMovementDirection(roomba.GetLocation(), toVisit);
			moves.push(directionToMove);
			r.TurnRoomba(roomba, directionToMove);
			r.MoveRoombaForward(roomba);
		}

		visited.insert(roomba.GetLocation());

		for (int i = 0; i < Room::Direction::NumDirections; i++)
		{
			r.TurnRoomba(roomba, (Room::Direction)i);
			if (r.MoveRoombaForward(roomba))
			{
				// To prevent the roomba from cleaning behind itself
				if (visited.find(roomba.GetLocation()) == visited.end())
				{
					toExplore.push(roomba.GetLocation());
				}
				r.TurnRoomba(roomba, Reverse(roomba.GetDirection()));
				r.MoveRoombaForward(roomba);
			}
		}
	}
}

int main()
{
	// Create a room
	Room r(50, 50);

	// Show the room
	r.PrintRoom();

	// Initialize a Roomba in the room
	Roomba roomba = r.AddRoomba();

	std::cout << "Cleaning room...\n";

	CleanRoom(r, roomba);

	// Show the room
	r.PrintRoom();
}