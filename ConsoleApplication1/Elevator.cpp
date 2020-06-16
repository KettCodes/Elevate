#include "Elevator.h"
#include <iostream>
#include <vector>

	Elevator::Elevator() {
		currFloor = 0;
		nextStop = NULL;
	}

	//function takes 2 floors from the single call and estimates time spent on request for this elevator
	float Elevator::est_arrival_time(int floor1, int floor2) {

		//est for stopped elevator could add an oppertunity cost of commiting free elevator
		if (nextStop == NULL)
			return (abs(floor1 - currFloor) / speed) + (abs(floor1 - floor2) / speed) + freeElevatorCost;

		bool goingUp = (currFloor < nextStop->floor);
		bool pickUp = (floor1 > currFloor);
		bool dropUp = (floor2 > floor1);
		Stop* listPtr = nextStop;
		int subCost = 0;

		//est for elevator moving in same direction as needed for pickup and dropoff
		//could also count # of stops after the drop off floor in future iteration for impact on other riders
		if (goingUp == pickUp && goingUp == dropUp) {
			if (goingUp) {//iterates up
				for (listPtr = nextStop; listPtr->stop_above != NULL && listPtr->floor <= floor2; listPtr = listPtr->stop_above)
					if (listPtr->floor != floor1 && listPtr->floor != floor2 && !listPtr->onHold) subCost += stopTime;
				if (listPtr->floor > floor2) return subCost;
			}
			else {//iterates down
				for (listPtr = nextStop; listPtr->stop_below != NULL && listPtr->floor >= floor2; listPtr = listPtr->stop_below)
					if (listPtr->floor != floor1 && listPtr->floor != floor2 && !listPtr->onHold) subCost += stopTime;
				if (listPtr->floor < floor2) return subCost;
			}
			//captures additional movement if needed
			return (subCost)+(abs(listPtr->floor - floor2) / speed);
		}


		//est for elevator moving opposite direction as needed for dropoff
		if (goingUp != dropUp) {
			if (goingUp) {//iterates up then down
				for (listPtr = nextStop; listPtr->stop_above != NULL; listPtr = listPtr->stop_above)
					if (listPtr->floor != floor1 && !listPtr->onHold) subCost += stopTime;
				subCost += 2 * (abs(listPtr->floor - floor1) / speed);
				if (nextStop->stop_below == NULL)
					return subCost + (abs(floor1 - floor2) / speed);
				else {
					for (listPtr = nextStop; listPtr->stop_below != NULL; listPtr = listPtr->stop_below)
						if (listPtr->floor != floor1 && listPtr->floor != floor2 && !listPtr->onHold) subCost += stopTime;
					if (listPtr->floor < floor2) return subCost;
					return subCost + (abs(listPtr->floor - floor2) / speed);
				}
			}
			else {//iterates down then up
				for (listPtr = nextStop; listPtr->stop_below != NULL; listPtr = listPtr->stop_below)
					if (listPtr->floor != floor1 && !listPtr->onHold) subCost += stopTime;
				subCost += 2 * (abs(listPtr->floor - floor1) / speed);
				if (nextStop->stop_above == NULL)
					return subCost + (abs(floor1 - floor2) / speed);
				else {
					for (listPtr = nextStop; listPtr->stop_above != NULL; listPtr = listPtr->stop_above)
						if (listPtr->floor != floor1 && listPtr->floor != floor2 && !listPtr->onHold) subCost += stopTime;
					if (listPtr->floor > floor2) return subCost;
					return subCost + (abs(listPtr->floor - floor2) / speed);
				}
			}
		}


		//est for worst case needing to switch directions twice different pickup same dropoff
		if (goingUp) {//iterates up then down then up
			for (listPtr = nextStop; listPtr->stop_above != NULL; listPtr = listPtr->stop_above)
				if (!listPtr->onHold) subCost += stopTime;
			subCost += 2 * (abs(listPtr->floor - currFloor) / speed);
			if (nextStop->stop_below == NULL) return subCost + ((abs(currFloor - floor1) + abs(floor1 - floor2)) / speed);
			for (listPtr = nextStop; listPtr->stop_below != NULL; listPtr = listPtr->stop_below)
				if (listPtr->floor != floor1 && !listPtr->onHold) subCost += stopTime;
			subCost += 2 * (abs(listPtr->floor - floor1) / speed);
			return subCost + (abs(floor1 - floor2) / speed);
		}
		else {//iterates down then up then down
			for (listPtr = nextStop; listPtr->stop_below != NULL; listPtr = listPtr->stop_below)
				if (!listPtr->onHold) subCost += stopTime;
			subCost += 2 * (abs(listPtr->floor - currFloor) / speed);
			if (nextStop->stop_above == NULL) return subCost + ((abs(currFloor - floor1) + abs(floor1 - floor2)) / speed);
			for (listPtr = nextStop; listPtr->stop_above != NULL; listPtr = listPtr->stop_above)
				if (listPtr->floor != floor1 && !listPtr->onHold) subCost += stopTime;
			subCost += 2 * (abs(listPtr->floor - floor1) / speed);
			return subCost + (abs(floor1 - floor2) / speed);
		}

	}

	//fucntion inserts the new stops call floor (floor1) and destination floor (floor2) into an elevator's queue
	void Elevator::add_stops(int floor1, int floor2) {

		//for stationary elevator
		if (nextStop == NULL) {
			nextStop = new Stop(floor1);
			if (floor2 > floor1)
				nextStop->stop_above = new Stop(floor2);
			else
				nextStop->stop_below = new Stop(floor2);
			return;
		}

		bool goingUp = (currFloor < nextStop->floor);
		bool pickUp = (floor1 > currFloor);
		bool dropUp = (floor2 > floor1);
		Stop* listPtr = nextStop;
		Stop* tempPtr;

		//inserting when traveling in a single direction start to finish
		if (goingUp == pickUp && goingUp == dropUp)
			if (goingUp) {
				if (nextStop->floor >= floor1) {
					if (nextStop->floor > floor1) {
						tempPtr = new Stop(floor1);
						tempPtr->stop_above = nextStop;
						tempPtr->stop_below = nextStop->stop_below;
						nextStop = tempPtr;
					}
					for (listPtr = nextStop; listPtr->stop_above != NULL && !listPtr->stop_above->onHold && (listPtr->stop_above->floor < floor2);
						listPtr = listPtr->stop_above);
					if (listPtr->stop_above == NULL || listPtr->stop_above->onHold || listPtr->stop_above->floor > floor2) {
						tempPtr = new Stop(floor2);
						tempPtr->stop_above = listPtr->stop_above;
						listPtr->stop_above = tempPtr;
					}
				}
				else {
					for (listPtr = nextStop; listPtr->stop_above != NULL && !listPtr->stop_above->onHold && (listPtr->stop_above->floor < floor1);
						listPtr = listPtr->stop_above);
					if (listPtr->stop_above == NULL || listPtr->stop_above->onHold) {
						tempPtr = new Stop(floor1);
						tempPtr->stop_above = listPtr->stop_above;
						listPtr->stop_above = tempPtr;
						tempPtr = new Stop(floor2);
						tempPtr->stop_above = listPtr->stop_above->stop_above;
						listPtr->stop_above->stop_above = tempPtr;
						return;
					}
					else {
						if (listPtr->stop_above->floor > floor1) {
							tempPtr = new Stop(floor1);
							tempPtr->stop_above = listPtr->stop_above;
							listPtr->stop_above = tempPtr;
						}
						for (listPtr = listPtr; listPtr->stop_above != NULL && !listPtr->stop_above->onHold && (listPtr->stop_above->floor < floor2);
							listPtr = listPtr->stop_above);
						if (listPtr->stop_above == NULL || listPtr->stop_above->onHold || listPtr->stop_above->floor > floor2) {
							tempPtr = new Stop(floor2);
							tempPtr->stop_above = listPtr->stop_above;
							listPtr->stop_above = tempPtr;
						}

					}
				}
				return;
			}
			else {
				if (nextStop->floor <= floor1) {
					if (nextStop->floor < floor1) {
						tempPtr = new Stop(floor1);
						tempPtr->stop_below = nextStop;
						tempPtr->stop_above = nextStop->stop_above;
						nextStop = tempPtr;
					}
					for (listPtr = nextStop; listPtr->stop_below != NULL && !listPtr->stop_below->onHold && (listPtr->stop_below->floor > floor2);
						listPtr = listPtr->stop_below);
					if (listPtr->stop_below == NULL || listPtr->stop_below->onHold || listPtr->stop_below->floor < floor2) {
						tempPtr = new Stop(floor2);
						tempPtr->stop_below = listPtr->stop_below;
						listPtr->stop_below = tempPtr;
					}
				}
				else {
					for (listPtr = nextStop; listPtr->stop_below != NULL && !listPtr->stop_below->onHold && (listPtr->stop_below->floor > floor1);
						listPtr = listPtr->stop_below);
					if (listPtr->stop_below == NULL || listPtr->stop_below->onHold) {
						tempPtr = new Stop(floor1);
						tempPtr->stop_below = listPtr->stop_below;
						listPtr->stop_below = tempPtr;
						tempPtr = new Stop(floor2);
						tempPtr->stop_below = listPtr->stop_below->stop_below;
						listPtr->stop_below->stop_below = tempPtr;
						return;
					}
					else {
						if (listPtr->stop_below->floor < floor1) {
							tempPtr = new Stop(floor1);
							tempPtr->stop_below = listPtr->stop_below;
							listPtr->stop_below = tempPtr;
						}
						for (listPtr = listPtr; listPtr->stop_below != NULL && !listPtr->stop_below->onHold && (listPtr->stop_below->floor > floor2);
							listPtr = listPtr->stop_below);
						if (listPtr->stop_below == NULL || listPtr->stop_below->onHold || listPtr->stop_below->floor < floor2) {
							tempPtr = new Stop(floor2);
							tempPtr->stop_below = listPtr->stop_below;
							listPtr->stop_below = tempPtr;
						}

					}
				}
				return;
			}

		//inserting when travelling opposite direction to dropoff
		if (goingUp != dropUp)
			if (!goingUp) {
				for (listPtr = nextStop; listPtr->stop_above != NULL && !listPtr->stop_above->onHold && (listPtr->stop_above->floor < floor1);
					listPtr = listPtr->stop_above);
				if (listPtr->stop_above == NULL || listPtr->stop_above->onHold) {
					tempPtr = new Stop(floor1);
					tempPtr->stop_above = listPtr->stop_above;
					listPtr->stop_above = tempPtr;
					tempPtr = new Stop(floor2);
					tempPtr->stop_above = listPtr->stop_above->stop_above;
					listPtr->stop_above->stop_above = tempPtr;
					return;
				}
				else {
					if (listPtr->stop_above->floor > floor1) {
						tempPtr = new Stop(floor1);
						tempPtr->stop_above = listPtr->stop_above;
						listPtr->stop_above = tempPtr;
					}
					for (listPtr = listPtr; listPtr->stop_above != NULL && !listPtr->stop_above->onHold && (listPtr->stop_above->floor < floor2);
						listPtr = listPtr->stop_above);
					if (listPtr->stop_above == NULL || listPtr->stop_above->onHold || listPtr->stop_above->floor > floor2) {
						tempPtr = new Stop(floor2);
						tempPtr->stop_above = listPtr->stop_above;
						listPtr->stop_above = tempPtr;
					}

				}
				return;
			}
			else {
				for (listPtr = nextStop; listPtr->stop_below != NULL && !listPtr->stop_below->onHold && (listPtr->stop_below->floor > floor1);
					listPtr = listPtr->stop_below);
				if (listPtr->stop_below == NULL || listPtr->stop_below->onHold) {
					tempPtr = new Stop(floor1);
					tempPtr->stop_below = listPtr->stop_below;
					listPtr->stop_below = tempPtr;
					tempPtr = new Stop(floor2);
					tempPtr->stop_below = listPtr->stop_below->stop_below;
					listPtr->stop_below->stop_below = tempPtr;
					return;
				}
				else {
					if (listPtr->stop_below->floor < floor1) {
						tempPtr = new Stop(floor1);
						tempPtr->stop_below = listPtr->stop_below;
						listPtr->stop_below = tempPtr;
					}
					for (listPtr = listPtr; listPtr->stop_below != NULL && !listPtr->stop_below->onHold && (listPtr->stop_below->floor > floor2);
						listPtr = listPtr->stop_below);
					if (listPtr->stop_below == NULL || listPtr->stop_below->onHold || listPtr->stop_below->floor < floor2) {
						tempPtr = new Stop(floor2);
						tempPtr->stop_below = listPtr->stop_below;
						listPtr->stop_below = tempPtr;
					}

				}
				return;
			}

		//inserting when changing directions twice
		if (goingUp) {
			for (listPtr = nextStop; listPtr->stop_above != NULL && !listPtr->stop_above->onHold; listPtr = listPtr->stop_above);
			if (listPtr->stop_above == NULL) {
				tempPtr = new Stop(floor2);
				tempPtr->stop_above = listPtr->stop_above;
				tempPtr->onHold = true;
				listPtr->stop_above = tempPtr;
			}
			else {
				for (listPtr = listPtr; listPtr->stop_above != NULL && (listPtr->stop_above->floor < floor2); listPtr = listPtr->stop_above);
				if (listPtr->stop_above == NULL || listPtr->stop_above->floor > floor2) {
					tempPtr = new Stop(floor2);
					tempPtr->stop_above = listPtr->stop_above;
					tempPtr->onHold = true;
					listPtr->stop_above = tempPtr;
				}
			}
			for (listPtr = nextStop; listPtr->stop_below != NULL && !listPtr->stop_below->onHold && (listPtr->stop_below->floor > floor1);
				listPtr = listPtr->stop_below);
			if (listPtr->stop_below == NULL || listPtr->stop_below->onHold || (listPtr->stop_below->floor < floor1)) {
				tempPtr = new Stop(floor1);
				tempPtr->stop_below = listPtr->stop_below;
				listPtr->stop_below = tempPtr;
			}
			return;
		}
		else {
			for (listPtr = nextStop; listPtr->stop_below != NULL && !listPtr->stop_below->onHold; listPtr = listPtr->stop_below);
			if (listPtr->stop_below == NULL) {
				tempPtr = new Stop(floor2);
				tempPtr->stop_below = listPtr->stop_below;
				tempPtr->onHold = true;
				listPtr->stop_below = tempPtr;
			}
			else {
				for (listPtr = listPtr; listPtr->stop_below != NULL && (listPtr->stop_below->floor > floor2); listPtr = listPtr->stop_below);
				if (listPtr->stop_below == NULL || listPtr->stop_below->floor < floor2) {
					tempPtr = new Stop(floor2);
					tempPtr->stop_below = listPtr->stop_below;
					tempPtr->onHold = true;
					listPtr->stop_below = tempPtr;
				}
			}
			for (listPtr = nextStop; listPtr->stop_above != NULL && !listPtr->stop_above->onHold && (listPtr->stop_above->floor < floor1);
				listPtr = listPtr->stop_above);
			if (listPtr->stop_above == NULL || listPtr->stop_above->onHold || (listPtr->stop_above->floor > floor1)) {
				tempPtr = new Stop(floor1);
				tempPtr->stop_above = listPtr->stop_above;
				listPtr->stop_above = tempPtr;
			}
			return;
		}

	}

	//gets the current elevator position from hardware (not existing here) and calls remove current stop when appropriate
	//for the purpose of this assignment updates position based on next stop and checks if arrived
	void Elevator::update_position() {

		if (nextStop != NULL) {
			bool goingUp = (currFloor < nextStop->floor);
			currFloor += goingUp - 0.5;
			if (abs(currFloor - nextStop->floor) < 0.5)
				remove_curr_stop(goingUp);
		}
	}

	void Elevator::print_queue()
	{
		if (nextStop != NULL) {
			std::vector<int> floorList;
			Stop* listPtr = nextStop;
			while (listPtr != NULL) {
				listPtr = listPtr->stop_below;
				if (listPtr != NULL)
					floorList.push_back(listPtr->floor);
			}
			std::cout << std::endl;
			std::reverse(floorList.begin(), floorList.end());
			for (auto x : floorList)
				std::cout << x << ", ";
			std::cout << '(' << nextStop->floor << "), ";
			listPtr = nextStop;
			while (listPtr != NULL) {
				listPtr = listPtr->stop_above;
				if (listPtr != NULL)
					std::cout << listPtr->floor << ", ";
			}
			std::cout << std::endl;
		}
		else {
			std::cout << "\nNothing In Queue" << std::endl;
		}
	}

	//removes the current stop and updates next stop
	void Elevator::remove_curr_stop(bool goingUp) {
		if (nextStop == NULL)
			return;

		Stop* tempStop;
		Stop* listPtr;

		if (goingUp) {
			if (nextStop->stop_above == NULL) {
				goingUp = false;
				tempStop = nextStop->stop_below;
				if (tempStop != NULL)
					tempStop->stop_above = nextStop->stop_above;
			}
			else if (nextStop->stop_above->onHold) {
				goingUp = false;
				tempStop = nextStop->stop_below;
				if (tempStop != NULL)
					tempStop->stop_above = nextStop->stop_above;
				for (listPtr = nextStop->stop_above; listPtr != NULL; listPtr = listPtr->stop_above)
					listPtr->onHold = false;
			}
			else {
				tempStop = nextStop->stop_above;
				tempStop->stop_below = nextStop->stop_below;
			}
			delete nextStop;
			nextStop = tempStop;
		}
		else {
			if (nextStop->stop_below == NULL) {
				goingUp = true;
				tempStop = nextStop->stop_above;
				if (tempStop != NULL)
					tempStop->stop_below = nextStop->stop_below;
			}
			else if (nextStop->stop_below->onHold) {
				goingUp = true;
				tempStop = nextStop->stop_above;
				if (tempStop != NULL)
					tempStop->stop_below = nextStop->stop_below;
				for (listPtr = nextStop->stop_below; listPtr != NULL; listPtr = listPtr->stop_below)
					listPtr->onHold = false;
			}
			else {
				tempStop = nextStop->stop_below;
				tempStop->stop_above = nextStop->stop_above;
			}
			delete nextStop;
			nextStop = tempStop;
		}

	}


