#ifndef MOTOR_CANOPEN_DRIVER_H
#define MOTOR_CANOPEN_DRIVER_H

#include "canwrapper.h"
#include <QString>
#include <QObject>
#include "log_handler.h"

#define ID_MIRROR_1 3
#define ID_MIRROR_2 4

#define OFFCET_MIRROR_1 265750
#define OFFCET_MIRROR_2 441000

#define REG_STATUSWORD 0x6041
#define REG_CTRLWORD 0x6040
#define REG_DISPOPMODE 0x6061
#define REG_OPMODE 0x6060

#define REG_PVEL_MAXPVEL 0x607F
#define REG_PVEL_TVEL 0x60FF
#define REG_PVEL_PACC 0x6083
#define REG_PVEL_PDEC 0x6084

#define REG_PPOS_TPOS 0x607A
#define REG_PPOS_ACPO1 0x6063
#define REG_PPOS_ACPO2 0x6064
#define REG_PPOS_PVEL 0x6081


class Motor_CANOpen_Driver
{

public:
    Motor_CANOpen_Driver(Log_handler* logs);
    bool readRegister (uint16_t regadd, uint32_t nodeid, void* regval, size_t size, unsigned char subindex=0, bool verbose=true);
    bool setRegister (uint16_t regadd, uint32_t nodeid, const void* regval, size_t size, unsigned char subindex=0, bool verbose=true);

    bool isarrived(uint32_t nodeid);
    bool go2position_angle(int phi1, int phi2);
    bool setPosition(uint32_t nodeid, int32_t tpos, uint16_t controlword);

    QString canFrame2QString(const struct can_frame& canframe);
    QString state2QString(unsigned int state);
    QString mode2QString(int8_t mode);
    unsigned int getStateFromStatusWord(uint16_t statusword);

    bool addStates2logs();

    bool connect();
    bool configureNode(unsigned char nodeid);
    void configureMirrors();


private:
    CanWrapper *_can;

    Log_handler* _logs;


};

#endif // MOTOR_CANOPEN_DRIVER_H
