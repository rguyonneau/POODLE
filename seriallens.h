#ifndef SERIALLENS_H
#define SERIALLENS_H

#include <QString>

class SerialLens
{
public:
    SerialLens();

    int init(const char *portname, QString& msg);
    int cmd_start(QString &msg);
    int cmd_CommandMode(QString &msg);
    int cmd_changeFocale(double focale, QString &msg);

    int16_t getMaxFocale(){ return _maxfocale; }
    int16_t getMinFocale(){ return _minfocale; }


private:
    int set_interface_attribs (int fd, int speed, int parity, QString& msg);
    int set_blocking (int fd, int should_block, QString& msg);
    unsigned int computeCRC16_2(unsigned char *buf, int len);
    int _fd;
    bool _initialized;
    int16_t _maxfocale;
    int16_t _minfocale;
};

#endif // SERIALLENS_H
