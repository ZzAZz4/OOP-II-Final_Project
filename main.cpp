#include "game/base/Selector.hpp"
#include "resources/board/GameItems.hpp"

int main ()
{
    /*
    Selector s(10, 5, 4, 4, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 1);

    for (auto i = 0; i < 19; ++i)
    {
        auto move = s.getMove();
        s.makeUpdate(move, UpdateMessage::Miss);
        s.updatePriority();
    }
    */
    Board<> board (10, 10);
    board.print_board();
    std::cout << std::endl;
    board.add_boat(3, 3, Direction::Right, 3);
    board.add_boat(8, 6, Direction::Up, 4);
    board.print_board();

}