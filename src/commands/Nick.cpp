#include "commands/Nick.hpp"
#include "ResponseMessage.hpp"
#include "Server.hpp"
#include "Logger.hpp"
#include <algorithm>

Nick::Nick(Server* server) : _server(server) {}

Nick::~Nick() {}

void Nick::execute(Client* client, const std::vector<std::string>& params) {
    if (params.empty()) {
        ResponseMessage::sendError(client, ERR_NONICKNAMEGIVEN, ":No nickname given");
        return;
    }
    
    std::string nickname = params[0];
    
    if (isNicknameInUse(nickname)) {
        ResponseMessage::sendError(client, ERR_NICKNAMEINUSE, nickname + " :Nickname is already in use");
        return;
    }
    
    client->setNickname(nickname);
    Logger::info("Client " + client->getIpAddr() + " is now known as " + nickname);
}

bool Nick::hasPermission(Client* client) {
    // NICK può essere usato solo dopo PASS (client autenticato)
    if (!client->isAuthenticated()) {
        ResponseMessage::sendError(client, ERR_NOTREGISTERED, ":You have not registered");
        return false;
    }
    return true;
}

bool Nick::isNicknameInUse(const std::string& nickname) {
    const std::map<int, Client*>& clients = _server->getClients();
    
    for (std::map<int, Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
        if (it->second->getNickname() == nickname) {
            return true;
        }
    }
    
    return false;
}

std::string Nick::getCmdName() const {return "NICK";}
