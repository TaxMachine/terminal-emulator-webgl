//
// Created by TaxMachine on 2024-05-29.
//

#include "../Command.hpp"

void Command::help(argument_type& args) {
    std::string str;
    for (auto& cmd : local_command_list) {
        str.append(cmd.name);
        str.append("\n\t");
        str.append(cmd.description);
        str.append("\n\n");
    }
    ImTerm::message message;
    message.value = std::move(str);
    message.color_beg = message.color_end = 0;
    args.term.add_message(message);
}