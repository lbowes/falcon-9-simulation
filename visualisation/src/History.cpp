#include "History.h"
#include "../3rd_party/json.hpp"


namespace F9Sim {
namespace Graphics {


History::History(const nlohmann::json& data) {
    // todo
}


StateSnapshot History::stateAt(double) const {
    StateSnapshot output;
    // todo
    return output;
}


} // namespace Graphics
} // namespace F9Sim
