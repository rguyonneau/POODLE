#include "log_handler.h"

Log_handler::Log_handler()
{

}


void Log_handler::addLog(QString txt, int type){
    signal_addLog(txt, type);
}
