#include <stdio.h>
#include <stdlib.h>

#include "ftd2xx.h"
#include "libMPSSE_i2c.h"

#define APP_CHECK_STATUS(exp) {if(exp!=FT_OK){printf("%s:%d:%s(): status(0x%x) \ != FT_OK\n",__FILE__, __LINE__, __FUNCTION__,exp);exit(1);}else{;}};
#define CHECK_NULL(exp){if(exp==NULL){printf("%s:%d:%s(): NULL expression \ encountered \n",__FILE__, __LINE__, __FUNCTION__);exit(1);}else{;}};

#define I2C_DEVICE_ADDRESS 0x32
#define I2C_DEVICE_BUFFER_SIZE 256

int main() {
    Init_libMPSSE();

    // ChannelConfig channelConf;
    // channelConf.ClockRate = I2C_CLOCK_STANDARD_MODE;
    // channelConf.LatencyTimer= 255;

    uint32 channels;
    FT_STATUS status;
    status = I2C_GetNumChannels(&channels);
    APP_CHECK_STATUS(status);
    printf("Number of available I2C channels = %d\n",channels);

    Cleanup_libMPSSE();

    return 0;
}
