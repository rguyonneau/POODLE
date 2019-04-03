// Copyright 2012 CrossControl

#ifndef CANWRAPPER_H
#define CANWRAPPER_H

#include <QString>

#include <linux/can.h>
#include <linux/can/raw.h>

class CanWrapper
{
public:
    CanWrapper();

    bool Init(const char *interfaceName, int &errorCode); // function to initialize the can_socket
    // interfaceName: the name of the can interface (i.e. can0 for the raspberry pi)
    // errorCode - error code indicating why init did fail
    // return true if the socket has been opened, false otherwise

    void Close(); // function to close the can_socket

    bool SendMsg(struct can_frame msg, bool extended, bool rtr, int &errorCode); // function to send a can message
    // msg: the message to be sent
    // extended: extended can frame or not
    // rtr: remote trame request (or data otherwise)
    // return true is the message has been sent, false otherwise


    bool GetMsg(struct can_frame &frame, bool &extended, bool &rtr, bool &error, int &errorCode); // function to het a can message
    // frame: the received can frame
    // extended: is the received can frame an extended can frame or not
    // rtr: is the reveived can frame a request can frame or not
    // ??

    bool SetRecvBufferSize(int size);
    // ??

    int EnableErrorMessages();
    // ??

    bool isInitialized() { return m_initialized; } // getter for the initialized boolean attribute

private:
    bool m_initialized; // indicates if socket is initialized
    int m_socket;       // id to use the can Socket


};

#endif // CANWRAPPER_H
