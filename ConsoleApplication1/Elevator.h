//Class for each elevator shaft
#include "Stop.h"
#include <cmath>
#include <cstddef>


class Elevator
{
public:
	float currFloor;

private:
	const float speed = 0.2f;
	const float stopTime = 1.0f;
	const float freeElevatorCost = 0.0f;

	Stop* nextStop;

public:
	Elevator();

	//function takes 2 floors from the single call and estimates time spent on request for this elevator
	float est_arrival_time(int floor1, int floor2);

	//fucntion inserts the new stops call floor (floor1) and destination floor (floor2) into an elevator's queue
	void add_stops(int floor1, int floor2);

	//gets the current elevator position from hardware (not existing here) and calls remove current stop when appropriate
	//for the purpose of this assignment updates position based on next stop and checks if arrived
	void update_position();

	//prints queue to the user
	void print_queue();

private:
	//removes the current stop and updates next stop
	void remove_curr_stop(bool goingUp);

};

