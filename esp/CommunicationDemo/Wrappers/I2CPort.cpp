/*
 * I2CPort.cpp
 *
 *  Created on: 5 Oct 2023
 *      Author: a1
 */

#include "I2CPort.h"

#include "freertos/FreeRTOS.h"
#include "driver/i2c.h"

#define I2C_SLAVE_NUM I2C_NUM_0
#define I2C_SLAVE_TX_BUF_LEN 256  //(2 * DATA_LENGTH)
#define I2C_SLAVE_RX_BUF_LEN 256  //(2 * DATA_LENGTH)

#define ESP_SLAVE_ADDR 0x04
#define WRITE_BIT I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT I2C_MASTER_READ   /*!< I2C master read */
#define ACK_CHECK_EN 0x1           /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0 /*!< I2C master will not check ack from slave */
#define ACK_VAL 0x0       /*!< I2C ack value */
#define NACK_VAL 0x1      /*!< I2C nack value */
#define I2C_SLAVE_MAX_SPEED 100000

#define I2C_SLAVE_SDA_IO GPIO_NUM_18
#define I2C_SLAVE_SCL_IO GPIO_NUM_23


using namespace comdemo;

// FIXME: it's actually a device which is a singleton
I2CPort::I2CPort() {
	int i2c_slave_port = I2C_SLAVE_NUM;

	i2c_config_t conf_slave;
	conf_slave.sda_io_num = I2C_SLAVE_SDA_IO;            // select SDA GPIO specific to your project
	conf_slave.sda_pullup_en = GPIO_PULLUP_ENABLE;
	conf_slave.scl_io_num = I2C_SLAVE_SCL_IO;            // select SCL GPIO specific to your project
	conf_slave.scl_pullup_en = GPIO_PULLUP_ENABLE;
	conf_slave.mode = I2C_MODE_SLAVE;
	conf_slave.slave.addr_10bit_en = 0;
	conf_slave.slave.slave_addr = ESP_SLAVE_ADDR;       // slave address of your project
	conf_slave.slave.maximum_speed = I2C_SLAVE_MAX_SPEED;
	conf_slave.clk_flags = 0;                            // optional; you can use I2C_SCLK_SRC_FLAG_* flags to choose I2C source clock here

	ESP_ERROR_CHECK(i2c_param_config(i2c_slave_port, &conf_slave));
	ESP_ERROR_CHECK(i2c_driver_install(i2c_slave_port, conf_slave.mode,
	                            I2C_SLAVE_RX_BUF_LEN, I2C_SLAVE_TX_BUF_LEN, 0));
}

I2CPort::~I2CPort() {
	// TODO Auto-generated destructor stub
}

int I2CPort::slaveReadBuffer(std::vector<uint8_t>& data, uint32_t msToWait) {
	int bytesRead = i2c_slave_read_buffer(I2C_NUM_0, data.data(),
			data.size(), pdMS_TO_TICKS(msToWait));
	ESP_ERROR_CHECK_WITHOUT_ABORT(bytesRead);

	return bytesRead;
}

//i2cset 1 0x04 0x01 0xFF 0x00 0xFF 0x00 0xFF 0x00 0xFF 0x00 0xFF

int I2CPort::slaveWriteBuffer(const std::vector<uint8_t>& data, uint32_t msToWait) {

	int bytesWritten = i2c_slave_write_buffer(I2C_NUM_0, data.data(),
			data.size(), pdMS_TO_TICKS(msToWait));
	ESP_ERROR_CHECK_WITHOUT_ABORT(bytesWritten);

	return bytesWritten;
}

