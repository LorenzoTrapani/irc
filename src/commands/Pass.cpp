#include "commands/Pass.hpp"
#include "ResponseMessage.hpp"
#include "Server.hpp"
#include "Logger.hpp"

Pass::Pass(Server* server) : _server(server) {}

Pass::~Pass() {}

void Pass::execute(Client* client, const std::vector<std::string>& params) {
    if (params.empty() || params.size() != 1) {
        ResponseMessage::sendError(client, ERR_NEEDMOREPARAMS, "PASS :Wrong number of parameters");
        return;
    }
    
    std::string password = params[0];
    
    if (password == _server->getPassword()) {
        // pwd OK ma il client non è ancora completamente autenticato
        // L'autenticazione completa avviene dopo i comandi NICK e USER
        client->setAuthenticated(true);
        Logger::info("Client " + client->getIpAddr() + " has entered the correct password");
    } else {
        ResponseMessage::sendError(client, ERR_PASSWDMISMATCH, ":Password incorrect");
        Logger::warning("Client " + client->getIpAddr() + " has entered an incorrect password");
    }
}

bool Pass::hasPermission(Client* client) {
    // PASS deve essere il primo comando di autenticazione
    // Non può essere usato se il client è già autenticato
    if (client->isAuthenticated()) {
        ResponseMessage::sendError(client, ERR_ALREADYREGISTERED, ":You may not reregister");
        return false;
    }
    return true;
}

std::string Pass::getCmdName() const {return "PASS";}
