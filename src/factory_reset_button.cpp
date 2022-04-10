#include <factory_reset_button.hpp>

#include <iop-hal/thread.hpp>
#include <iop-hal/log.hpp>

static std::optional<iop_hal::io::Pin> factoryResetButton;
// Note: this probably should be AtomicBool
static std::shared_ptr<bool> factoryResetFlag;

static volatile iop::time::milliseconds resetStateTime = 0;
constexpr const uint32_t fifteenSeconds = 15000;

void IOP_RAM buttonChanged() noexcept {
  IOP_TRACE();

  if (!factoryResetButton || !factoryResetFlag) {
    if (IOP_LOG_LEVEL >= iop::LogLevel::CRIT) {
      iop::Log::print("[CRIT] RESET: CRITICAL FAILURE\n", iop::LogLevel::INFO, iop::LogType::STARTEND);
    }
    return;
  }

  if (iop_hal::gpio.digitalRead(*factoryResetButton) == iop_hal::io::Data::HIGH) {
    resetStateTime = iop_hal::thisThread.timeRunning() + fifteenSeconds;
    
    if (IOP_LOG_LEVEL <= iop::LogLevel::INFO) {
      iop::Log::print("[INFO] RESET: Press this button for 15 seconds more to factory reset the device\n", iop::LogLevel::INFO, iop::LogType::STARTEND);
    }
  } else if (resetStateTime < iop_hal::thisThread.timeRunning()) {
    *factoryResetFlag = true;

    if (IOP_LOG_LEVEL <= iop::LogLevel::INFO) {
      iop::Log::print("[INFO] RESET: Factory reset scheduled, it will run in the next loop run\n", iop::LogLevel::INFO, iop::LogType::STARTEND);
    }
  }
}

namespace reset {
auto setup(iop_hal::io::Pin button, std::shared_ptr<bool> flag) noexcept -> void {
  factoryResetButton = button;
  factoryResetFlag = flag;

  IOP_TRACE();
  iop_hal::gpio.setMode(button, iop_hal::io::Mode::INPUT);
  iop_hal::gpio.setInterruptCallback(button, iop_hal::io::InterruptState::CHANGE, buttonChanged);
}
}