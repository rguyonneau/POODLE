#include "seriallens.h"

#include <QDebug>

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

SerialLens::SerialLens()
{
    _initialized = false;
}

int SerialLens::set_interface_attribs (int fd, int speed, int parity, QString& msg){
    struct termios tty;
    memset (&tty, 0, sizeof tty);
    if (tcgetattr (fd, &tty) != 0)
    {
            msg = "error from tcgetattr";
            return -1;
    }

    cfsetospeed (&tty, speed);
    cfsetispeed (&tty, speed);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK;         // disable break processing
    tty.c_lflag = 0;                // no signaling chars, no echo,
                                    // no canonical processing
    tty.c_oflag = 0;                // no remapping, no delays
    tty.c_cc[VMIN]  = 0;            // read doesn't block
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                    // enable reading
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag |= parity;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr (fd, TCSANOW, &tty) != 0)
    {
            msg = "error from tcsetattr";
            return -1;
    }
    return 0;
}

int SerialLens::set_blocking (int fd, int should_block, QString& msg)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                msg = "error from tggetattr";
                return -1;
        }

        tty.c_cc[VMIN]  = should_block ? 1 : 0;
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        if (tcsetattr (fd, TCSANOW, &tty) != 0){
            msg = "error setting term attributes";
            return -1;
        }
        return 0;
}

int SerialLens::init(const char *portname, QString& msg){

    _fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (_fd < 0)
    {
        msg = "error opening";
        return -1;
    }

    if(set_interface_attribs(_fd, B115200, 0, msg)){ // set speed to 115,200 bps, 8n1 (no parity)
        return -1;
    }
    if(set_blocking(_fd, 0, msg)){                // set no blocking
        return -1;
    }
    _initialized = true;
    return 1;
}

unsigned int SerialLens::computeCRC16_2(unsigned char *buf, int len){
  unsigned int crc = 0xFFFF;
  for (int pos = 0; pos < len; pos++)
  {
  crc ^= (unsigned int)buf[pos];    // XOR byte into least sig. byte of crc

  for (int i = 8; i != 0; i--) {    // Loop over each bit
    if ((crc & 0x0001) != 0) {      // If the LSB is set
      crc >>= 1;                    // Shift right and XOR 0xA001
      crc ^= 0xA001;
    }
    else                            // Else LSB is not set
      crc >>= 1;                    // Just shift right
    }
  }

  return crc;
}

int SerialLens::cmd_start(QString &msg){
    if(!_initialized){
        msg = "The lens should be initialized first!";
        return -1;
    }

    unsigned char nbelem = 5;

    if(write (_fd, "Start", nbelem)!= nbelem){
        msg = "Error while writting on serial port";
        return -1;
    }

    usleep ((nbelem + 30) * 100);             // sleep enough to transmit the 5 plus
    char buf [100];
    int n = read (_fd, buf, sizeof buf);  // read up to 100 characters if ready to read
    buf[n] = '\0';

    if(n!=7){
        msg = QString("Not the expected answer size: ")+QString::number(n);
        return -1;
    }

    char expectedAnswer[] = "Ready\n\n"; // the documentation is not correct, it is not Ready\r\n but Ready\n\n that is sent

    for(int i=0; i<7; i++){
        if(buf[i] != expectedAnswer[i]){
            msg = QString("Not the expected answer: ")+QString::number(buf[i], 16)+" vs "+QString::number(expectedAnswer[i], 16);
            return -1;
        }
    }
    buf[5] = '\0'; // we remove the \r\n

    msg = buf;

    return 0;
}

int SerialLens::cmd_CommandMode(QString &msg){
    if(!_initialized){
        msg = "The lens should be initialized first!";
        return -1;
    }

    unsigned char nbelem = 6;

    unsigned char cmd[nbelem];
    cmd[0] = 'M';
    cmd[1] = 'w';
    cmd[2] = 'C';
    cmd[3] = 'A';
    cmd[4] = 0x56;
    cmd[5] = 0x76;


    if(write (_fd, cmd, nbelem)!= nbelem){
        msg = "Error while writting on serial port";
        return -1;
    }

    usleep ((nbelem + 30) * 100);             // sleep enough to transmit the 5 plus
    char buf [100];
    int n = read (_fd, buf, 100);  // read up to 100 characters if ready to read

    if(n!=12){
        msg = QString("Not the expected answer size (cmd_CommandMode): ")+QString::number(n);
        return -1;
    }

    msg="";
    for(int i=0; i<3; i++){
        msg += buf[i];
    }
    msg+= "\n\t Status byte:"+QString::number(buf[3]);

    //getting the maximale focale over the two bytes
    memcpy(&_maxfocale, &buf[5], 1);
    memcpy(((char*)&_maxfocale) +1, &buf[4], 1);
    msg+= "\n\t maxfocale:"+QString::number(_maxfocale);

    //getting the minimale focale over the two bytes
    memcpy(&_minfocale, &buf[7], 2);
    memcpy(((char*)&_minfocale) +1, &buf[6], 1);
    msg+= "\n\t minfocale:"+QString::number(_minfocale);

    return 0;
}

int SerialLens::cmd_changeFocale(double focale, QString &msg){
    // focale en mm
    if(!_initialized){
        msg = "The lens should be initialized first!";
        return -1;
    }

    double focale_power = 1/(focale/1000);
    int16_t focale_power2 = focale_power*200; // 200 arbitraire

    if(focale_power2 < _minfocale || focale_power2 > _maxfocale){
        msg = "The focale lenght is not in a correct range! it should be : \n\t";
        msg += QString::number(_minfocale) + " < ["+  QString::number(focale_power2);
        msg += "] < "+QString::number(_maxfocale);
        return -1;
    }

    unsigned char nbelem = 10;

    unsigned char cmd[nbelem];
    cmd[0] = 'P';
    cmd[1] = 'w';
    cmd[2] = 'D';
    cmd[3] = 'A';
    cmd[4] = focale_power2 >> 8;
    cmd[5] = focale_power2;
    cmd[6] = 0x00; // dummy byte
    cmd[7] = 0x00; // dummy byte;

    uint16_t crc = computeCRC16_2(cmd,6);

    cmd[8] = crc;
    cmd[9] = crc >> 8;


    if(write (_fd, cmd, nbelem)!= nbelem){
        msg = "Error while writting on serial port";
        return -1;
    }

    /*usleep ((nbelem + 30) * 100);             // sleep enough to transmit the 5 plus
    int buffermax = 1024;
    char buf [buffermax];
    int n = read (_fd, buf, buffermax-1);  // read up to 100 characters if ready to read

    if(n!=0){
        buf[n] = '\0';
        msg = QString("Error CRC? : ")+QString(buf);
        return -1;
    }*/

    msg = QString("focale should be changed to ")+QString::number(focale_power2);

    return 0;
}
