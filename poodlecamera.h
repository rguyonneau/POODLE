#ifndef POODLECAMERA_H
#define POODLECAMERA_H

#include <raspicam/raspicam.h>
#include <vector>

#include "log_handler.h"

class PoodleCamera
{
public:
    PoodleCamera(Log_handler* logs);

    const unsigned char* getProcessedData() const { return _dataImageProcessed; }
    const unsigned char* getNonProcessedData() const { return _dataImageNonProcessed; }
    unsigned int getWidth() const { return _image_width; }
    unsigned int getHeight() const { return _image_height; }

    double x_px2mm(unsigned int xpx);
    double y_px2mm(unsigned int ypx);

    unsigned int getOffcetXpx() const { return _offcetxpx; }
    unsigned int getOffcetYpx() const { return _offcetypx; }

    double getCoeffx() const { return _coeffx; }
    double getCoeffy() const { return _coeffy; }

    bool getDataFromCamera();
    bool process(std::vector<double>& adv_pos);
    void calibrate(unsigned int xinfpx, unsigned int xsuppx, unsigned int yinfpx, unsigned int ysuppx);

    bool get_isCalibrated() const { return _isCalibrated; }

private:


    raspicam::RaspiCam _camera; //Camera object

    unsigned char* _dataImageProcessed;
    unsigned char* _dataImageNonProcessed;

    unsigned int _image_width;
    unsigned int _image_height;

    double _coeffx;
    double _coeffy;

    unsigned int _offcetxpx;
    unsigned int _offcetypx;

    unsigned int _xpx_inf;
    unsigned int _xpx_sup;
    unsigned int _ypx_inf;
    unsigned int _ypx_sup;

    Log_handler* _logs;

    bool _isCalibrated;
};

#endif // POODLECAMERA_H
