#ifndef MASTER_EVENT_RECEIVER_H_
#define MASTER_EVENT_RECEIVER_H_

#include "HWEventReceiver.h"

#include <IEventReceiver.h>
#include <irrArray.h>

namespace Input {

    class MasterEventReceiver : public irr::IEventReceiver {
    private:
        irr::core::array<irr::IEventReceiver*> mReceivers;

    public:
        MasterEventReceiver() = default;
        ~MasterEventReceiver() = default;

        void addReceiver(irr::IEventReceiver* receiver);
        void removeReceiver(irr::IEventReceiver* receiver);

        virtual bool OnEvent(const irr::SEvent& e);
    };

} // namespace Input

#endif // MASTER_EVENT_RECEIVER_H_

