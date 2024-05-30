//
// Created by TaxMachine on 2024-05-29.
//

#include "../Command.hpp"

#include <iostream>

void Command::echo(argument_type& args) {
    std::string str = std::move(args.command_line[1]);
    for (auto it = std::next(args.command_line.begin(), 2); it != args.command_line.end(); it++) {
        str += " " + std::move(*it);
    }
    ImTerm::message message;
    message.value = std::move(str);
    message.color_beg = message.color_end = 0;
    args.term.add_message(std::move(message));
}