#include "LostDeviceActivity.h"

#include <GfxRenderer.h>
#include <I18n.h>

#include <string>

#include "CrossPointSettings.h"
#include "MappedInputManager.h"
#include "components/TouchHeaderBackButton.h"
#include "components/UITheme.h"
#include "fontIds.h"

namespace {
constexpr int kIntroMaxLines = 3;
constexpr int kContactMaxLines = 4;
}  // namespace

void LostDeviceActivity::onEnter() {
  Activity::onEnter();
  requestUpdate();
}

void LostDeviceActivity::loop() {
  if (TouchHeaderBackButton::wasTapped(mappedInput, renderer) ||
      mappedInput.wasPressed(MappedInputManager::Button::Back)) {
    finish();
  }
}

void LostDeviceActivity::render(RenderLock&&) {
  const auto& metrics = UITheme::getInstance().getMetrics();
  const int pageWidth = renderer.getScreenWidth();
  const int textWidth = pageWidth - metrics.contentSidePadding * 2;

  renderer.clearScreen();

  const Rect header = TouchHeaderBackButton::headerRect(renderer, mappedInput);
  if (mappedInput.hasTouchHardware()) {
    TouchHeaderBackButton::draw(renderer, header, tr(STR_LOST_DEVICE), false);
  } else {
    GUI.drawHeader(renderer, header, tr(STR_LOST_DEVICE));
  }

  const bool hasContact = SETTINGS.ownerContact[0] != '\0';
  const int introLineHeight = renderer.getLineHeight(UI_10_FONT_ID);
  const int contactLineHeight = renderer.getLineHeight(UI_12_FONT_ID);

  int y = header.y + header.height + metrics.verticalSpacing * 3;

  if (hasContact) {
    for (const auto& line : renderer.wrappedText(UI_10_FONT_ID, tr(STR_LOST_DEVICE_INTRO), textWidth, kIntroMaxLines)) {
      renderer.drawCenteredText(UI_10_FONT_ID, y, line.c_str());
      y += introLineHeight;
    }
    y += metrics.verticalSpacing * 2;
  }

  const char* contactText = hasContact ? SETTINGS.ownerContact : tr(STR_LOST_DEVICE_NOT_SET);
  const int contactFont = hasContact ? UI_12_FONT_ID : UI_10_FONT_ID;
  const int lineHeight = hasContact ? contactLineHeight : introLineHeight;
  const auto style = hasContact ? EpdFontFamily::BOLD : EpdFontFamily::REGULAR;
  for (const auto& line : renderer.wrappedText(contactFont, contactText, textWidth, kContactMaxLines, style)) {
    renderer.drawCenteredText(contactFont, y, line.c_str(), true, style);
    y += lineHeight;
  }

  const auto labels = mappedInput.mapLabels(mappedInput.withBackArrow(tr(STR_BACK)), "", "", "");
  GUI.drawButtonHints(renderer, labels.btn1, labels.btn2, labels.btn3, labels.btn4);
  renderer.displayBuffer();
}
