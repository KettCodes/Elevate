// Andrew Kett, starting from a console application project
//
// This project manages different elevator queues operating from the command line
// I did limited error checking so it assumes only integers will be entered
// The project also assumes destinations are selected before entering an elevator
// While not the most common model it better represents both users and the data entry to this system.

#include <iostream>
#include <vector>
#include "Elevator.h"

enum Options { Call = 1, View = 2,  All = 3, Move = 4, Quit = 5};
bool checkInt(int, int, int);

int main()
{
    bool test = false;
    int numElevators, numFloors, pick = 0, src, dest, index;
    float score, toBeat;
    Elevator* elevator;

    std::vector <Elevator*> elevators;

    std::cout << "Hello Lets Move Some Elevators\n";

    while (!test) {
        std::cout << "How Many Elevators (1-20)\n";
        std::cin >> numElevators;
        test = checkInt(1, 20, numElevators);
    }
    test = false;
    while (!test) {
        std::cout << "How Many Floors (1-50)\n";
        std::cin >> numFloors;
        test = checkInt(1, 50, numFloors);
    }
    for (int i = 0; i < numElevators; i++)
        elevators.push_back(new Elevator);
    
    while (pick != Quit) {
        std::cout << "\n\n1. Call - Call an Elevator\n"
            << "2. View - View an Elevator's Queue\n"
            << "3. All - See all Elevator Positions\n"
            << "4. Move - Move all Elevator's some time\n"
            << "5. Quit - Clean up and exit program\n"
            << "Enter a number 1-5:\n";
        std::cin >> pick;

        switch (pick)
        {
        case Call:
            std::cout << "Floor to Call From:\n";
            std::cin >> src;
            if (!checkInt(1, numFloors, src)) break;
            std::cout << "Floor to Travel To:\n";
            std::cin >> dest;
            if (!checkInt(1, numFloors, dest)) break;
            toBeat = -1.0f;
            elevator = elevators[0];
            for (Elevator* x : elevators) {
                score = x->est_arrival_time(src, dest);
                if (toBeat < 0.0f || score < toBeat) {
                    toBeat = score;
                    elevator = x;
                }
            }
            elevator->add_stops(src, dest);
            break;

        case View:
            std::cout << "Elevator to Print Queue From:\n";
            std::cin >> index;
            if (!checkInt(1, numElevators, index)) break;
            std::cout << "Here is the queue with next floor in ()\n"
                << "Floors to move down to are on the left\n"
                << "Floors to move up to are on the right\n";
            elevators[index-1]->print_queue();
            break;

        case All:
            std::cout << "The Elevators are at the following Floor Heights" << std::endl;
            for (Elevator* x : elevators)
                std::cout << x->currFloor << ", ";
            std::cout << std::endl;
            break;

        case Move:
            std::cout << "Steps For Each Elevator To Take (1/2 floor per step):\n";
            std::cin >> index;
            for (index; index>0; index--)
                for (Elevator* x : elevators)
                    x->update_position();
            break;

        case Quit:
            break;
        default:
            std::cout << "Did Not Recognize Command\n";
            break;
        }
    }


}

bool checkInt(int min, int max, int entered) {
    if (entered < min || entered > max)
        std::cout << "\n **Entry Error** \n";
    return (entered >= min) && (entered <= max);
}