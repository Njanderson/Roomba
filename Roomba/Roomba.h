#pragma once

#include <vector>

#define VERBOSE 1

#define MAX_DIM 100
#define FILL 'X'
#define DIRTY '0'
#define CLEAN '1'

// TODO: If I were to redo this, I probably would have a toplevel module,
// and then you could do MoveRoomba, which would grab information from Roomba,
// and then execute move on the room, altering the 

namespace RoombaCleaner
{
	// Forward declare
	class Roomba;

	class Room
	{
	public:
		// TODO: Probably should use enum class,
		// in which case it's harder to generate a
		// random direction
		enum Direction {
			North = 0,
			East,
			South,
			West,
			NumDirections,
			Invalid
		};

		Room();
		Room(unsigned int width, unsigned int height);
		// TODO: This is gross
		void PrintRoom(Roomba* r = NULL);
		int GetWidth();
		int GetHeight();
		Roomba AddRoomba();

		// Helpers added
		// TODO: Probably should move these to utils...
		Direction GetMovementDirection(int first, int second);
		int IsClean(int index);

		// API for implementing the "Clean Room" Solution
		// TODO: Put this into another module - weird incestuous 
		// relationship between Roomba and Room
		bool MoveRoombaForward(Roomba& r);
		void TurnRoomba(Roomba& r, Direction d);

		friend class Roomba;

	private:
		int GetX(int index);
		int GetY(int index);
		int GetPos(int x, int y);
		
		char GetMovesPossible(int index);
		int GetNumNeighbors(int index);
		void RandomlyPopulateRoom();
		int Move(int index, Direction d);

		int width;
		int height;
		std::vector<char> v;
	};

	// A representation of where the Roomba currently is
	// TODO: The Roomba should be able to execute "Move"
	// knowing what its current room is, and then be able
	// to tell a caller whether it was successful in moving
	// THEN, the CleanRooms class only needs to deal with 
	// writing a program for the Room, which makes more sense
	class Roomba
	{
	public:
		Roomba(int location, Room::Direction direction);
		
		int GetLocation();
		Room::Direction GetDirection();
		
		// TODO: Fix this.
		friend class Room;
	private:
		int location;
		Room::Direction direction;
	};

	class CleanRooms
	{
		
	};

}