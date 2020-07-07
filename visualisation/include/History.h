#ifndef F9SIM_GRAPHICS_HISTORY_H_
#define F9SIM_GRAPHICS_HISTORY_H_


#include "../3rd_party/json.hpp"
#include "StateSnapshot.h"


namespace F9Sim {
namespace Graphics {


class History {
public:
    History() = default;
    ~History() = default;

    void loadWith(const nlohmann::json& data);
    StateSnapshot stateAt(double) const;
};


} // namespace Graphics
} // namespace F9Sim


#endif // #define F9SIM_GRAPHICS_HISTORY_H_

