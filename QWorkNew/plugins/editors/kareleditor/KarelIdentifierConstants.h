#ifndef KARELIDENTIFIERCONSTANTS_H
#define KARELIDENTIFIERCONSTANTS_H

#include "KarelToken.h"

namespace Karel {
namespace Constants {

const Identifier ID_NORMAL("NORMAL", 6);
const Identifier ID_ORIENT("ORIENT", 6);
const Identifier ID_APPROACH("APPROACH", 8);
const Identifier ID_LOCATION("LOCATION", 8);
const Identifier ID_CONFIG_DATA("CONFIG_DATA", 11);

const Identifier ID_CFG_TURN_NO1("CFG_TURN_NO1", 12);
const Identifier ID_CFG_TURN_NO2("CFG_TURN_NO2", 12);
const Identifier ID_CFG_TURN_NO3("CFG_TURN_NO3", 12);
const Identifier ID_CFG_FLIP("CFG_FLIP", 8);
const Identifier ID_CFG_LEFT("CFG_LEFT", 8);
const Identifier ID_CFG_UP("CFG_UP", 6);
const Identifier ID_CFG_FRONT("CFG_FRONT", 9);

const Identifier ID_X("X", 1);
const Identifier ID_Y("Y", 1);
const Identifier ID_Z("Z", 1);
const Identifier ID_W("W", 1);
const Identifier ID_P("P", 1);
const Identifier ID_R("R", 1);
const Identifier ID_EXT1("EXT1", 4);
const Identifier ID_EXT2("EXT2", 4);
const Identifier ID_EXT3("EXT3", 4);

}   // namespace Constants
}   // namespace Karel

#endif // KARELIDENTIFIERCONSTANTS_H
