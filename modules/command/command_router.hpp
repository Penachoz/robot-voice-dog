#pragma once
#include <string>
#include <optional>

enum class Command { FORWARD, STOP, LEFT, RIGHT, SIT, STAND };

std::optional<Command> parse_command_es(const std::string& text);
