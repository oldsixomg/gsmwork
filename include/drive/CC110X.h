/* ----------------------------------------------------------------------------
 *         vteng 公司提供支持 
 * ----------------------------------------------------------------------------
 * 版权：2010 VTENG公司
 *
 * 公司保留所有权力
 *
 *此文件为TI公司CC110X的驱动程序
 *
 *
 *
 *
 *
 * ----------------------------------------------------------------------------
 */

#ifndef CC110X_H
#define CC110X_H

#define CC1100 
//#define CHIP_CC1101
#define CC110X CC1100

#include "bsp.h"


#define WL_MAX_SIZE     64

// Configuration Registers
#define CC110X_IOCFG2           0x00        // GDO2 output pin configuration
#define CC110X_IOCFG1           0x01        // GDO1 output pin configuration
#define CC110X_IOCFG0           0x02        // GDO0 output pin configuration
#define CC110X_FIFOTHR          0x03        // RX FIFO and TX FIFO thresholds
#define CC110X_SYNC1            0x04        // Sync word, high byte
#define CC110X_SYNC0            0x05        // Sync word, low byte
#define CC110X_PKTLEN           0x06        // Packet length
#define CC110X_PKTCTRL1         0x07        // Packet automation control
#define CC110X_PKTCTRL0         0x08        // Packet automation control
#define CC110X_ADDR             0x09        // Device address
#define CC110X_CHANNR           0x0A        // Channel number
#define CC110X_FSCTRL1          0x0B        // Frequency synthesizer control
#define CC110X_FSCTRL0          0x0C        // Frequency synthesizer control
#define CC110X_FREQ2            0x0D        // Frequency control word, high byte
#define CC110X_FREQ1            0x0E        // Frequency control word, middle byte
#define CC110X_FREQ0            0x0F        // Frequency control word, low byte
#define CC110X_MDMCFG4          0x10        // Modem configuration
#define CC110X_MDMCFG3          0x11        // Modem configuration
#define CC110X_MDMCFG2          0x12        // Modem configuration
#define CC110X_MDMCFG1          0x13        // Modem configuration
#define CC110X_MDMCFG0          0x14        // Modem configuration
#define CC110X_DEVIATN          0x15        // Modem deviation setting
#define CC110X_MCSM2            0x16        // Main Radio Cntrl State Machine config
#define CC110X_MCSM1            0x17        // Main Radio Cntrl State Machine config
#define CC110X_MCSM0            0x18        // Main Radio Cntrl State Machine config
#define CC110X_FOCCFG           0x19        // Frequency Offset Compensation config
#define CC110X_BSCFG            0x1A        // Bit Synchronization configuration
#define CC110X_AGCCTRL2         0x1B        // AGC control
#define CC110X_AGCCTRL1         0x1C        // AGC control
#define CC110X_AGCCTRL0         0x1D        // AGC control
#define CC110X_WOREVT1          0x1E        // High byte Event 0 timeout
#define CC110X_WOREVT0          0x1F        // Low byte Event 0 timeout
#define CC110X_WORCTRL          0x20        // Wake On Radio control
#define CC110X_FREND1           0x21        // Front end RX configuration
#define CC110X_FREND0           0x22        // Front end TX configuration
#define CC110X_FSCAL3           0x23        // Frequency synthesizer calibration
#define CC110X_FSCAL2           0x24        // Frequency synthesizer calibration
#define CC110X_FSCAL1           0x25        // Frequency synthesizer calibration
#define CC110X_FSCAL0           0x26        // Frequency synthesizer calibration
#define CC110X_RCCTRL1          0x27        // RC oscillator configuration
#define CC110X_RCCTRL0          0x28        // RC oscillator configuration
#define CC110X_FSTEST           0x29        // Frequency synthesizer cal control
#define CC110X_PTEST            0x2A        // Production test
#define CC110X_AGCTEST          0x2B        // AGC test
#define CC110X_TEST2            0x2C        // Various test settings
#define CC110X_TEST1            0x2D        // Various test settings
#define CC110X_TEST0            0x2E        // Various test settings

// Status registers
#define CC110X_PARTNUM          0x30        // Part number
#define CC110X_VERSION          0x31        // Current version number
#define CC110X_FREQEST          0x32        // Frequency offset estimate
#define CC110X_LQI              0x33        // Demodulator estimate for link quality
#define CC110X_RSSI             0x34        // Received signal strength indication
#define CC110X_MARCSTATE        0x35        // Control state machine state
#define CC110X_WORTIME1         0x36        // High byte of WOR timer
#define CC110X_WORTIME0         0x37        // Low byte of WOR timer
#define CC110X_PKTSTATUS        0x38        // Current GDOx status and packet status
#define CC110X_VCO_VC_DAC       0x39        // Current setting from PLL cal module
#define CC110X_TXBYTES          0x3A        // Underflow and # of bytes in TXFIFO
#define CC110X_RXBYTES          0x3B        // Overflow and # of bytes in RXFIFO

// Multi byte memory locations
#define CC110X_PATABLE          0x3E
#define CC110X_TXFIFO           0x3F
#define CC110X_RXFIFO           0x3F

// Definitions for burst/single access to registers
#define CC110X_WRITE_BURST      0x40
#define CC110X_READ_SINGLE      0x80
#define CC110X_READ_BURST       0xC0

// Strobe commands
#define CC110X_SRES             0x30        // Reset chip.
#define CC110X_SFSTXON          0x31        // Enable and calibrate frequency synthesizer (if MCSM0.FS_AUTOCAL=1).
                                            // If in RX/TX: Go to a wait state where only the synthesizer is
                                            // running (for quick RX / TX turnaround).
#define CC110X_SXOFF            0x32        // Turn off crystal oscillator.
#define CC110X_SCAL             0x33        // Calibrate frequency synthesizer and turn it off
                                            // (enables quick start).
#define CC110X_SRX              0x34        // Enable RX. Perform calibration first if coming from IDLE and
                                            // MCSM0.FS_AUTOCAL=1.
#define CC110X_STX              0x35        // In IDLE state: Enable TX. Perform calibration first if
                                            // MCSM0.FS_AUTOCAL=1. If in RX state and CCA is enabled:
                                            // Only go to TX if channel is clear.
#define CC110X_SIDLE            0x36        // Exit RX / TX, turn off frequency synthesizer and exit
                                            // Wake-On-Radio mode if applicable.
#define CC110X_SAFC             0x37        // Perform AFC adjustment of the frequency synthesizer
#define CC110X_SWOR             0x38        // Start automatic RX polling sequence (Wake-on-Radio)
#define CC110X_SPWD             0x39        // Enter power down mode when CSn goes high.
#define CC110X_SFRX             0x3A        // Flush the RX FIFO buffer.
#define CC110X_SFTX             0x3B        // Flush the TX FIFO buffer.
#define CC110X_SWORRST          0x3C        // Reset real time clock.
#define CC110X_SNOP             0x3D        // No operation. May be used to pad strobe commands to two
                                            // bytes for simpler software.
//自定义命令，用于取得CC110X的状态。
#define CC110X_GET_TX_STAT      CC110X_SNOP   
#define CC110X_GET_RX_STAT      (CC110X_SNOP|CC110X_READ_SINGLE)

//----------------------------------------------------------------------------------
// Chip Status Byte
//----------------------------------------------------------------------------------

// Bit fields in the chip status byte
#define CC110X_STATUS_CHIP_RDYn_BM             0x80
#define CC110X_STATUS_STATE_BM                 0x70
#define CC110X_STATUS_FIFO_BYTES_AVAILABLE_BM  0x0F

// Chip states
#define CC110X_STATE_IDLE                      0x00
#define CC110X_STATE_RX                        0x10
#define CC110X_STATE_TX                        0x20
#define CC110X_STATE_FSTXON                    0x30
#define CC110X_STATE_CALIBRATE                 0x40
#define CC110X_STATE_SETTLING                  0x50
#define CC110X_STATE_RX_OVERFLOW               0x60
#define CC110X_STATE_TX_UNDERFLOW              0x70

#define CC110X_TX_FIFO_NOT_ENOUGH              0x80
#define CC110X_RX_FIFO_NOT_ENOUGH              0x80
#define CC110X_STATE_STATE_MASK                0x70
#define CC110X_STATE_BYTE_MASK                 0x0F
//----------------------------------------------------------------------------------
// Other register bit fields
//----------------------------------------------------------------------------------
#define CC110X_LQI_CRC_OK_BM                   0x80
#define CC110X_LQI_EST_BM                      0x7F



//CS拉高和拉低
#define TO_CC110X_CS_LOW        AT91F_PIO_ClearOutput(AT91C_BASE_PIOA,CC110X_CS);\
                                AT91F_PIO_SetOutput(AT91C_BASE_PIOA,W25X16_CS);
#define TO_CC110X_CS_HIGH       AT91F_PIO_SetOutput(AT91C_BASE_PIOA,CC110X_CS);

#define WRITE_CC110X_SPI(byte)  AT91F_PIO_SetOutput(AT91C_BASE_PIOA,W25X16_CS);\
                                SPI_Write(AT91C_BASE_SPI,SPI_SELECT_CC110X,(byte));\

#define READ_CC110X_SPI         SPI_Read(AT91C_BASE_SPI)
//读之前要等待一段时间
#define CC110X_READ_WAIT        wait_us(70)
#define CC110X_WRITE_WAIT       wait_us(15)


#define  FRAME_CONTROL_DATA     0
#define  FRAME_CONTROL_ACK      (1 << 0)
#define  FRAME_CONTROL_REQ_ACK  (1 << 3)

#define  CRC_OK              0X80
#define  PHY_FRAME_HEAD_LENGHT  11
//物理层帧结构,IAR要字对齐
typedef struct{
  uint8   lenght;
  uint8   control;
  uint8   serial;
  uint32  dest_addr;
  uint32  source_addr;
}PHY_FRAME_HEAD;
//配置结构定义
typedef struct {
    uint8 fsctrl1;    // Frequency synthesizer control.
    uint8 fsctrl0;    // Frequency synthesizer control.
    uint8 freq2;      // Frequency control word, high byte.
    uint8 freq1;      // Frequency control word, middle byte.
    uint8 freq0;      // Frequency control word, low byte.
    uint8 mdmcfg4;    // Modem configuration.
    uint8 mdmcfg3;    // Modem configuration.
    uint8 mdmcfg2;    // Modem configuration.
    uint8 mdmcfg1;    // Modem configuration.
    uint8 mdmcfg0;    // Modem configuration.
    uint8 channr;     // Channel number.
    uint8 deviatn;    // Modem deviation setting (when FSK modulation is enabled).
    uint8 frend1;     // Front end RX configuration.
    uint8 frend0;     // Front end RX configuration.
    uint8 mcsm0;      // Main Radio Control State Machine configuration.
    uint8 foccfg;     // Frequency Offset Compensation Configuration.
    uint8 bscfg;      // Bit synchronization Configuration.
    uint8 agcctrl2;   // AGC control.
    uint8 agcctrl1;   // AGC control.
    uint8 agcctrl0;   // AGC control.
    uint8 fscal3;     // Frequency synthesizer calibration.
    uint8 fscal2;     // Frequency synthesizer calibration.
    uint8 fscal1;     // Frequency synthesizer calibration.
    uint8 fscal0;     // Frequency synthesizer calibration.
    uint8 fstest;     // Frequency synthesizer calibration.
    uint8 test2;      // Various test settings.
    uint8 test1;      // Various test settings.
    uint8 test0;      // Various test settings.
    uint8 iocfg2;     // GDO2 output pin configuration.
    uint8 iocfg0;     // GDO0 output pin configuration.
    uint8 pktctrl1;   // Packet automation control.
    uint8 pktctrl0;   // Packet automation control.
    uint8 addr;       // Device address.
    uint8 pktlen;     // Packet length.
} HAL_RF_CONFIG;

typedef struct {
  void (*phy_recv_to_mac)(unsigned char* data_ptr);
}PHY_CONFIG;

// For rapid chip configuration with a minimum of function overhead.
// The array has to be set up with predefined values for all registers.
typedef uint8 HAL_RF_BURST_CONFIG[47];

// The chip status byte, returned by chip for all SPI accesses
typedef uint8 HAL_RF_STATUS;


#define CC110X_EXT extern


CC110X_EXT HAL_RF_CONFIG myRfConfig;

CC110X_EXT uint8 myPaTable[8];
CC110X_EXT uint8 myPaTableLen ;




//----------------------------------------------------------------------------------
// Function declarations
//----------------------------------------------------------------------------------

CC110X_EXT void  CC110X_Config(HAL_RF_CONFIG* rfConfig,  uint8* rfPaTable, uint8 rfPaTableLen);
CC110X_EXT void  CC110X_BurstConfig(const HAL_RF_BURST_CONFIG rfConfig,  uint8* rfPaTable, uint8 rfPaTableLen);
CC110X_EXT void  CC110X_ResetChip(void);
CC110X_EXT uint8 CC110X_ReadStatusReg(uint8 addr);
CC110X_EXT uint8 CC110X_ReadReg(uint8 addr);

CC110X_EXT void CC110X_WriteReg(uint8 addr, uint8 data);
CC110X_EXT HAL_RF_STATUS CC110X_WriteFifo( uint8* data, uint8 length);
CC110X_EXT uint8 CC110X_ReadFifo(uint8* data, uint8 length);
CC110X_EXT HAL_RF_STATUS CC110X_Strobe(uint8 cmd);
CC110X_EXT HAL_RF_STATUS CC110X_GetTxStatus(void);
CC110X_EXT HAL_RF_STATUS CC110X_GetRxStatus(void);
CC110X_EXT void CC110X_SendPacket(uint8 *txBuffer, uint8 size);
CC110X_EXT uint8 CC110X_ReceivePacket(uint8 *rxBuffer, uint8 length);
/*
函数名：CC110X_ReadBurstReg
参数：addr功能寄存器的地址0X00到0X3D，在0X30到OX3D他们和状态寄存器共用一个地址，
返回值：功能寄存器当前值
描述：突发读取某一块数据
*/
extern uint8 CC110X_ReadBurstReg(uint8 addr , uint8 *buffer, uint8 count);


extern void cc110x_init(void);
extern void cc110x_idle(void);
extern void cc110x_send_data(uint8 *data_ptr , uint8 lenght);

/*
函数名：CC110X_WriteBurstReg
参数：
返回值：
描述：
*/
extern void CC110X_WriteBurstReg(uint8 addr, uint8 *buffer, uint8 count) ;


/*
*函数名：phy_send_data
*输入：
*data_ptr  发送数据缓冲区地址
*dest_addr 发送目的地址
*lenght  发送数据长度
*返回值：
*作用：
*前提：
*/
extern void phy_send_data(uint8 *data_ptr  , uint8 lenght);


/*
*函数名：phy_recv_data
*输入：data_ptr 读取数据存放的地址
*返回值：无
*作用：用于接收数据，当中断发生时调用此函数读取数据并进行处理（提取物理帧头）
*      并把处理后的数据发送给上层。
*前提：
*/
extern void phy_recv_data(void);
/*
*函数名：phy_config
*输入：
*返回值：无
*作用：用于配置物理层，像注册钩子函数等。
*前提：
*/
extern void wl_phy_init(PHY_CONFIG *phy_config);
#endif //CC110X_H