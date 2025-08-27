#ifndef DISPLAYCONSTS_H
#define DISPLAYCONSTS_H

// Main Window/Rendering
// Defined Constants
inline constexpr int TRUE_DISPLAY_MAX_X = 1920;
inline constexpr int TRUE_DISPLAY_MAX_Y = 1080;
inline constexpr int DISPLAY_MAX_X = 1920;
inline constexpr int DISPLAY_MAX_Y = 1080;
inline constexpr float DISPLAY_MAX_X_RATIO = static_cast<float>(DISPLAY_MAX_X) / TRUE_DISPLAY_MAX_X;
inline constexpr float DISPLAY_MAX_Y_RATIO = static_cast<float>(DISPLAY_MAX_Y) / TRUE_DISPLAY_MAX_Y;
inline constexpr int MAX_FRAMERATE = 60;

// Tower Selection "Menu"
// Defined Constants
inline constexpr int ICON_LEFT_PADDING = 180 * DISPLAY_MAX_X_RATIO;
inline constexpr int ICON_TOP_PADDING = 100 * DISPLAY_MAX_Y_RATIO;
inline constexpr int ICON_GAP = 20 * (DISPLAY_MAX_X_RATIO + DISPLAY_MAX_Y_RATIO) / 2;

// Placement Constants
inline constexpr float ICON_START_X = DISPLAY_MAX_X - ICON_LEFT_PADDING;
inline constexpr float ICON_START_Y = 0 + ICON_TOP_PADDING;
inline constexpr float SEL_MENU_START_X = ICON_START_X - 50.f * 1.8 * DISPLAY_MAX_X_RATIO - ICON_GAP;

// Text
// Defined Constants
inline constexpr int TEXT_TOP_PADDING           = 0;
inline constexpr float FPS_PERCENT_RIGHT        = 0;
inline constexpr float LIFE_PERCENT_RIGHT       = 1.f / 15.f;
inline constexpr float PRESSURE_PERCENT_RIGHT   = 4.f / 10;
inline constexpr float BALANCE_PERCENT_RIGHT    = 2.f / 10.f;
inline constexpr float WAVE_PERCENT_RIGHT       = 7.8 / 10.f;
inline constexpr float STAT_PERCENT_RIGHT       = 8.6 / 10.f;
inline constexpr float PLAY_PERCENT_RIGHT       = 9 / 10.f;


// Placement Constants
inline constexpr float FPS_START_RIGHT      = DISPLAY_MAX_X * FPS_PERCENT_RIGHT;
inline constexpr float LIFE_START_RIGHT     = DISPLAY_MAX_X * LIFE_PERCENT_RIGHT;
inline constexpr float PRESSURE_START_RIGHT = DISPLAY_MAX_X * PRESSURE_PERCENT_RIGHT;
inline constexpr float BALANCE_START_RIGHT  = DISPLAY_MAX_X * BALANCE_PERCENT_RIGHT;
inline constexpr float WAVE_START_RIGHT     = DISPLAY_MAX_X * WAVE_PERCENT_RIGHT;
inline constexpr float STAT_START_RIGHT     = DISPLAY_MAX_X * STAT_PERCENT_RIGHT;
inline constexpr float PLAY_START_RIGHT     = DISPLAY_MAX_X * PLAY_PERCENT_RIGHT;
inline constexpr float TEXT_START_Y         = 0 + TEXT_TOP_PADDING;
inline constexpr float PRESSURE_GAP_Y       = 25 * DISPLAY_MAX_Y_RATIO;
inline constexpr float STAT_GAP_Y           = 30 * DISPLAY_MAX_Y_RATIO;

#endif //DISPLAYCONSTS_H
