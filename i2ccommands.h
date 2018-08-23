#ifndef I2CCOMMANDS_H
#define I2CCOMMANDS_H

#define I2C_NOP           0x00
#define I2C_SET_PARAM     0x01
#define I2C_GET_PARAM     0x02
#define I2C_RELEASE_SW    0x03
#define I2C_MOVE          0x04
#define I2C_RUN           0x05
#define I2C_GOTO          0x06
#define I2C_GO_HOME       0x07
#define I2C_GO_MARK       0x08
#define I2C_SOFT_STOP     0x09
#define I2C_HARD_STOP     0x0A
#define I2C_RESET_DEV     0x0B
#define I2C_RESET_POS     0x0C
#define I2C_GET_STATUS    0x0D
#define I2C_SET_MOTOR     0x1F

#define DITO                0x00
#define CORPO               0x01
#define MANINE              0x02
#define CHIUSURA            0x03
#define LUNETTA             0x04
#define NASTRO              0x05
#define ALL_MOTORS          0xAB

#define RPI_START           0x1F
#define RPI_STOP            0xF1
#define ARDU_START          0x1E
#define ARDU_STOP           0xE1


#endif // I2CCOMMANDS_H
