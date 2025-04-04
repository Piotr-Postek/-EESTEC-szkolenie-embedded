#ifndef ICONS_H
#define ICONS_H

#include <LiquidCrystal.h>

// Customowe ikony
#define ICON_LOCKED_CHAR   (byte)0
#define ICON_UNLOCKED_CHAR (byte)1

// Standardowa ikona na LCD1602
#define ICON_RIGHT_ARROW   (byte)126

void init_icons(LiquidCrystal &lcd);

#endif /* ICONS_H */
