#pragma once

#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include<unordered_map>

enum ErrList {

	//NICk errors
	ERR_NONICKNAMEGIVEN,
	ERR_ERRONEUSNICKNAME,
	ERR_NICKNAMEINUSE,
	ERR_NICKCOLLISION,
	//USER errors 
	ERR_ALREADYREGISTRED,
	//JOIN errors
	ERR_BANNEDFROMCHAN,
	ERR_INVITEONLYCHAN,
	ERR_BADCHANNELKEY,
	ERR_CHANNELISFULL,
	ERR_BADCHANMASK,
	ERR_NOSUCHCHANNEL,
	ERR_TOOMANYCHANNELS,
	RPL_TOPIC,
	RPL_NAMREPLY,
	RPL_ENDOFNAMES,
	//PART errors
	ERR_NEEDMOREPARAMS,
	ERR_NOTONCHANNEL,
	//CHANNEL (MODE) errors
	ERR_UNKNOWNMODE,
	ERR_KEYSET,
	RPL_BANLIST,
	RPL_ENDOFBANLIST,
	RPL_CHANNELMODEIS,
	//user(MODE) erros
	ERR_NOSUCHNICK,
	ERR_USERSDONTMATCH,
	ERR_UMODEUNKNOWNFLAG,
	RPL_UMODEIS,
	//LIST erros
	ERR_NOSUCHSERVER,
	RPL_LISTSTART,
	RPL_LIST,
	RPL_LISTEND,
	//INVITE error
	ERR_USERONCHANNEL,
	ERR_CHANOPRIVSNEEDED,
	ERR_INVLD_CMMND,
	ERR_INVLD_PARAMS,
	ERR_INVLD_FRMT

};

std::unordered_map<ErrList, const char*> ErrMap = {
	{ERR_NONICKNAMEGIVEN, "No nickname given"},
	{ERR_ERRONEUSNICKNAME, "Erroneous nickname"},
	{ERR_NICKNAMEINUSE, "Nickname is already in use"},
	{ERR_NICKCOLLISION, "Nickname collision"},
	{ERR_ALREADYREGISTRED, "Already registered"},
	{ERR_BANNEDFROMCHAN, "Banned from channel"},
	{ERR_INVITEONLYCHAN, "Channel is invite-only"},
	{ERR_BADCHANNELKEY, "Bad channel key"},
	{ERR_CHANNELISFULL, "Channel is full"},
	{ERR_BADCHANMASK, "Bad channel mask"},
	{ERR_NOSUCHCHANNEL, "No such channel"},
	{ERR_TOOMANYCHANNELS, "Too many channels"},
	{RPL_TOPIC, "Topic"},
	{RPL_NAMREPLY, "Name reply"},
	{RPL_ENDOFNAMES, "End of names"},
	{ERR_NOTONCHANNEL, "Not on channel"},
	{ERR_NEEDMOREPARAMS, "Need more parameters"},
	{ERR_CHANOPRIVSNEEDED, "Channel operator privileges needed"},
	{ERR_UNKNOWNMODE, "Unknown mode"},
	{ERR_KEYSET, "Key is set"},
	{RPL_BANLIST, "Ban list"},
	{RPL_ENDOFBANLIST, "End of ban list"},
	{RPL_CHANNELMODEIS, "Channel mode is"},
	{ERR_USERSDONTMATCH, "Users don't match"},
	{ERR_UMODEUNKNOWNFLAG, "Unknown user mode flag"},
	{RPL_UMODEIS, "User mode is"},
	{ERR_NOSUCHSERVER, "No such server"},
	{RPL_LISTSTART, "List start"},
	{RPL_LIST, "List"},
	{RPL_LISTEND, "List end"},
	{ERR_NOSUCHNICK, "No such nickname"},
	{ERR_USERONCHANNEL, "User is on channel"},
	{ERR_INVLD_CMMND, "No such command"},
	{ERR_INVLD_PARAMS,"Invalid paramaters"},
	{ERR_INVLD_FRMT,"Invalid message format"}
};

class errorparse {

	std::string message;
	ErrList Err ;

public:
	errorparse() {}

	errorparse(ErrList Error) {
		Err = Error;
		if (ErrMap.find(Error) != ErrMap.end()) {
			message = ErrMap[Error];
		}
		else {
			message = "unknown error on Parsing";
		}
	}

	ErrList desc() const {
		return Err;
	}

	std::string error() {
		return message;
	}

};

void ParsError(ErrList err) {
	errorparse obj(err);
	std::cerr <<"Error code "<< obj.desc() << ": " << obj.error();
	exit(err);
}





