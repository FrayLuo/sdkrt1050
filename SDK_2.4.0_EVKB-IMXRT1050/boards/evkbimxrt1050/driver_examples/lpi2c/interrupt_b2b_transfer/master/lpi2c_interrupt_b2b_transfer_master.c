/*
 * The Clear BSD License
 * Copyright 2017 NXP
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*  Standard C Included Files */
#include <stdio.h>
#include <string.h>
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_lpi2c.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_I2C_MASTER_BASE (LPI2C1_BASE)

/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/* Get frequency of lpi2c clock */
#define LPI2C_CLOCK_FREQUENCY ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (LPI2C_CLOCK_SOURCE_DIVIDER + 1U))

#define LPI2C_MASTER_CLOCK_FREQUENCY LPI2C_CLOCK_FREQUENCY


#define EXAMPLE_I2C_MASTER ((LPI2C_Type *)EXAMPLE_I2C_MASTER_BASE)

#define I2C_MASTER_SLAVE_ADDR_7BIT 0x7EU
#define I2C_BAUDRATE 100000U
#define I2C_DATA_LENGTH 33U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t g_master_txBuff[I2C_DATA_LENGTH];
uint8_t g_master_rxBuff[I2C_DATA_LENGTH];
lpi2c_master_handle_t g_m_handle;
volatile bool g_MasterCompletionFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void lpi2c_master_callback(LPI2C_Type *base, lpi2c_master_handle_t *handle, status_t status, void *userData)
{
    /* Signal transfer success when received success status. */
    if (status == kStatus_Success)
    {
        g_MasterCompletionFlag = true;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    lpi2c_master_transfer_t masterXfer = {0};
    status_t reVal = kStatus_Fail;

    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /*Clock setting for LPI2C*/
    CLOCK_SetMux(kCLOCK_Lpi2cMux, LPI2C_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, LPI2C_CLOCK_SOURCE_DIVIDER);

    PRINTF("\r\nLPI2C board2board interrupt example -- Master transfer.\r\n");

    /* Set up i2c master to send data to slave*/
    /* First data in txBuff is data length of the transmiting data. */
    g_master_txBuff[0] = I2C_DATA_LENGTH - 1U;
    for (uint32_t i = 1U; i < I2C_DATA_LENGTH; i++)
    {
        g_master_txBuff[i] = i - 1;
    }

    PRINTF("Master will send data :");
    for (uint32_t i = 0U; i < I2C_DATA_LENGTH - 1; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_master_txBuff[i + 1]);
    }
    PRINTF("\r\n\r\n");

    lpi2c_master_config_t masterConfig;

    /*
     * masterConfig.debugEnable = false;
     * masterConfig.ignoreAck = false;
     * masterConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * masterConfig.baudRate_Hz = 100000U;
     * masterConfig.busIdleTimeout_ns = 0;
     * masterConfig.pinLowTimeout_ns = 0;
     * masterConfig.sdaGlitchFilterWidth_ns = 0;
     * masterConfig.sclGlitchFilterWidth_ns = 0;
     */
    LPI2C_MasterGetDefaultConfig(&masterConfig);

    /* Change the default baudrate configuration */
    masterConfig.baudRate_Hz = I2C_BAUDRATE;

    /* Initialize the LPI2C master peripheral */
    LPI2C_MasterInit(EXAMPLE_I2C_MASTER, &masterConfig, LPI2C_MASTER_CLOCK_FREQUENCY);

    /* Create the LPI2C handle for the non-blocking transfer */
    LPI2C_MasterTransferCreateHandle(EXAMPLE_I2C_MASTER, &g_m_handle, lpi2c_master_callback, NULL);

    /* subAddress = 0x01, data = g_master_txBuff - write to slave.
      start + slaveaddress(w) + subAddress + length of data buffer + data buffer + stop*/
    uint8_t deviceAddress = 0x01U;
    masterXfer.slaveAddress = I2C_MASTER_SLAVE_ADDR_7BIT;
    masterXfer.direction = kLPI2C_Write;
    masterXfer.subaddress = (uint32_t)deviceAddress;
    masterXfer.subaddressSize = 1;
    masterXfer.data = g_master_txBuff;
    masterXfer.dataSize = I2C_DATA_LENGTH;
    masterXfer.flags = kLPI2C_TransferDefaultFlag;

    /* Send master non-blocking data to slave */
    reVal = LPI2C_MasterTransferNonBlocking(EXAMPLE_I2C_MASTER, &g_m_handle, &masterXfer);
    /*  Reset master completion flag to false. */
    g_MasterCompletionFlag = false;
    if (reVal != kStatus_Success)
    {
        return -1;
    }
    /*  Wait for transfer completed. */
    while (!g_MasterCompletionFlag)
    {
    }
    g_MasterCompletionFlag = false;

    PRINTF("Receive sent data from slave :");

    /* subAddress = 0x01, data = g_master_rxBuff - read from slave.
      start + slaveaddress(w) + subAddress + repeated start + slaveaddress(r) + rx data buffer + stop */
    masterXfer.slaveAddress = I2C_MASTER_SLAVE_ADDR_7BIT;
    masterXfer.direction = kLPI2C_Read;
    masterXfer.subaddress = (uint32_t)deviceAddress;
    masterXfer.subaddressSize = 1;
    masterXfer.data = g_master_rxBuff;
    masterXfer.dataSize = I2C_DATA_LENGTH - 1;
    masterXfer.flags = kLPI2C_TransferDefaultFlag;

    reVal = LPI2C_MasterTransferNonBlocking(EXAMPLE_I2C_MASTER, &g_m_handle, &masterXfer);
    if (reVal != kStatus_Success)
    {
        return -1;
    }
    /*  Reset master completion flag to false. */
    g_MasterCompletionFlag = false;

    /*  Wait for transfer completed. */
    while (!g_MasterCompletionFlag)
    {
    }
    g_MasterCompletionFlag = false;

    for (uint32_t i = 0U; i < I2C_DATA_LENGTH - 1; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_master_rxBuff[i]);
    }
    PRINTF("\r\n\r\n");

    /* Transfer completed. Check the data.*/
    for (uint32_t i = 0U; i < I2C_DATA_LENGTH - 1; i++)
    {
        if (g_master_rxBuff[i] != g_master_txBuff[i + 1])
        {
            PRINTF("\r\nError occured in the transfer ! \r\n");
            break;
        }
    }

    PRINTF("\r\nEnd of LPI2C example .\r\n");
    while (1)
    {
    }
}
