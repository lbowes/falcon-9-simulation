#include "History.h"
#include "../3rd_party/json.hpp"


namespace F9Sim {
namespace Graphics {


History::History(const nlohmann::json& data) {
    // todo: load and store whatever information is necessary to make `stateAt` behave correctly
}


StateSnapshot History::stateAt(double time) const {
    StateSnapshot output;
    // todo
    return output;
}


} // namespace Graphics
} // namespace F9Sim
