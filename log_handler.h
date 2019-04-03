#ifndef LOG_HANDLER_H
#define LOG_HANDLER_H

#include <QObject>

#define LOG_INFO 1
#define LOG_WARN 2
#define LOG_ERR 3
#define LOG_CAN 4

class Log_handler :  public QObject{
    Q_OBJECT

public:
    Log_handler();

    void addLog(QString txt, int type=LOG_INFO);

 signals:
    void signal_addLog(QString txt, int type);

};

#endif // LOG_HANDLER_H
