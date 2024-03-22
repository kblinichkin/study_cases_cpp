#include <stdio.h>
#include <stdlib.h>

#include "ftd2xx.h"
#include "libMPSSE_i2c.h"

#define APP_CHECK_STATUS(exp) {if(exp!=FT_OK){printf("%s:%d:%s(): status(0x%x) \ != FT_OK\n",__FILE__, __LINE__, __FUNCTION__,exp);exit(1);}else{;}};
#define CHECK_NULL(exp){if(exp==NULL){printf("%s:%d:%s(): NULL expression \ encountered \n",__FILE__, __LINE__, __FUNCTION__);exit(1);}else{;}};

#define I2C_DEVICE_ADDRESS 0x04
#define I2C_DEVICE_BUFFER_SIZE 256
#define CHANNEL_TO_OPEN 0

/** PLAN:
*       1. Write a command and read a response from ESP32
*       2. Check high speed I2C:
*           +a) Set up external pull-ups using breadboard
*           +b) Remove internal pull-ups from ESP32 and configure for 400kHz
*           +c) Configure FTDI for 400kHz
*           +d) Check 1Mb/s
*       -->3. Show battery state to Terminal
*           a) Show to UART console
*           b) Implement a command to show the battery state in Terminal as I2C response
*       4. Think about final architecture
*/

int main() {
    Init_libMPSSE();

    uint32 channels;
    FT_STATUS status;
    FT_HANDLE ftHandle;

    status = I2C_GetNumChannels(&channels);
    APP_CHECK_STATUS(status);
    printf("Number of available I2C channels = %d\n",channels);

    FT_DEVICE_LIST_INFO_NODE devList;
    status = I2C_GetChannelInfo(CHANNEL_TO_OPEN, &devList);
    APP_CHECK_STATUS(status);
    printf("Information on channel number %d:\n", CHANNEL_TO_OPEN); /*print the dev info*/
    printf("Flags=0x%x\n",devList.Flags);
    printf("Type=0x%x\n",devList.Type);
    printf("ID=0x%x\n",devList.ID);
    printf("LocId=0x%x\n",devList.LocId);
    printf("SerialNumber=%s\n",devList.SerialNumber);
    printf("Description=%s\n",devList.Description);
    printf("ftHandle=0x%x\n",devList.ftHandle);

    status = I2C_OpenChannel(CHANNEL_TO_OPEN, &ftHandle);
    APP_CHECK_STATUS(status);

    ChannelConfig channelConf;
    channelConf.ClockRate = I2C_CLOCK_FAST_MODE_PLUS;
    channelConf.LatencyTimer = 255;

    status = I2C_InitChannel(ftHandle, &channelConf);
    APP_CHECK_STATUS(status);

    // FIXME: In suche a case buffers are sent chaotically; it's necessary to investigate
    //        correct approach
    for (int i = 0; i < 100; ++i) {
        uint32 bytesTransfered = 0;
        uint8 buffer[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x00, 0x0A};
        status = I2C_DeviceWrite(ftHandle, I2C_DEVICE_ADDRESS, 7, buffer,
            &bytesTransfered, I2C_TRANSFER_OPTIONS_START_BIT|I2C_TRANSFER_OPTIONS_STOP_BIT);
        APP_CHECK_STATUS(status);
        printf("bytesTransfered=0x%x\n",bytesTransfered);
    }
    status = I2C_CloseChannel(ftHandle);
    APP_CHECK_STATUS(status);


    Cleanup_libMPSSE();

    return 0;
}
