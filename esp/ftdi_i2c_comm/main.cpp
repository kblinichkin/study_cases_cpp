#include <ftd2xx.h>
#include <stdio.h>

int main() {
    FT_HANDLE ftHandle;
    char serialNumber[] = "FT67ZH87";
    FT_STATUS ftStatus = FT_OpenEx(serialNumber, FT_OPEN_BY_SERIAL_NUMBER, &ftHandle);

    if (ftStatus != FT_OK) {
        printf("Can't open FT232H device! ftStatus=%d\n", ftStatus);
        getchar();
        return 1;
    }

    FT_Close(ftHandle);
}
