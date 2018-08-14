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

#define DITO                0x01
#define LUNETTA             0x02
#define NASTRO              0x03
#define CORPO               0x04
#define MANINE              0x05
#define CHIUSURA            0x06


#endif // I2CCOMMANDS_H
