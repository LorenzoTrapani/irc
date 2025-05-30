#ifndef PING_HPP
#define PING_HPP

#include "ICommand.hpp"

class Ping : public ICommand {
private:
    Server* _server;

public:
    Ping(Server* server);
    ~Ping();
    
    void execute(Client* client, const std::vector<std::string>& params);
    bool hasPermission(Client* client);
    std::string getCmdName() const;
};

#endif
