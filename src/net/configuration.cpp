#include "configuration.h"
#include "defination.h"
#include "event/event_scheduler.h"

namespace ember::net::config {

std::unique_ptr<EventScheduler> scheduler()
{
    return scheduler(SchedulerType::Poll);
}

} // namespace ember::net::config