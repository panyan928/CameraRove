#pragma once
#include "TMUtil/tmtools.h"
void drawAirplaneIcon(Vec2d  posMec, Vec2d center, float aircraftHeading, float viewportWidth, float viewportHeight);
//void drawAirportIcon(double lon, double lat, const char* iconPath, Vec2d center, double viewportWidth, double viewportHeight, double iconSize);
void drawAirportIcon(double lon, double lat, const char* iconPath, Vec2d center, float viewportWidth, float viewportHeight, double iconSize, double aircraftHeading, double zoom);
