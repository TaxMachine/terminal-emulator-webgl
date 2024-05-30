//
// Created by TaxMachine on 2024-05-29.
//

#ifndef TERMINAL_ANIMATION_COMMAND_HPP
#define TERMINAL_ANIMATION_COMMAND_HPP

#include <imterm/terminal.hpp>
#include <imterm/terminal_helpers.hpp>

class Command : public ImTerm::basic_terminal_helper<Command, void> {
public:
    Command() {
        for (const command_type& cmd : local_command_list) {
            add_command_(cmd);
        }
    }

    static std::vector<std::string> no_completion(argument_type&) { return {}; }

    static void clear(argument_type&);
    static void echo(argument_type&);
    static void help(argument_type&);
    static void whoami(argument_type&);

private:
    constexpr static std::array<command_type, 5> local_command_list = {
            command_type{"clear", "Clear the terminal", &Command::clear, &Command::no_completion},
            command_type{"echo", "Echo the input", &Command::echo, &Command::no_completion},
            command_type{"help", "Display help", &Command::help, &Command::no_completion},
            command_type{"whoami", "Display the current user", &Command::whoami, &Command::no_completion}
    };
};

#endif //TERMINAL_ANIMATION_COMMAND_HPP
