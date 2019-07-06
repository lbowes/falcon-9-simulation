#ifndef ISERIALISABLE_HPP
#define ISERIALISABLE_HPP
#pragma once

#include "../3rd_Party/json.hpp"

class ISerialisable {
public:
    ISerialisable() = default;
    virtual ~ISerialisable() = default;

    virtual void save(nlohmann::json& dest) const = 0;
    virtual void load(const std::string& source) = 0;

};

#endif // SERIALISABLE_HPP
