#ifndef VALUES_H
#define VALUES_H

#include <QStringList>

static QStringList freqList = {"5.2K", "7.8K", "10.4K", "12.5K", "15.6K",
                           "20.8K", "27.3K", "31.3K", "39.1K", "54.7K", "62.5K"};
static int freqMul[] = {0x03, 0x05, 0x07, 0x07, 0x07, 0x07, 0x06, 0x07, 0x04, 0x06, 0x07};

static int freqDiv[] = {0x05, 0x05, 0x05, 0x04, 0x03, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00};

static QStringList stepList = {"FULL STEP", "HALF STEP", "1/4 MICRO", "1/8 MICRO", "1/16 MICRO", "1/32 MICRO",
                     "1/64 MICRO", "1/128 MICRO"};

static QStringList ocdTHList = {"31,25", "125", "250", "375", "500", "625", "750", "875", "1,00"};

static int ocdTH[] = {0x00, 0x03, 0x07, 0x0B, 0x0F, 0x13, 0x17, 0x1B, 0x1f};
#endif // VALUES_H
