//
// Created by TaxMachine on 2024-05-29.
//

#include "../Command.hpp"

#include <iostream>

void Command::whoami(argument_type& args) {
    ImTerm::message message;
    message.value = "TaxMachine";
    message.color_beg = message.color_end = 0;
    args.term.add_message(std::move(message));
}