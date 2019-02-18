#ifndef SPI
#define SPI

#include <stdint.h>

void spi_initialize();
inline void Mode16(void);
inline void Mode8(void);
uint8_t spi_send_8(uint8_t data);
uint16_t spi_send(uint16_t data);
void write_cmd_8(uint8_t com);
void write_data_8(uint8_t data);
void write_cmd_16(uint16_t com);
void write_data_16(uint16_t data);
void write_data_multi(uint16_t* out,uint16_t count);
void rotate(char c);
#endif