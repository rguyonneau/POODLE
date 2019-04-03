#include "motor_canopen_driver.h"

#include <QTime>
#include <QDebug>

#define WATCHDOG_MS 100

#define STATE_NA 0
#define STATE_NOTREADY 1
#define STATE_DISABLED 2
#define STATE_READY 3
#define STATE_SWITCHEDON 4
#define STATE_ENABLED 5
#define STATE_QUICKSTOP 6
#define STATE_FAULTACTIVE 7
#define STATE_FAULT 8

#define MODE_NA 0
#define MODE_NOMODE -1
#define MODE_PPOS 1
#define MODE_VEL 2
#define MODE_PVEL 3
#define MODE_TORQUE 4
#define MODE_HOMING 5
#define MODE_IPOS 7

Motor_CANOpen_Driver::Motor_CANOpen_Driver(Log_handler* logs){
    _can = new CanWrapper(); // to handle the can socket
    _logs = logs;
}

QString Motor_CANOpen_Driver::canFrame2QString(const struct can_frame& canframe){
    // function that convert a can frame to a QString for display purpose
    QString retval;
    retval += QString::number(canframe.can_id, 16);
    retval += " # ";
    for(int i=0; i<canframe.can_dlc; i++){
        retval += QString::number(canframe.data[i], 16);
        retval += " ";
    }
    return retval;
}

bool Motor_CANOpen_Driver::readRegister (uint16_t regadd, uint32_t nodeid, void* regval, size_t size, unsigned char subindex, bool verbose){
    //function to read a register of a controller
    // regadd : the register address
    // nodeid: the id of the controller node
    // regval: the value of the read register
    // size: the size of the register value
    // subindex: the subindex we want to read (default value = 0)
    // verbose: to add (true) or not (false) log messages (default value = true)

    // we flush the CAN buffer just in case
    struct can_frame msg_rcvd;
    int errorCode = 0;
    bool extended, rtr, error;
    while(_can->GetMsg(msg_rcvd, extended, rtr, error, errorCode)){
        if(verbose) _logs->addLog("Unexpected CAN frame - getRegister", LOG_WARN);
        if(verbose) _logs->addLog(canFrame2QString(msg_rcvd), LOG_CAN);
    }

    // initialization of the request
    struct can_frame msg_scan;
    msg_scan.can_id  = 0x600+nodeid;
    msg_scan.can_dlc = 8;
    msg_scan.data[0] = 0x40;
    msg_scan.data[1] = regadd;
    msg_scan.data[2] = (regadd >> 8);
    msg_scan.data[3] = subindex;
    msg_scan.data[4] = 0x00;
    msg_scan.data[5] = 0x00;
    msg_scan.data[6] = 0x00;
    msg_scan.data[7] = 0x00;

    // sending the request
    if(!_can->SendMsg(msg_scan, 0, 0, errorCode)){
        if(verbose) _logs->addLog("Failed to send the request - getRegister", LOG_ERR);
        return false;
    }
    if(verbose) _logs->addLog(canFrame2QString(msg_scan), LOG_CAN);

    QTime myTimer; // watchdog not the indefenetely blocked by the reading
    myTimer.start();
    int nMilliseconds = 0;

    // getting the response from the node
    while((!_can->GetMsg(msg_rcvd, extended, rtr, error, errorCode)) && (nMilliseconds < WATCHDOG_MS)){
        nMilliseconds = myTimer.elapsed();
    }// we want to get out of the loop if we received a can message or if we wait more than WATCHDOG_MS ms

    if(nMilliseconds >= WATCHDOG_MS){ // if we did not reveiced a can message before the watchdog
        if(verbose) _logs->addLog("Can response not received - getRegister", LOG_ERR);
        return false;
    }

    if(verbose) _logs->addLog(canFrame2QString(msg_rcvd), LOG_CAN);

    if(msg_rcvd.can_id != (nodeid + 0x580)){ // the response is tested
        // note: TODO: check if the answer is an error frame or not
        if(verbose) _logs->addLog("Unexpected response - getRegister", LOG_ERR);
        return false;
    }

    for(unsigned int i=0; i< size; i++){
        // save the gotten value in the regval address variable
        memcpy((unsigned char*)regval+i, &msg_rcvd.data[4+i], 1);
    }

    return true;

}

QString Motor_CANOpen_Driver::state2QString(unsigned int state){
    // function that convert a state number to a QString for display purpose
    QString retval;
    switch(state){
    case STATE_NOTREADY:
        retval = "Not ready to swith on";
        break;
    case STATE_DISABLED:
        retval = "Swith on disabled";
        break;
    case STATE_READY:
        retval = "Ready to switch on";
        break;
    case STATE_SWITCHEDON:
        retval = "Switched on";
        break;
    case STATE_ENABLED:
        retval = "Operation Enabled";
        break;
    case STATE_QUICKSTOP:
        retval = "Quick stop active";
        break;
    case STATE_FAULTACTIVE:
        retval = "Fault reaction active";
        break;
    case STATE_FAULT:
        retval = "Fault";
    default:
        retval = "Not a valide state";

    }
    return retval;
}

QString Motor_CANOpen_Driver::mode2QString(int8_t mode){
    // function that convert a mode number to a QString for display purpose
    QString retval;
    switch(mode){
    case MODE_NOMODE:
        retval = "No mode";
        break;
    case MODE_PPOS:
        retval = "Profile positon";
        break;
    case MODE_VEL:
        retval = "Velocity";
        break;
    case MODE_PVEL:
        retval = "Profile velocity";
        break;
    case MODE_TORQUE:
        retval = "Torque velocity";
        break;
    case MODE_HOMING:
        retval = "Homing";
        break;
    case MODE_IPOS:
        retval = "Interpolated position";
        break;
    default:
        retval = "Not a valide mode";
    }
    return retval;
}

unsigned int Motor_CANOpen_Driver::getStateFromStatusWord(uint16_t statusword){
    // processing the status word to display the status of the node
    unsigned int state = STATE_NA;
    if((statusword & 0x004F) == 0){ // xxxx xxxx x0xx 0000
        state = STATE_NOTREADY;
    }else if((statusword & 0x005F) == 0x50){ // xxxx xxxx x1x1 0000
        state = STATE_DISABLED;
    }else if((statusword & 0x007F) == 0x31){ // xxxx xxxx x011 0001
        state = STATE_READY;
    }else if((statusword & 0x007F) == 0x33){ // xxxx xxxx x011 0011
        state = STATE_SWITCHEDON;
    }else if((statusword & 0x007F) == 0x37){ // xxxx xxxx x011 0111
        state = STATE_ENABLED;
    }else if((statusword & 0x007F) == 0x17){ // xxxx xxxx x001 0111
        state = STATE_QUICKSTOP;
    }else if((statusword & 0x004F) == 0x0F){ // xxxx xxxx x001 0111
        state = STATE_FAULTACTIVE;
    }else if((statusword & 0x004F) == 0x08){ // xxxx xxxx x001 0111
        state = STATE_FAULT;
    }
    return state;
}

bool Motor_CANOpen_Driver::setRegister (uint16_t regadd, uint32_t nodeid, const void* regval, size_t size, unsigned char subindex, bool verbose){
    // function to set the value of a register in the controller
    // regadd: the register address
    // nodeid: the id of the controller
    // regval: the value we want to write
    // size: the size of the value (byte number)
    // subindex: the subindex we want to write (default value: 0)
    // verbose: to add (true) or not (false) the messages to the listwidget log (fault value:true)

    // we flush the CAN buffer just in case
    struct can_frame msg_rcvd;
    int errorCode = 0;
    bool extended, rtr, error;
    while(_can->GetMsg(msg_rcvd, extended, rtr, error, errorCode)){
        if(verbose) _logs->addLog("Unexpected CAN frame - setRegister", LOG_WARN);
        if(verbose) _logs->addLog(canFrame2QString(msg_rcvd), LOG_CAN);
    }


    struct can_frame msg_scan;

    // initialization of the request
    msg_scan.can_id  = 0x600+nodeid;
    msg_scan.can_dlc = 8;

    // the first byte change depending of the size to write
    msg_scan.data[0] = 0x2 << 4;
    msg_scan.data[0] += 0x3;
    msg_scan.data[0] += (4-size) << 2;

    // setting the register address
    msg_scan.data[1] = regadd;
    msg_scan.data[2] = (regadd >> 8);

    // subindex
    msg_scan.data[3] = subindex;

    //setting the register value
    for(unsigned int i=0; i<size; i++){
        memcpy(&msg_scan.data[4+i], (unsigned char*)regval+i, 1);
    }
    for(unsigned int i=size; i<4; i++){
       msg_scan.data[4+i]=0;
    }

    // sending the request
    if(!_can->SendMsg(msg_scan, 0, 0, errorCode)){
        if(verbose) _logs->addLog("Failed to send the request - setRegister", LOG_ERR);
        return false;
    }
    if(verbose) _logs->addLog(canFrame2QString(msg_scan), LOG_CAN);

    QTime mytime; // to handle the watchdog
    int nbms = 0;

    mytime.start();
    while((!_can->GetMsg(msg_rcvd, extended, rtr, error, errorCode)) && nbms < WATCHDOG_MS){
        nbms = mytime.elapsed();
    }// we want to end the loop if we received a can message or if the watchdog time out

    if(nbms >= WATCHDOG_MS){ // if we did not received a can message before the watchog timed out
        if(verbose) _logs->addLog("Can response not received - setRegister", LOG_ERR);
        return false;
    }
    if(verbose) _logs->addLog(canFrame2QString(msg_rcvd), LOG_CAN);

    if(msg_rcvd.can_id != (nodeid + 0x580)){ // the response is tested
        if(verbose) _logs->addLog("Unexpected response - setRegister", LOG_ERR);
        return false;
    }else{
        // we test if it is an error can frame
        if (msg_rcvd.data[0]==0x80){
            if(verbose) _logs->addLog("Error frame received - setRegister", LOG_ERR);
            return false;
        }
    }
    return true;
}

bool Motor_CANOpen_Driver::addStates2logs(){
    // function to update the labels according to the controller's status word (controller mode and state)
    _logs->addLog("Updating states...");
    uint16_t statusword;
    if(!readRegister(REG_STATUSWORD, ID_MIRROR_1, &statusword, sizeof(statusword))){
        _logs->addLog(QString("Fail to read the control word of mirror ")+QString::number(ID_MIRROR_1), LOG_ERR);
        return false;
    }
    _logs->addLog(QString("State of mirror ")+QString::number(ID_MIRROR_1) + QString(" : ") +
                  state2QString(getStateFromStatusWord(statusword)), LOG_INFO);

    if(!readRegister(REG_STATUSWORD, ID_MIRROR_2, &statusword, sizeof(statusword))){
        _logs->addLog(QString("Fail to read the control word of mirror ")+QString::number(ID_MIRROR_2), LOG_ERR);
        return false;
    }
    _logs->addLog(QString("State of mirror ")+QString::number(ID_MIRROR_2) + QString(" : ") +
                  state2QString(getStateFromStatusWord(statusword)), LOG_INFO);

    int8_t mode;
    if(!readRegister(REG_OPMODE, ID_MIRROR_1, &mode, sizeof(mode))){
        _logs->addLog(QString("Fail to read the Operation mode register of mirror")+QString::number(ID_MIRROR_1), LOG_ERR);
        return false;
    }

    _logs->addLog(QString("Mode of mirror ")+QString::number(ID_MIRROR_1) + QString(" : ") +
                  mode2QString(mode), LOG_INFO);

    if(!readRegister(REG_OPMODE, ID_MIRROR_2, &mode, sizeof(mode))){
        _logs->addLog(QString("Fail to read the Operation mode register of mirror")+QString::number(ID_MIRROR_2), LOG_ERR);
        return false;
    }
    _logs->addLog(QString("Mode of mirror ")+QString::number(ID_MIRROR_2) + QString(" : ") +
                  mode2QString(mode), LOG_INFO);

    return true;
}

bool Motor_CANOpen_Driver::connect(){
    // function to connect the can socket and check if the controllers are up
    int errorCode;
    // Connect the CAN socket
    if(!_can->isInitialized()){
        if(_can->Init("can0",errorCode)){
            _logs->addLog("Can initialized");
        }else{
            _logs->addLog("Failed to initialized CanBus",LOG_ERR);
            return false;
        }
    }else{
        _logs->addLog("CANbus already initialized",LOG_WARN);
    }

    // can frame to list the connected can nodes
    struct can_frame msg_scan;
    msg_scan.can_id  = 0x000;
    msg_scan.can_dlc = 2;
    msg_scan.data[0] = 0x81;
    msg_scan.data[1] = 0x00;

    if(_can->SendMsg(msg_scan, 0, 0, errorCode)){
        _logs->addLog(canFrame2QString(msg_scan), LOG_CAN);
    }else{
        _logs->addLog("Failed to send the request", LOG_ERR);
        return false;
    }

    bool m1found = false;
    bool m2found = false;

    struct can_frame msg_rcvd;
    bool extended, rtr, error;
    bool brcvd;
    QTime mytimer; // to handle the watch dog
    mytimer.start();
    int nbms = 0;
    do{
        mytimer.restart(); // restart the watch after each can message received
        do{
            brcvd = _can->GetMsg(msg_rcvd, extended, rtr, error, errorCode);
            nbms = mytimer.elapsed();
        }while((!brcvd) && (nbms < WATCHDOG_MS)); // we loop until we get a can message or the watchdog is over

        if(brcvd){
            // if we received a CAN message
            _logs->addLog(canFrame2QString(msg_rcvd), LOG_CAN);
            if((msg_rcvd.can_id) - 0x700 == ID_MIRROR_1){ // we check if it is a controller that indicated it is up
                m1found = true;
            }else if((msg_rcvd.can_id) - 0x700 == ID_MIRROR_2){
                m2found = true;
            }else{
                _logs->addLog("unexpexted can node!", LOG_ERR);
                return false;
            }
        }
    }while(brcvd);

    if(!m1found){
        _logs->addLog(QString("Mirror ")+ QString::number(ID_MIRROR_1)+" has not been found", LOG_ERR);
        return false;
    }
    if(!m2found){
        _logs->addLog(QString("Mirror ")+ QString::number(ID_MIRROR_2)+" has not been found", LOG_ERR);
        return false;
    }

    _logs->addLog("Connexion OK");

    return addStates2logs();
}

bool Motor_CANOpen_Driver::configureNode(unsigned char nodeid){
    //function to configure a node into the state ENABLE and the mode Profile Position
    bool end = false;
    int state;
    uint16_t controlword = 0;

    // configure the state
    while(!end){
        //get the state:
        uint16_t statusword;
        if(!readRegister(REG_STATUSWORD, nodeid, &statusword, sizeof(statusword))){
            _logs->addLog("Fail to read the control word", LOG_ERR);
            return false;
        }
        state = getStateFromStatusWord(statusword);

        switch (state) { // according to the current state we modify the control word to change it if needed
        case STATE_DISABLED: // STOP to go to state READY
            controlword = ((controlword & 0xFF7E) | 0x06); // 0xxx x110
            break;
        case STATE_READY: // SWITCH ON to go to state SWITCH ON
            controlword = ((controlword & 0xFF7F) | 0x07); // 0xxx x111
            break;
        case STATE_SWITCHEDON: // ENABLE OPERATION to go to state ENABLE
            controlword = ((controlword & 0xFF7F) | 0x0F); // 0xxx 1111
            break;
        case STATE_ENABLED:
            end = true;
            break;
        default:
            _logs->addLog("Error regarding the state", LOG_ERR);
            return false;
        }
        if(!end){
            controlword = controlword | 0x0100; // set bit 8 to 1, to pause it
            // we update the controlword in the controller
            if(!setRegister(REG_CTRLWORD, nodeid, &controlword, sizeof(controlword))){
                _logs->addLog("Fail to set the control word! - configureNode", LOG_ERR);
                return false;
            }
        }
    }

    //configure the mode
    int8_t mode = MODE_PPOS;
    if(!setRegister(REG_OPMODE, nodeid, &mode, sizeof(mode))){
        _logs->addLog("Fail to set the operation mode register", LOG_ERR);
        return false;
    }

    return true;
}

void Motor_CANOpen_Driver::configureMirrors(){
    // slot connected to the button configure,
    // function that calls configureNode() for both controller

    QString txt = "Error configuring the node ";
    if(!configureNode(ID_MIRROR_1)){
        _logs->addLog(txt+QString::number(ID_MIRROR_1), LOG_ERR);
        return;
    }
    _logs->addLog("Mirror "+ QString::number(ID_MIRROR_1) + " configured");

    if(!configureNode(ID_MIRROR_2)){
        _logs->addLog(txt+QString::number(ID_MIRROR_2), LOG_ERR);
        return;
    }
    _logs->addLog("Mirror "+ QString::number(ID_MIRROR_2) + " configured");

    // update the User Interface
    addStates2logs();
}

bool Motor_CANOpen_Driver::isarrived(uint32_t nodeid){
    // function to test if the motor has reached its position according to the target position
    // nodeid: the id of the controller to the corresponding motor

    // get the state
    uint16_t statusword;
    if(!readRegister(REG_STATUSWORD, nodeid, &statusword, sizeof(statusword),0,false)){
        _logs->addLog("Fail to read the control word", LOG_ERR);
        return false;
    }

    if((statusword & 0x0400) != 0){
        // the motor has reached its position
        return true;
    }else return false; // the motor has not reached its positon yet
}

bool Motor_CANOpen_Driver::go2position_angle(int phi1, int phi2){
    // we get the current controlword, in order not to have to ask for them each time we update a position
    uint16_t controlword1;
    if(!readRegister(REG_CTRLWORD, ID_MIRROR_1, &controlword1, sizeof(controlword1)),false){
        _logs->addLog(QString("Fail to read the control word of mirror ")+QString::number(ID_MIRROR_1), LOG_ERR);
        return false;
    }
    uint16_t controlword2;
    if(!readRegister(REG_CTRLWORD, ID_MIRROR_2, &controlword2, sizeof(controlword2)),false){
        _logs->addLog(QString("Fail to read the control word of mirror ")+QString::number(ID_MIRROR_2), LOG_ERR);
        return false;
    }

    if(!setPosition(ID_MIRROR_1, OFFCET_MIRROR_1 + phi1, controlword1)){
        _logs->addLog(QString("Fail to set position of mirror ")+QString::number(ID_MIRROR_1), LOG_ERR);
        return false;
    }
    if(!setPosition(ID_MIRROR_2, OFFCET_MIRROR_2 + phi2, controlword2)){
        _logs->addLog(QString("Fail to set position of mirror ")+QString::number(ID_MIRROR_1), LOG_ERR);
        return false;
    }
    _logs->addLog(QString("Waiting for the mirrors to arrived"), LOG_INFO);

    while(!isarrived(ID_MIRROR_1) || !isarrived(ID_MIRROR_2));

    return true;
}


bool Motor_CANOpen_Driver::setPosition(uint32_t nodeid, int32_t tpos, uint16_t controlword){
    //function to set the motor into the desired position
    // nodeid: the id of the corresponding controller
    // tpos: the target position
    // controlword: the current controlword (avoid to request the controlword again and again...)

    // we set the target position
    if(!setRegister(REG_PPOS_TPOS, nodeid, &tpos, sizeof(tpos),0,false)){
        _logs->addLog("Fail to set the target position", LOG_ERR);
        return false;
    }

    // bit value for the controlword (starting at 0):
    //  4 : new set point (0 not applied, 1 applied)
    //  5 : change set immediatly (0 terminate, 1 interrupt)
    //  6 : aboslute/relative (0 absolute, 1 relative)
    //  8 : stop (0 run, 1 stop)

    controlword = controlword & 0xFFEF; // set bit 4 to 0
    controlword = controlword & 0xFFDF; // set bit 5 to 0 // controlword | 0x0020; // set bit 5 to 1
    controlword = controlword & 0xFFBF; // set bit 6 to 0
    controlword = controlword & 0xFEFF; // set bit 8 to 0

    // we update the control word
    if(!setRegister(REG_CTRLWORD, nodeid, &controlword, sizeof(controlword),0,false)){
        _logs->addLog("Fail to set the control word!", LOG_ERR);
        return false;
    }

    controlword = controlword | 0x0010; // set bit 4 to 1
    // reset the control word for the next command
    if(!setRegister(REG_CTRLWORD, nodeid, &controlword, sizeof(controlword), 0, false)){
        _logs->addLog("Fail to set the control word!", LOG_ERR);
        return false;
    }
    return true;
}
