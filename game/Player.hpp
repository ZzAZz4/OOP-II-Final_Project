//
// Created by esteb on 6/24/2020.
//

#ifndef UNTITLED1_PLAYER_HPP
#define UNTITLED1_PLAYER_HPP

#include "base/Builder.hpp"
#include "base/Selector.hpp"
#include "base/Parser.hpp"
#include "com/Message.hpp"

class Player
{
    unsigned ID;
    Builder builder;
    Selector selector;
    Parser parser;

    enum class actions
    {
        Handshake, Place, Attack
    };

public:
    void receiveMessage (Message message)
    {

    }

    Message createMessage ()
    {

    }

    auto attack ()
    {

    }


};


#endif //UNTITLED1_PLAYER_HPP
