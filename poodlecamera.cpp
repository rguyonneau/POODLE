#include "poodlecamera.h"
#include <QDebug>

#define TARGETX 270
#define TARGETY 400

PoodleCamera::PoodleCamera(Log_handler* logs){

    _dataImageProcessed = nullptr;
    _dataImageNonProcessed = nullptr;

    _coeffx = 0;
    _coeffy = 0;

    _logs = logs;

    _isCalibrated = false;
}


bool PoodleCamera::getDataFromCamera(){

   if ( !_camera.open()) {
        _logs->addLog("Error while opening the camera", LOG_ERR);
        return false;
    }

    _camera.grab();
    //remove old data
    if(_dataImageProcessed != nullptr){
        delete _dataImageProcessed;
    }
    if(_dataImageNonProcessed != nullptr){
        delete _dataImageNonProcessed;
    }

    //allocate memory
    _dataImageProcessed = new unsigned char[_camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB )];
    _dataImageNonProcessed = new unsigned char[_camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB )];

    //extract the image in rgb format
    _camera.retrieve ( _dataImageProcessed,raspicam::RASPICAM_FORMAT_RGB );//get camera image
    _camera.retrieve ( _dataImageNonProcessed,raspicam::RASPICAM_FORMAT_RGB );//get camera image

    _image_width = _camera.getWidth();
    _image_height = _camera.getHeight();

    //free resources
    _camera.release();
    return true;
}


bool PoodleCamera::process(std::vector<double>& adv_pos){
    if(!_isCalibrated){
        _logs->addLog("The camera should be calibrated first!", LOG_ERR);
        return false;
    }


    if(_coeffx == 0 || _coeffy == 0){
        _logs->addLog("On of the coeff is equal to 0, that should not be", LOG_ERR);
        return false;
    }

    if(!getDataFromCamera()){
        return false;
    }


    std::vector<unsigned int> adventices(0);
    unsigned int nbadvendices = 0;
    unsigned int maxXadventice = 20/_coeffx+1; //2cm max
    unsigned int maxYadventice = 20/_coeffy+1; //2cm max

    for (unsigned int i=0; i<_camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB )-3; i+=3){
        unsigned int x_pixel = (i/3) %_camera.getWidth();
        unsigned int y_pixel = (i/3) /_camera.getWidth();

        if(y_pixel < _ypx_inf || y_pixel > _ypx_sup){
            _dataImageProcessed[i] = 0;
            _dataImageProcessed[i+1] = 0;
            _dataImageProcessed[i+2] = 0;
        }else if( x_pixel < _xpx_inf || x_pixel > _xpx_sup){
            _dataImageProcessed[i] = 0;
            _dataImageProcessed[i+1] = 0;
            _dataImageProcessed[i+2] = 0;
        }else{
            if(_dataImageNonProcessed[i] < 70 && _dataImageNonProcessed[i+1] < 70 && _dataImageNonProcessed[i+2] < 70){
                bool already = false;
                for(unsigned int j=0; j<nbadvendices; j++){
                    if( (x_pixel > (adventices[j*4+0] - maxXadventice)) &&
                        (x_pixel < (adventices[j*4+1] + maxXadventice)) &&
                        (y_pixel > (adventices[j*4+2] - maxYadventice)) &&
                        (y_pixel < (adventices[j*4+3] + maxYadventice)) ){
                        //the pixel belongs to an adventice already detected
                        //we update its boundary box
                        if(x_pixel < adventices[j*4+0]){
                            adventices[j*4+0] = x_pixel;
                        }
                        if(x_pixel > adventices[j*4+1]){
                            adventices[j*4+1] = x_pixel;
                        }
                        if(y_pixel < adventices[j*4+2]){
                            adventices[j*4+2] = y_pixel;
                        }
                        if(y_pixel > adventices[j*4+3]){
                            adventices[j*4+3] = y_pixel;
                        }
                        already = true;
                        break; // ho!!!
                    }
                }
                if(!already){
                    //new adventice
                    adventices.push_back(x_pixel); // xinf
                    adventices.push_back(x_pixel); // xsup
                    adventices.push_back(y_pixel); // yinf
                    adventices.push_back(y_pixel); // ysup
                    nbadvendices ++;
                }

                _dataImageProcessed[i] = 255;
                _dataImageProcessed[i+1] = 255;
                _dataImageProcessed[i+2] = 255;
            }else{
                _dataImageProcessed[i] = 0;
                _dataImageProcessed[i+1] = 0;
                _dataImageProcessed[i+2] = 0;
            }
        }

    }
    adv_pos.clear();
    for(unsigned int j=0; j<nbadvendices; j++){

        if(adventices[j*4+1] - adventices[j*4+0] < 30 && adventices[j*4+3] - adventices[j*4+2] < 30){

            adv_pos.push_back(x_px2mm((adventices[j*4+1] - adventices[j*4+0])/2.0+adventices[j*4+0]));
            adv_pos.push_back(y_px2mm((adventices[j*4+3] - adventices[j*4+2])/2.0+adventices[j*4+2]));

        }
    }

    return true;
}

void PoodleCamera::calibrate(unsigned int xinfpx, unsigned int xsuppx, unsigned int yinfpx, unsigned int ysuppx){
    if(!getDataFromCamera()){
        return;
    }

    unsigned int minx_px = _camera.getWidth();
    unsigned int maxx_px = 0;
    unsigned int miny_px = _camera.getHeight();
    unsigned int maxy_px = 0;

    for (unsigned int i=0; i<_camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB )-3; i+=3){

        if((i/3) /_camera.getWidth() < yinfpx || (i/3) /_camera.getWidth() > ysuppx){
            _dataImageProcessed[i] = 60;
            _dataImageProcessed[i+1] = 60;
            _dataImageProcessed[i+2] = 60;
        }else if((i/3) %_camera.getWidth() < xinfpx || (i/3) %_camera.getWidth() > xsuppx){
            _dataImageProcessed[i] = 30;
            _dataImageProcessed[i+1] = 30;
            _dataImageProcessed[i+2] = 30;
        }else{
            if(_dataImageNonProcessed[i] > 70 || _dataImageNonProcessed[i+1] > 70 || _dataImageNonProcessed[i+2] > 70){
                unsigned int xpx = (i/3) %_camera.getWidth();
                unsigned int ypx = (i/3) /_camera.getWidth();

                if(xpx > maxx_px){
                    maxx_px = xpx;
                }
                if(xpx < minx_px){
                    minx_px = xpx;
                }
                if(ypx > maxy_px){
                    maxy_px = ypx;
                }
                if(ypx < miny_px){
                    miny_px = ypx;
                }
            }
        }
    }

    _coeffx = TARGETX/((double)maxx_px - minx_px);
    _coeffy = TARGETY/((double)maxy_px - miny_px);

    _xpx_inf = minx_px + 20;
    _xpx_sup = maxx_px - 20;
    _ypx_inf = miny_px + 20;
    _ypx_sup = maxy_px - 20;

    _offcetxpx = minx_px;
    _offcetypx = miny_px;

    _isCalibrated = true;

}

double PoodleCamera::x_px2mm(unsigned int xpx){
    return (TARGETX - (xpx-_offcetxpx)*_coeffx);
}

double PoodleCamera::y_px2mm(unsigned int ypx){
    return (ypx-_offcetypx)*_coeffy;
}
