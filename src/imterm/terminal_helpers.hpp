#ifndef TERMINAL_HELPER_HPP
#define TERMINAL_HELPER_HPP

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                                                     ///
///  Copyright C 2019, Lucas Lazare                                                                                                     ///
///  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation         ///
///  files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy,         ///
///  modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software     ///
///  is furnished to do so, subject to the following conditions:                                                                        ///
///                                                                                                                                     ///
///  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.     ///
///                                                                                                                                     ///
///  The Software is provided “as is”, without warranty of any kind, express or implied, including but not limited to the               ///
///  warranties of merchantability, fitness for a particular purpose and noninfringement. In no event shall the authors or              ///
///  copyright holders be liable for any claim, damages or other liability, whether in an action of contract, tort or otherwise,        ///
///  arising from, out of or in connection with the software or the use or other dealings in the Software.                              ///
///                                                                                                                                     ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <set>
#include <array>

#include "terminal.hpp"

namespace ImTerm {


	// terminal_helper_example is meant to be an example
	// if you want to inherit from one, pick term::basic_terminal_helper (see below)
	template <typename T>
	class terminal_helper_example {
	public:
		using value_type = T; // < Mandatory, this type will be passed to commands via argument_type
		using term_t = ImTerm::terminal<terminal_helper_example>;
		using command_type = ImTerm::command_t<ImTerm::terminal<terminal_helper_example>>;
		using argument_type = ImTerm::argument_t<ImTerm::terminal<terminal_helper_example>>;
		using command_type_cref = std::reference_wrapper<const command_type>;

		// mandatory : return every command starting by prefix
		std::vector<command_type_cref> find_commands_by_prefix(std::string_view prefix) {

			auto compare_by_name = [](const command_type& cmd) { return cmd.name; };
			auto map_to_cref = [](const command_type& cmd) { return std::cref(cmd); };

			return misc::prefix_search(prefix, cmd_list.begin(), cmd_list.end(), std::move(compare_by_name), std::move(map_to_cref));
		}

		// mandatory : return every command starting by the text formed by [beg, end)
		std::vector<command_type_cref> find_commands_by_prefix(const char* beg, const char* end) {
			return find_commands_by_prefix({beg, static_cast<unsigned>(end - beg)});
		}

		// mandatory: returns the full command list
		std::vector<command_type_cref> list_commands() {
			return find_commands_by_prefix(std::string_view{});
		}

		// mandatory: formats the given string.
		// msg type is either user_input, error, or cmd_history_completion
		// return an empty optional if you do not want the string to be logged
		// message's members 'is_term_message' and 'severity' are ignored
		std::optional<ImTerm::message> format(std::string str, [[maybe_unused]] ImTerm::message::type msg_type) {
			ImTerm::message msg;
			msg.color_beg = msg.color_end = 0u;
			msg.value = std::move(str);
			return {std::move(msg)}; // other fields are ignored
		}

		// optional : to be friendlier with non ascii encoding
		// return value : 0 if str does not begin by a space
		//                otherwise, the number of characters participating in the representation of the beginning space
		// should not return a value > str.size()
		// the passed string_view is never empty (at least one char)
//		int is_space(std::string_view str) const {
//			return str[0] == ' ';
//		}

		// optional : to be friendlier with non ascii encoding
		// return value : number of glyphs represented in str (== str.size() for ascii)
//		unsigned long get_length(std::string_view str) {
//	      return str.size();
//		}

		// optional : called right after terminal's construction. You may store the terminal for future usage
//		void set_terminal(term_t& term) {
//		}



		// command samples (implemented as static methods, but they can be outside of a class, if you will to)
		static std::vector<std::string> no_completion(argument_type&) { return {}; }

		static void clear(argument_type& arg) {
			arg.term.clear();
		}

		static void echo(argument_type& arg) {
			std::string str{};
			str = arg.command_line[1];
			for (auto it = std::next(arg.command_line.begin(), 2) ; it != arg.command_line.end() ; ++it) {
				str.reserve(str.size() + it->size() + 1);
				str += ' ';
				str += *it;
			}
			message msg;
			msg.value = std::move(str);
			msg.color_beg = msg.color_end = 0; // color is disabled when color_beg == color_end
			// other parameters are ignored
			arg.term.add_message(std::move(msg));
		}

		static constexpr std::array<command_type, 2> cmd_list = {
				command_type{"clear ", "clears the screen", clear, no_completion},
				command_type{"echo ", "prints text to the screen", echo, no_completion},
		};
	};

	// Basic terminal helper
	// You may inherit to save some hassle
	// Template parameter TerminalHelper is in most cases the derived class (and should be if you don't know what to put)
	// Template parameter Value is the type passed to commands together with the other arguments
	// You may add commands with the 'add_command_' method.
	// Refer to terminal_helper_example (see above) for a commented example
	template <typename TerminalHelper, typename Value>
	class basic_terminal_helper {
	public:
		using value_type = Value;
		using term_t = ImTerm::terminal<TerminalHelper>;
		using command_type = ImTerm::command_t<ImTerm::terminal<TerminalHelper>>;
		using argument_type = ImTerm::argument_t<ImTerm::terminal<TerminalHelper>>;
		using command_type_cref = std::reference_wrapper<const command_type>;

		basic_terminal_helper() = default;
		basic_terminal_helper(const basic_terminal_helper&) = default;
		basic_terminal_helper(basic_terminal_helper&&) noexcept = default;

		std::vector<command_type_cref> find_commands_by_prefix(std::string_view prefix) {
			auto compare_name = [](const command_type& cmd) { return cmd.name; };
			auto map_to_cref = [](const command_type& cmd) { return std::cref(cmd); };

			return misc::prefix_search(prefix, cmd_list_.begin(), cmd_list_.end(), std::move(compare_name),
			                           std::move(map_to_cref));
		}

		std::vector<command_type_cref> find_commands_by_prefix(const char * beg, const char * end) {
			return find_commands_by_prefix({beg, static_cast<unsigned>(end - beg)});
		}

		std::vector<command_type_cref> list_commands() {
			std::vector<command_type_cref> ans;
			ans.reserve(cmd_list_.size());
			for (const command_type& cmd : cmd_list_) {
				ans.emplace_back(cmd);
			}
			return ans;
		}

		std::optional<ImTerm::message> format(std::string str, ImTerm::message::type) {
			ImTerm::message msg;
			msg.value = std::move(str);
			msg.color_beg = msg.color_end = 0u;
			return {std::move(msg)};
		}

	protected:
		void add_command_(const command_type& cmd) {
			cmd_list_.emplace(cmd);
		}

		std::set<command_type> cmd_list_{};
	};
}

#endif //TERMINAL_HELPER_HPP
