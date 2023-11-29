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
	std::string username;
	std::string nickname;
	std::vector<std::string>buffer;		//to hold parameters as tokens
	std::string tempcheck;				//for parsing individual token
	std::string buff;					//for stringstream purposes	
	std::string com;					//to store the command which user had input
	std::vector<std::string>::iterator curr_token;
	std::vector<std::string>user_details;
	bool check;			
	int pos;
	int hopcount;


public:
	void Parse(std::string usrmsg){
		message = usrmsg;
		check = false;
		std::stringstream parse(usrmsg);

		parse >> com;
		tempParam = parse.str().substr(com.size() + 1);
		for (const auto& ComPair : ConCommands) {
			if (ComPair.first == com) {
				check = true;
				pos = ComPair.second;
				break;
			}
		}

		if (!check)
			ParsError(ERR_INVLD_CMMND);
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

		if (!UserNameRegex(tempcheck))
			ParsError(ERR_INVLD_PARAMS);

		user_details.push_back(tempcheck);
		std::advance(curr_token, 1);
		tempcheck = *curr_token;
		
		if (!HostNameRegex(tempcheck))
			ParsError(ERR_INVLD_PARAMS);

		std::advance(curr_token, 1);
		tempcheck = *curr_token;
		if (!ServerNameRegex(tempcheck))
			ParsError(ERR_INVLD_PARAMS);

		std::advance(curr_token, 1);
		tempcheck = *curr_token;

		std::string coloncheck = tempcheck.substr(0, 1);
		std::string suffixparams = tempcheck.substr(1, tempcheck.size());
		if (coloncheck != ":")
			ParsError(ERR_INVLD_PARAMS);

		if (!RealNameRegex(suffixparams))
			ParsError(ERR_INVLD_PARAMS);

		std::cout <<std::endl<< "command is ok"<<std::endl;
	}

	bool UserNameRegex(std::string Param) {
		std::smatch smatch;

		std::regex UserPattern("[A-Za-z0-9]*");

		if (std::regex_match(Param, smatch, UserPattern))
			return true;
		return false;
	}

	bool ServerNameRegex(std::string Param) {
		std::smatch smatch;

		std::regex serverPattern("^(?!irc\\.)((([[:alnum:]-]+)(\\.([[:alnum:]-]+))*)|((\\d{1,3}\\.){3}\\d{1,3})|((\\[?([a-fA-F0-9]{0,4}:){1,7}[a-fA-F0-9]{0,4}\\]?)(:\\d+)?))$");

		if (std::regex_match(Param, smatch, serverPattern))
			return true;
		return false;
	}

	bool HostNameRegex(std::string Param) {
		std::smatch smatch;

		std::regex HostPattern("^(?!www\.|ftp\.)((([[:alnum:]-]+)\.)+[[:alnum:]-]{1,255})$");

		if (std::regex_match(Param, smatch, HostPattern))
			return true;
		return false;
	}

	bool RealNameRegex(std::string Param) {
		std::smatch smatch;

		std::regex RealnamePattern("^([^\\r\\n\\0]*)$");

		if (std::regex_match(Param, smatch, RealnamePattern))
			return true;
		return false;
	}

	bool QuitParse() {

		std::stringstream parse(QuitParams);
		while (parse >> buff) {
			buffer.emplace_back(buff);
		}
		if (buffer.size() != 1)
			ParsError(ERR_INVLD_PARAMS);
		else
			curr_token = buffer.begin();
			tempcheck = *curr_token;
			std::string coloncheck = tempcheck.substr(0, 1);
			std::string msg = tempcheck.substr(1, tempcheck.size());
			if (coloncheck != ":")
				ParsError(ERR_INVLD_FRMT);
			std::smatch match;
			std::regex msgPattern(":(.+)");
			if (std::regex_match(tempcheck, match, msgPattern))
				return true;
			return false;


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
	
	bool NickPArse() {

		std::stringstream parse(NickParams);
		buffer = {};
		while (parse >> buff) {
			
			buffer.emplace_back(buff);
		}
		if (buffer.size() != 1)
			ParsError(ERR_INVLD_PARAMS);

		curr_token = buffer.begin();
		tempcheck = *curr_token;
		user_details.push_back(tempcheck);
		
		std::smatch match;
		std::regex nickPattern("^((([a-zA-Z][a-zA-Z0-9-]*)+\\.)"
			"+[a-zA-Z][a-zA-Z0-9-]*)$");
		if (std::regex_match(tempcheck, match, nickPattern))
			return true;
		return false;
	}


	void ServerParse() {

		std::stringstream parse(ServerParams);
		while (parse >> buff) {
			buffer.emplace_back(buff);
		}
		if (buffer.size() != 3)
			ParsError(ERR_INVLD_PARAMS);

		curr_token = buffer.begin();
		tempcheck = *curr_token;
		std::string coloncheck = tempcheck;

	}

	auto FetchUSer() {
		return user_details;
	}
};
