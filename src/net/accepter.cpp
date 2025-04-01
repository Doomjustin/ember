#include "accepter.h"
#include "posix.h"

#include <gsl/gsl>

namespace ember::net::tcp {

Accepter::Accepter(int socket)
  : socket_{ socket }
{}

void Accepter::listen(int backlog)
{
    Expects(is_valid());
    posix::listen(id(), backlog);
}

} // namespace ember::net::tcp