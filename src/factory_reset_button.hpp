#ifndef IOP_FACTORY_RESET_HPP
#define IOP_FACTORY_RESET_HPP

#include <iop-hal/io.hpp>

#include <memory>

namespace reset {
  /// Sets interrupt to handle factory reset (pressing a button for 15 seconds)
  ///
  /// Factory resets deletes both the wifi credentials and the monitor server token
  auto setup(iop_hal::io::Pin button, std::shared_ptr<bool> flag) noexcept -> void;
}

#endif