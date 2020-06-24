#ifndef UNTITLED1_BUILDER_HPP
#define UNTITLED1_BUILDER_HPP

#include "../../resources/board/GameItems.hpp"

class Builder
{
    using UInt = uint32_t;
    using BoatList = std::vector<int>;

    Board<> board;
    BoatList boatList;

public:
    template<class ... Args>
    explicit Builder (
        UInt size,
        Args... sizes)
        : boatList{sizes...},
          board(size, size)
    {}

    void populateBoard ()
    {
        for (auto sz : boatList)
        {
            int x, y;
            Direction::type dir;
            do
            {
                dir = (abs(rand()) % 2) ? Direction::Right : Direction::Down;
                x = abs(rand()) % 10;
                y = abs(rand()) % 10;
            } while (!board.add_boat(x, y, dir, sz));
            board.print_board();
            std::cout << '\n';
        }
        board.print_board();
    }


};


#endif //UNTITLED1_BUILDER_HPP
