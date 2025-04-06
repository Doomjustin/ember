#ifndef EMBER_NET_CONFIGURATION_H
#define EMBER_NET_CONFIGURATION_H

#include "event_scheduler.h"

#include <memory>

namespace ember::net::config {

std::unique_ptr<EventScheduler> scheduler();

} // namespace ember::net::config

#endif // !EMBER_NET_CONFIGURATION_H