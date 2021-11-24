#include "./ui_mainwindow.h"
#include "mainwindow.h"

// arming
void MainWindow::arm() {
  console_log("Arming...");
  const Action::Result arm_result = action->arm();
  if (arm_result != Action::Result::Success) {
    console_log("Arming failed: ");
  }
};

// disarming
void MainWindow::disarm() {
  console_log("Disarming..");
  const Action::Result arm_result = action->disarm();
};

// takeoff
void MainWindow::takeoff() {
  const Action::Result takeoff_result = action->takeoff();
  if (takeoff_result != Action::Result::Success) {
    console_log("Takeoff failed");
  }
};

// landing
void MainWindow::land() {
  console_log("Landing...");

  const Action::Result land_result = action->land();
  if (land_result != Action::Result::Success) {
    console_log("Land failed");
  }
};

// enable offboard mode
void MainWindow::offboard_enable() {
  // check if offboard already enabled
  if (!offb_enabled) {

    // Send it once before starting offboard, otherwise it will be rejected.
    const Offboard::VelocityNedYaw stay{};
    // Drone stays in place waiting for commands
    offboard->set_velocity_ned(stay);

    Offboard::Result offboard_result = offboard->start();
    if (offboard_result != Offboard::Result::Success) {
      console_log("Offboard start failed");
    } else {
      console_log("Offboard started");

      // Set flag to indicate offboard mode is activated
      offb_enabled = true;

      // Start offboard thread
      // Start fastdds thread
      fastdds_obj = std::make_unique<fastdds_thread>(dp.get(), offboard.get(),
                                                     telemetry.get());
      fastdds_obj->start();

      // // If simulation, start position publisher
      // pos_pub_thread = std::make_unique<PositionPublisher>(
      //     dp.get(), std::move(std::make_unique<Telemetry>(system)));
      // // pos_pub_thread->start();
    }
  } else {
    console_log("Offboard is already enabled");
  }
};

// disable offboard mode
void MainWindow::offboard_disable() {
  // check if offboard is enabled before stopping
  if (offb_enabled) { // Set flag to indicate offboard mode is activated

    // Stop offboard thread
    fastdds_obj->quit();
    fastdds_obj->requestInterruption();
    fastdds_obj->wait();

    Offboard::Result offboard_result = offboard->stop();
    if (offboard_result != Offboard::Result::Success) {
      console_log("Offboard stop failed");
    } else {
      console_log("Offboard stopped");
      // reset offboard enabled status
      offb_enabled = false;
    }

  } else {
    console_log("Offboard is already disabled");
  }
};

void MainWindow::connect() {
  // check if already connected
  if (!connected) {
    // Create mavsdk object
    mavsdk = std::make_unique<Mavsdk>();

    QString mavsdk_port = ui->port_selector->currentText();
    ConnectionResult connection_result =
        mavsdk->add_any_connection(px4_port[mavsdk_port].toStdString());

    // Error checking
    if (connection_result != ConnectionResult::Success) {
      console_log("Connection failed:");
    }

    else {

      // Get pointer to system from mavsdk obj
      system = get_system(*mavsdk);

      // set intitialized status
      connected = true;

      // Error checking
      if (!system) {
        console_log("Couldn't get system");
      }

      else {
        // Create telemetry object
        telemetry = std::make_unique<Telemetry>(system);
        action = std::make_unique<Action>(system);
        offboard = std::make_unique<Offboard>(system);
      }
    }
  }

  else {
    console_log("Already Initialized");
  }
}

void MainWindow::setup_console_logging() {
  // Custom logger that logs to app console
  mavsdk::log::subscribe(
      [this](mavsdk::log::Level level,   // message severity level
             const std::string &message, // message text
             const std::string
                 &file,  // source file from which the message was sent
             int line) { // line number in the source file
        // process the log message in a way you like
        console_log(message);

        // returning true from the callback disables printing the message to
        // stdout
        return level < mavsdk::log::Level::Warn;
      });
}

std::shared_ptr<System> MainWindow::get_system(Mavsdk &mavsdk) {
  console_log("Waiting to discover system...");
  auto prom = std::promise<std::shared_ptr<System>>{};
  auto fut = prom.get_future();

  // We wait for new systems to be discovered, once we find one that has an
  // autopilot, we decide to use it.
  mavsdk.subscribe_on_new_system([this, &mavsdk, &prom]() {
    auto system = mavsdk.systems().back();

    if (system->has_autopilot()) {
      console_log("Discovered autopilot");

      // Unsubscribe again as we only want to find one system.
      mavsdk.subscribe_on_new_system(nullptr);
      prom.set_value(system);
    }
  });

  // We usually receive heartbeats at 1Hz, therefore we should find a
  // system after around 3 seconds max, surely.
  if (fut.wait_for(seconds(3)) == std::future_status::timeout) {
    console_log("No autopilot found.");
    return {};
  }

  // Get discovered system now.
  return fut.get();
}
