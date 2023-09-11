#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include <set>
#include <regex>
#include<utility>

#include "inc/errorNums.h"

class ParseMsg {
	
	std::vector<std::pair<std::string, int >>ConCommands{
		{"USER", 1},
		{"SERVER",2 },
		{"QUIT",3 },
		{"SQUIT",4 },
		{"PASS",5 },
		{"NICK",6 }
	};

	std::string message;
	std::string UserParams;
	std::string ServerParams;
	std::string QuitParams;
	std::string PassParams;
	std::string NickParams;
	std::string SQuitParams;
	std::string tempParam;				 //to hold the parameters prior to parsing
	std::vector<std::string>buffer;		//to hold parameters as tokens
	std::string tempcheck;				//for parsing individual token
	std::string buff;					//for stringstream purposes	
	std::string com;					//to store the command which user had input
	std::vector<std::string>::iterator curr_token; 
	bool check;			
	int pos;

	ParseMsg(std::string usrmsg) :message(usrmsg), 
	check(false){

		com = message.substr(0, 1);
		tempParam = message.substr(9, message.size());
	
		for (const auto& ComPair : ConCommands) {
			if (ComPair.first == com) {
				check = true;
				pos = ComPair.second;
				break;
			}
		}

		if (!check)
			ParsError(ERR_INVLD_COMMND);
		else
			assignParams();
	}

	void assignParams() {

		switch (pos) {
		case 1:
			UserParams = tempParam;
			UserParse();
			break;
		case 2:
			ServerParams = tempParam;
			ServerParse();
			break;
		case 3:
			QuitParams = tempParam;
			QuitParse();
			break;
		case 4:
			SQuitParams = tempParam;
			SQuitParse();
			break;
		case 5:
			PassParams = tempParam;
			PassParse();
			break;
		case 6:
			NickParams = tempParam;
			NickPArse();
			break;
		}

	}

	void UserParse() {

		std::stringstream parse(UserParams);

		while (parse >> buff) {
			buffer.emplace_back(buff);
		}

		//since user command have this syntax :
		//USER <username> <hostname> <servername> <realname>

		if (buffer.size() != 4)
			ParsError(ERR_NEEDMOREPARAMS);

		curr_token = buffer.begin();
		tempcheck = *curr_token;

		if (!ServerNameRegex(tempcheck))
			ParsError(ERR_INLVLD_PARAMS);
		
		std::advance(curr_token, 1);
		tempcheck = *curr_token;

		if (!HostNameRegex(tempcheck))
			ParsError(ERR_INLVLD_PARAMS);

		std::advance(curr_token, 1);
		tempcheck = *curr_token;
		
		w
	}

	bool ServerNameRegex(std::string ServerParam) {
		std::smatch smatch;

		std::regex serverPattern("^((([a-zA-Z][a-zA-Z0-9-]*)+\\.)"
			"+[a-zA-Z][a-zA-Z0-9-]*)$");

		if (std::regex_match(ServerParam, smatch, serverPattern))
			return true;
		return false;
	}

	bool HostNameRegex(std::string ServerParam) {
		std::smatch smatch;

		std::regex serverPattern("^((([a-zA-Z][a-zA-Z0-9-]*)+\\.)"
			"+[a-zA-Z][a-zA-Z0-9-]*)$");

		if (std::regex_match(ServerParam, smatch, serverPattern))
			return true;
	}

	void ServerParse() {

		std::stringstream parse(ServerParams);
		while (parse >> buff) {
			buffer.emplace_back(buff);
		}
	}

	void QuitParse() {

		std::stringstream parse(QuitParams);
		while (parse >> buff) {
			buffer.emplace_back(buff);
		}

	}

	void SQuitParse() {

		std::stringstream parse(SQuitParams);
		while (parse >> buff) {
			buffer.emplace_back(buff);
		}
	}

	void PassParse() {

		std::stringstream parse(PassParams);
		while (parse >> buff) {
			buffer.emplace_back(buff);
		}
		
	}
	
	void NickPArse() {

		std::stringstream parse(NickParams);
		while (parse >> buff) {
			buffer.emplace_back(buff);
		}
	}

	
};

int main() {

	return 0;
}