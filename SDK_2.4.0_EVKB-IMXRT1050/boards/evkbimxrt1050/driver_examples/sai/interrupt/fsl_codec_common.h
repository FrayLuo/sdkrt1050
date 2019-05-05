/*
 * The Clear BSD License
 * Copyright (c) 2017, NXP Semiconductors, Inc.
 * All rights reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
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

#ifndef _FSL_CODEC_COMMON_H_
#define _FSL_CODEC_COMMON_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Define I2C access function. */
typedef status_t (*codec_i2c_send_func_t)(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
typedef status_t (*codec_i2c_receive_func_t)(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);

/*! @brief CODEC device register address type. */
typedef enum _codec_reg_addr
{
    kCODEC_RegAddr8Bit = 1U,  /*!< 8-bit register address.  */
    kCODEC_RegAddr16Bit = 2U, /*!< 16-bit register address. */
} codec_reg_addr_t;

/*! @brief CODEC device register width. */
typedef enum _codec_reg_width
{
    kCODEC_RegWidth8Bit = 1U,  /*!< 8-bit register width.  */
    kCODEC_RegWidth16Bit = 2U, /*!< 16-bit register width. */
    kCODEC_RegWidth32Bit = 4U, /*!< 32-bit register width. */
} codec_reg_width_t;

/*! @brief AUDIO format definition. */
typedef enum _codec_protocol
{
    kCODEC_BusI2S = 0x0,            /*!< I2S type */
    kCODEC_BusLeftJustified = 0x1,  /*!< Left justified mode */
    kCODEC_BusRightJustified = 0x2, /*!< Right justified mode */
    kCODEC_BusPCMA = 0x3,           /*!< PCM A mode */
    kCODEC_BusPCMB = 0x4            /*!< PCM B mode */
} codec_protocol_t;

typedef struct codec_handle codec_handle_t;

/*! @brief Codec common operation */
typedef struct codec_operation
{
    status_t (*Init)(codec_handle_t *handle, void *config); /* Codec init operation */
    status_t (*SetFormat)(codec_handle_t *handle, uint32_t mclk, uint32_t sampleRate, uint32_t bitWidth); /*!< Codec format setting */
    status_t (*Deinit)(codec_handle_t *handle);
} codec_operation_t;

/*! @brief Initialize structure of WM8960 */
typedef struct codec_config
{
    /* Pointer to the user-defined I2C Send Data function. */
    codec_i2c_send_func_t I2C_SendFunc;
    /* Pointer to the user-defined I2C Receive Data function. */
    codec_i2c_receive_func_t I2C_ReceiveFunc;
    void * codecConfig; /* Codec specific configuration */
    codec_operation_t op;
} codec_config_t;

/*! @brief Codec configure definition. */
struct codec_handle
{
    /* Pointer to the user-defined I2C Send Data function. */
    codec_i2c_send_func_t I2C_SendFunc;
    /* Pointer to the user-defined I2C Receive Data function. */
    codec_i2c_receive_func_t I2C_ReceiveFunc;
    /* The I2C slave address . */ 
    uint8_t slaveAddress;
    codec_operation_t op;
};

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Write value to the register.
 *
 * @param i2cAddr I2C address.
 * @param addrType Register address type.
 * @param reg The register to write.
 * @param regWidth The width of the register.
 * @param value The value to write.
 * @param i2cSendFunc The actual I2C send function.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
status_t CODEC_I2C_WriteReg(uint8_t i2cAddr,
                            codec_reg_addr_t addrType,
                            uint32_t reg,
                            codec_reg_width_t regWidth,
                            uint32_t value,
                            codec_i2c_send_func_t i2cSendFunc);

/*!
 * @brief Read the register value.
 *
 * @param i2cAddr I2C address.
 * @param addrType Register address type.
 * @param reg The register to read.
 * @param regWidth The width of the register.
 * @param value The value read out.
 * @param i2cReceiveFunc The actual I2C receive function.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
status_t CODEC_I2C_ReadReg(uint8_t i2cAddr,
                           codec_reg_addr_t addrType,
                           uint32_t reg,
                           codec_reg_width_t regWidth,
                           void *value,
                           codec_i2c_receive_func_t i2cReceiveFunc);

/*!
 * @brief Modify the register value.
 *
 * This function modifies some bit fields of a register.
 * reg[clrMask] = value & clrMask
 *
 * @param i2cAddr I2C address.
 * @param addrType Register address type.
 * @param reg The register to modify.
 * @param regWidth The width of the register.
 * @param clrMask The mask value to clear.
 * @param value The value to set.
 * @param i2cReceiveFunc The actual I2C receive function.
 * @param i2cSendFunc The actual I2C send function.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
status_t CODEC_I2C_ModifyReg(uint8_t i2cAddr,
                             codec_reg_addr_t addrType,
                             uint32_t reg,
                             codec_reg_width_t regWidth,
                             uint32_t clrMask,
                             uint32_t value,
                             codec_i2c_receive_func_t i2cReceiveFunc,
                             codec_i2c_send_func_t i2cSendFunc);

status_t CODEC_Init(codec_handle_t *handle, codec_config_t *config);

static inline status_t CODEC_SetFormat(codec_handle_t *handle, uint32_t mclk, uint32_t sampleRate, uint32_t bitWidth)
{
    return handle->op.SetFormat(handle, mclk, sampleRate, bitWidth);
}

static inline status_t CODEC_Deinit(codec_handle_t *handle)
{
    return handle->op.Deinit(handle);
}

#if defined(__cplusplus)
}
#endif

#endif /* _FSL_CODEC_COMMON_H_ */
