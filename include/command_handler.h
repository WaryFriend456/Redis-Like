//
// Created by kushal bang on 16-03-2025.
//

#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <string>
#include <vector>
#include "database.h"

std::string handleCommand(const std::vector<std::string>& tokens, Database& db);

#endif //COMMAND_HANDLER_H
