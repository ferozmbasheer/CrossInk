#pragma once

#include "activities/Activity.h"

// Read-only screen shown from the Home menu that displays the owner contact
// line from SETTINGS.ownerContact, so a finder can return the device.
class LostDeviceActivity final : public Activity {
 public:
  explicit LostDeviceActivity(GfxRenderer& renderer, MappedInputManager& mappedInput)
      : Activity("LostDevice", renderer, mappedInput) {}

  void onEnter() override;
  void loop() override;
  void render(RenderLock&&) override;
};
