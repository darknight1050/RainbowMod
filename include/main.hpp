#pragma once
#include <map>
#include <vector>

#include "../extern/beatsaber-hook/shared/utils/utils.h"

static struct Config_t {
    bool Lights = true;
    bool Walls = false;
    bool Sabers = false;
    bool Trails = false;
    bool Notes = false;
    bool QSabers = false;
    double SaberASpeed = 1.0;
    double SaberBSpeed = 1.0;
    double SabersStartDiff = 180.0;
    double LightASpeed = 2.0;
    double LightBSpeed = 2.0;
    double LightsStartDiff = 180.0;
    double WallsSpeed = 2.0;
} Config;

enum SaberType
{
	SaberA,
	SaberB
};

enum NoteType
{
	NoteA,
	NoteB,
	GhostNote,
	Bomb
};
