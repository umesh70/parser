#pragma once


#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include <set>
#include <regex>

class CLientParser {

	struct parsed {
		const char* pref;
		const char* com;
		const char* prms;
	}prsOBJ;

	std::string message;
	std::string prefix;
	std::string command;
	std::string params;
	std::string buff;
	std::stringstream parse;
	std::vector<std::string>buffer;
	std::vector<std::string>::iterator cur_comp;
	std::vector<std::string>::size_type size;
	std::set<std::string>curr_users;

public:

	auto tokenizer(std::string usermsg){
	
		message = usermsg;

		while (parse >> buff)
			buffer.emplace_back(buff);

		cur_comp = buffer.begin();

		prefix = *cur_comp;
		std::advance(cur_comp, 1);
		command = *cur_comp;
		std::advance(cur_comp, 1);

		while (cur_comp != buffer.end()) {
			params += *cur_comp;
			if (std::next(cur_comp) != buffer.end()) {
				params += " ";
			}
			std::advance(cur_comp, 1);
		}

		prefix_parse();
		command_parse();
		params_parse();
	}

	void prefix_parse() {

		std::string colon = prefix.substr(0, 1);
		std::string usrsrvrnme = prefix.substr(1, prefix.size());
		std::smatch matches;
		std::regex serverPattern("^((([a-zA-Z][a-zA-Z0-9-]*)+\\.)"     
								"+[a-zA-Z][a-zA-Z0-9-]*)$");

		std::regex nickPattern("[a-zA-Z][a-zA-Z0-9!@#$%^&*()]*");

		std::regex extendedPattern("([a-zA-Z][a-zA-Z0-9!@#$%^&*()]*)"
									"!([^\\x20\\x00\\x0d\\x0a]+)"
									"@([a-zA-Z][a-zA-Z0-9-]*)(\\.[a-zA-Z][a-zA-Z0-9-]*)*");

		if (colon != ":") {
			std::cout << "colon missing.";
			exit(1);
		}
		else if (std::regex_match(usrsrvrnme, matches, extendedPattern)) {
			std::cout << "extended prefix matched:" << matches.str() << std::endl;
			
		}

		else if (std::regex_match(usrsrvrnme, matches, serverPattern)) {
			std::cout << "servername matched:" << matches.str() << std::endl;
			
		}

		else if (std::regex_match(usrsrvrnme, matches, nickPattern)) {
			std::cout << "nickname matched:" << matches.str() << std::endl;
			
		}
		else
		std::cout << "no valid servername or nickname or extended prefix";

	}

	void command_parse() {
		
		std::smatch match;
		std::regex commandptrn("^([a-zA-Z]+|[0-9][0-9][0-9])$");

		if (std::regex_match(command, match, commandptrn)) {
			std::cout << "command matched: " << match.str() << std::endl;

		}
	}

	void params_parse() {
		std::smatch match;
		std::regex paramPattern(R"(^(\s+|)(:\S*|[^:\s\0\r\n]+\s*)*)");
		
		/*R represents a raw string literal, 
		  does not interpret escape sequences*/ 

		if (regex_match(params, match, paramPattern)) {
			std::cout << "paramaters matched: " << match.str() << std::endl;
		}
	}	

	auto returnparsed() {

		prsOBJ.pref = prefix.c_str();
		prsOBJ.com = command.c_str();
		prsOBJ.prms = params.c_str();
		return prsOBJ;
	}

	
};
