/**
 * @file     eeprom.h
 * @brief    Contains all macro definitions and function prototypes
 *           support for EEPROM firmware library on LPC
 * @version  1.0
 * @date     02. June. 2011
 * @author   NXP MCU SW Application Team
 *
 * Copyright(C) 2011, NXP Semiconductor
 * All rights reserved.
 **/

#ifndef EEPROM_H
#define EEPROM_H
#include <stdint.h>

// Defining the EEPROM command register constants
#define EEPROM_CMD_8_BIT_READ (0) // 8-bit read needed
#define EEPROM_CMD_16_BIT_READ (1)
#define EEPROM_CMD_32_BIT_READ (2)
#define EEPROM_CMD_8_BIT_WRITE (3) // 8-bit write needed
#define EEPROM_CMD_16_BIT_WRITE (4)
#define EEPROM_CMD_32_BIT_WRITE (5)
#define EEPROM_CMD_ERASE_PRG_PAGE (6) // Erase and program page

#define EEPROM_CMD_RDPREFETCH (1 << 3)

// Defining the EEPROM page size and number of pages
#define EEPROM_PAGE_SIZE 64
#define EEPROM_PAGE_NUM 63

// Defining the EEPROM register addresses

// Page offset
#define EEPROM_PAGE_OFFSET(n) (n & 0x3F)
// Page address (lower 6 bits are dont care for the page address)
#define EEPROM_PAGE_ADDRESS(n) ((n & 0x3F) << 6)

// EEPROM write data register
#define EEPROM_WDATA_8_BIT(n) (n & 0x000000FF) // 8-bit write data
#define EEPROM_WDATA_16_BIT(n) (n & 0x0000FFFF)
#define EEPROM_WDATA_32_BIT(n) (n & 0xFFFFFFFF)

// EEPROM read data register
#define EEPROM_RDATA_8_BIT(n) (n & 0x000000FF) // 8-bit read data
#define EEPROM_RDATA_16_BIT(n) (n & 0x0000FFFF)
#define EEPROM_RDATA_32_BIT(n) (n & 0xFFFFFFFF)

// EEPROM power down register (Not used in this project)
#define EEPROM_PWRDWN (1 << 0)

// Define the EEPROM start and end of the EEPROM memory
#define EEPROM_ENDOF_RW (26)
// End of the program
#define EEPROM_ENDOF_PROG (28)

/**
 * @brief Function to initialize the EEPROM. Setting up necessary configurations and prepares the EEPROM for read and write operations.
 * @param None
 */
void eeprom_init(void);

/**
 * @brief Function to write data to the EEPROM, starting from the specified page offset
 * within the given page address. The data to be written and its size are provided
 * as parameters.
 * @param page_offset The offset within the page where writing should start.
 * @param page_address The address of the EEPROM page to write to.
 * @param data Pointer to the data to be written.
 * @param size The size of the data to be written, in bytes.
 */
void eeprom_write(uint16_t page_offset, uint16_t page_address, void *data, uint32_t size);

/**
 * @brief Function to read data from the EEPROM, starting from the specified page offset
 * within the given page address. The data read is stored in the provided buffer.
 *
 * @param page_offset The offset within the page where reading should start.
 * @param page_address The address of the EEPROM page to read from.
 * @param data Pointer to the buffer where the read data will be stored.
 * @param size The size of the data to be read, in bytes.
 */
void eeprom_read(uint16_t page_offset, uint16_t page_address, void *data, uint32_t size);

/**
 * @brief Function to erase the EEPROM at the specified address.
 * @param address The address of the EEPROM to be erased.
 */
void eeprom_erase(uint16_t address);

#endif // End of file
