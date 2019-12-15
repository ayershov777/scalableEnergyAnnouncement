#include <iostream>
#include <string> // to_string
#include <cmath> // round

using namespace std;

// I'm assuming all devices have can be turned on or off without removing the power supply.
struct Device {
  Device() : power(false) {}

  void handlePowerOn() {
    power = true;
  }

  void handlePowerOff() {
    power = false;
  }

  // returns the string to be shown or read when the power status is requested
  virtual string getPowerStatus() {
    return "device " + string(power ? "on" : "off");
  }

  bool power;
};

// A battery device may be "off" and unplugged but still function.
struct BatteryDevice : virtual public Device {
  bool charging;
  float charge;

  BatteryDevice() : charging(false), charge(1.0f) {}

  void handleUnplug() {
    charging = false;
  }

  void handlePlug() {
    charging = true;
  }

  void handleUpdateCharge(float increase) {
    charge += increase;
  }

  string getPowerStatus() override {
    string status = "battery life at " + to_string((int) round(charge * 100)) + "%";
    if(!power)
      status = "device off, " + status;
    if(charging)
      status += ", and charging";
    return status;
  }
};


class InterfaceDevice : virtual public Device {
 protected:
  virtual void announcePowerStatus() = 0;
};

class AudioDevice : virtual public InterfaceDevice {
 protected:
  virtual void vocalizePowerStatus() = 0;
};

class VideoDevice : virtual public InterfaceDevice {
 protected:
  virtual void renderPowerStatus() = 0;
};

/*
// remote devices
class NetworkDevice: virtual public InterfaceDevice {
  ...
};

// devices that have "off" or "very low power" state functions
class BackupBatteryDevice : virtual public BatteryDevice {
  ...
};
*/


// First Example
struct EchoSub: public AudioDevice {
  void announcePowerStatus() override {
    vocalizePowerStatus();
    cout << endl;
  }

  void vocalizePowerStatus() override {
    cout << "EchoSub vocalizing: " << getPowerStatus() << endl;
  }
};


// Second Example
struct FireTV: public AudioDevice, public VideoDevice {
  void announcePowerStatus() override {
    vocalizePowerStatus();
    renderPowerStatus();
    cout << endl;
  }

  void vocalizePowerStatus() override {
    cout << "FireTV vocalizing: " << getPowerStatus() << endl;
  }

  void renderPowerStatus() override {
    cout << "FireTV rendering: " << getPowerStatus() << endl;
  }
};


// Third Example
struct Kindle: public BatteryDevice, public AudioDevice, public VideoDevice {
  void announcePowerStatus() override {
    vocalizePowerStatus();
    renderPowerStatus();
    cout << endl;
  }

  void vocalizePowerStatus() override {
    cout << "Kindle vocalizing: " << BatteryDevice::getPowerStatus() << endl;
  }

  void renderPowerStatus() override {
    cout << "Kindle rendering: " << BatteryDevice::getPowerStatus() << endl;
  }
};

// Demos

// Echo Sub must be plugged in, and it only has audio.
// A power status request could reasonably only be made during a change of power status
void demoEchoSub() {
  cout << "--- Echo Dot ---" << endl;

  EchoSub demo;
  cout << "Echo Sub instantiated" << endl;
  demo.announcePowerStatus();
  // "device off"

  demo.handlePowerOn();
  cout << "Echo Sub turned on" << endl;
  demo.announcePowerStatus();
  // "device on"

  demo.handlePowerOff();
  cout << "Echo Sub turned off" << endl;
  demo.announcePowerStatus();
  // "device off"
}

// Fire TV must be plugged in, and it has both video and audio.
// A power status request could reasonably be made at any time during operation
void demoFireTV() {
  cout << "--- Fire TV---" << endl;

  FireTV demo;
  cout << "Fire TV instantiated" << endl;
  demo.announcePowerStatus();
  // "powering off"

  demo.handlePowerOn();
  cout << "Fire TV turned on" << endl;
  demo.announcePowerStatus();
  // "power on"

  demo.handlePowerOff();
  cout << "Fire TV turned off" << endl;
  demo.announcePowerStatus();
  // "powering off"
}

// Kindle must contain a battery to function and can be plugged in to charge.
// Kindle contains both audio and video
void demoKindle() {
  cout << "--- Kindle ---" << endl;

  Kindle demo;
  cout << "Kindle instantiated" << endl;
  demo.announcePowerStatus();
  // "Device off, Battery life 100%"

  demo.handlePlug();
  cout << "Kindle plugged in" << endl;
  demo.announcePowerStatus();
  // "Device off, Battery life 100% and charging"

  demo.handleUnplug();
  cout << "Kindle unplugged" << endl;
  demo.announcePowerStatus();
  // "Device off, Battery life 100%

  demo.handleUpdateCharge(-.01f);
  cout << "Kindle lost some charge" << endl;
  demo.announcePowerStatus();
  // "Device off, Battery life 99%

  demo.handlePowerOn();
  cout << "Kindle turned on" << endl;
  demo.announcePowerStatus();
  // "Battery life 99%"

  demo.handlePlug();
  cout << "Kindle plugged in" << endl;
  demo.announcePowerStatus();
  // "Battery life 99%, and charging"
}

int main() {
  cout << "DEMOS" << endl << endl;
  demoEchoSub();
  demoFireTV();
  demoKindle();
  return 0;
}
