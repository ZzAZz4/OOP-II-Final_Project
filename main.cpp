#include <iostream>
#include <vector>

#include "Battleship.h"

int main()
{
    std::vector<Boat<>> fleet_1 = {Boat<>({1, 1}, Direction::H, 3),
                                   Boat<>({6, 3}, Direction::V, 3),
                                   Boat<>({4, 1}, Direction::H, 4),
                                   Boat<>({3, 2}, Direction::H, 2),
                                   Boat<>({3, 2}, Direction::H, 3)};

    Board<int> b(10, 10);
    for (const auto &boat : fleet_1)
        b.add_boat(boat);


    std::vector<Point<>> attacks = {{7, 7},
                                    {3, 2},
                                    {4, 2},
                                    {5, 5},
                                    {6, 4}};

    for (const auto &pos : attacks)
        b.receive_attack(pos);

    std::cout << '\n';
    b.print_board();

}
