#ifndef IR24BUTTONREMOTE
#define IR24BUTTONREMOTE

#include <ArduinoLog.h>

const int IR_COMMAND_ON = 0;
const int IR_COMMAND_OFF = 1;
const int IR_COMMAND_BRIUP = 2;
const int IR_COMMAND_BRIDOWN = 3;
const int IR_COMMAND_R0 = 4;
const int IR_COMMAND_G0 = 5;
const int IR_COMMAND_B0 = 6;
const int IR_COMMAND_W0 = 7;
const int IR_COMMAND_R1 = 8;
const int IR_COMMAND_G1 = 9;
const int IR_COMMAND_B1 = 10;
const int IR_COMMAND_R2 = 11;
const int IR_COMMAND_G2 = 12;
const int IR_COMMAND_B2 = 13;
const int IR_COMMAND_R3 = 14;
const int IR_COMMAND_G3 = 15;
const int IR_COMMAND_B3 = 16;
const int IR_COMMAND_R4 = 17;
const int IR_COMMAND_G4 = 18;
const int IR_COMMAND_B4 = 19;
const int IR_COMMAND_FLASH = 20;
const int IR_COMMAND_STROBE = 21;
const int IR_COMMAND_FADE = 22;
const int IR_COMMAND_SMOOTH = 23;
const int IR_COMMAND_UNKNOWN = -1;

unsigned long _hash(const char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

int IR24_Parse(String hex)
{

    long hash = _hash(hex.c_str());
    int command = IR_COMMAND_UNKNOWN;
    switch (hash)
    {
    case -1355746066: //"F7C03F"
        command = IR_COMMAND_ON;
        break;
    case -1356284626: //"F740BF"
        command = IR_COMMAND_OFF;
        break;
    case -1356141241: //"F740BF"
        command = IR_COMMAND_BRIDOWN;
        break;
    case -1356428242: //"F740BF"
        command = IR_COMMAND_BRIUP;
        break;
    case -1356356434: //"F740BF"
        command = IR_COMMAND_R0;
        break;
    case -1355817874: //"F740BF"
        command = IR_COMMAND_G0;
        break;
    case -1356213049: //"F740BF"
        command = IR_COMMAND_B0;
        break;
    case -1355674258: //"F740BF"
        command = IR_COMMAND_W0;
        break;
    case -1356392338: //"F740BF"
        command = IR_COMMAND_R1;
        break;
    case -1356105337: //"F740BF"
        command = IR_COMMAND_G1;
        break;
    case -1356248722: //"F740BF"
        command = IR_COMMAND_B1;
        break;
    case -1355710162: //"F740BF"
        command = IR_COMMAND_FLASH;
        break;
    case -1356320530: //"F740BF"
        command = IR_COMMAND_R2;
        break;
    case -1503317154: //"F740BF"
        command = IR_COMMAND_G2;
        break;
    case -1356177145: //"F740BF"
        command = IR_COMMAND_B2;
        break;
    case -1355638354: //"F740BF"
        command = IR_COMMAND_STROBE;
        break;
    case -1356419545: //"F740BF"
        command = IR_COMMAND_R3;
        break;
    case -1356132544: //"F740BF"
        command = IR_COMMAND_G3;
        break;
    case -1356275929: //"F740BF"
        command = IR_COMMAND_B3;
        break;
    case -1355737369: //"F740BF"
        command = IR_COMMAND_FADE;
        break;
    case -1356347737: //"F740BF"
        command = IR_COMMAND_R4;
        break;
    case -1355809177: //"F740BF"
        command = IR_COMMAND_G4;
        break;
    case -1356204352: //"F740BF"
        command = IR_COMMAND_B4;
        break;
    case -1355665561: //"F740BF"
        command = IR_COMMAND_SMOOTH;
        break;
    default:
        Log.trace("IR24BUTTONREMOTE : IR HEX not found:hex=" + hex + ":hash=" + hash);
        break;
    }
    return command;
}


#endif
