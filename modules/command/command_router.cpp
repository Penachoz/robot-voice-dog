#include "command_router.hpp"
#include <algorithm>

static std::string lower(std::string s){
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){return std::tolower(c);});
    return s;
}

std::optional<Command> parse_command_es(const std::string& t) {
    auto s = lower(t);
    if (s.find("avanza") != std::string::npos || s.find("adelante") != std::string::npos)
        return Command::FORWARD;
    if (s.find("alto") != std::string::npos || s.find("para") != std::string::npos)
        return Command::STOP;
    if (s.find("izquierda") != std::string::npos)
        return Command::LEFT;
    if (s.find("derecha") != std::string::npos)
        return Command::RIGHT;
    if (s.find("sentado") != std::string::npos || s.find("siéntate") != std::string::npos)
        return Command::SIT;
    if (s.find("parado") != std::string::npos || s.find("de pie") != std::string::npos)
        return Command::STAND;
    return std::nullopt;
}
