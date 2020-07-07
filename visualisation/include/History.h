#ifndef F9SIM_GRAPHICS_HISTORY_H_
#define F9SIM_GRAPHICS_HISTORY_H_
#pragma once


#include "../3rd_party/json.hpp"
#include "StateSnapshot.h"


namespace F9Sim {
namespace Graphics {


class History {
public:
    History(const nlohmann::json& data);
    ~History() = default;

    StateSnapshot stateAt(double time) const;
};


} // namespace Graphics
} // namespace F9Sim


#endif // #define F9SIM_GRAPHICS_HISTORY_H_

