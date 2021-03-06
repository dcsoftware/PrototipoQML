#ifndef VALUES_H
#define VALUES_H

#include <QStringList>


static QStringList freqList = {"5.2K", "7.8K", "10.4K", "12.5K", "15.6K",
                           "20.8K", "27.3K", "31.3K", "39.1K", "54.7K", "62.5K"};
static int freqMul[] = {0x03, 0x05, 0x07, 0x07, 0x07, 0x07, 0x06, 0x07, 0x04, 0x06, 0x07};

static int freqDiv[] = {0x05, 0x05, 0x05, 0x04, 0x03, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00};

static QStringList stepList = {"FULL STEP", "HALF STEP", "1/4 MICRO", "1/8 MICRO", "1/16 MICRO", "1/32 MICRO",
                     "1/64 MICRO", "1/128 MICRO"};

static QStringList ocdTHList = {"31,25 mV", "125 mV", "250 mV", "375 mV", "500 mV", "625 mV", "750 mV", "875 mV", "1,00 V"};

static int ocdTH[] = {0x00, 0x03, 0x07, 0x0B, 0x0F, 0x13, 0x17, 0x1B, 0x1f};

static int gpioFlagInput[] = {26, 19, 13, 21, 20, 16};

static int gpioOutput[] = {4};

static int gpioEncInput[] = {6, 12, 13};
#endif // VALUES_H
