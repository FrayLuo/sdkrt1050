/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
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
#include "mcdrv_adc_adc16.h"
#include "mcdrv_adc_adc16_hvp_ke18.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
* Variables
******************************************************************************/

static bool_t s_statusPass;
static volatile bool_t s_bkey = TRUE;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Function set new channel assignment for next BEMF voltage sensing.
 *        Board HVP-KE18F16 specific function.
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_AssignBemfChannel_hvp_ke18(mcdrv_adc16_t *this)
{
    s_statusPass = TRUE;

    switch (this->ui16Sector) 
    {
      /* BEMF phase C sensing */
    case 0:
    case 3:
      if((this->bldcAdcSelCfg[this->ui16Sector]) == ADC1)
      /* Set ADC_SC1_ADCH bitfield */
      this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] = 
        (this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] & ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) | 
          (ADC_SC1_ADCH(this->bldcAdc0SectorCfg[this->ui16Sector]));
      else
      /* Set ADC_SC1_ADCH bitfield */
      this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] = 
        (this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] & ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) | 
          (ADC_SC1_ADCH(this->bldcAdc1SectorCfg[this->ui16Sector]));
      break;
    
      /* BEMF phase B sensing */
    case 1: 
    case 4: 
      if((this->bldcAdcSelCfg[this->ui16Sector]) == ADC1)
      /* Set ADC_SC1_ADCH bitfield */
      this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] = 
        (this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] & ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) | 
          (ADC_SC1_ADCH(this->bldcAdc0SectorCfg[this->ui16Sector]));
      else
      /* Set ADC_SC1_ADCH bitfield */
      this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] = 
        (this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] & ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) | 
          (ADC_SC1_ADCH(this->bldcAdc1SectorCfg[this->ui16Sector]));
      
      break;
       
      /* BEMF phase A sensing */
    case 2:
    case 5:
      if((this->bldcAdcSelCfg[this->ui16Sector]) == ADC1)
      /* Set ADC_SC1_ADCH bitfield */
      this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] = 
        (this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] & ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) | 
          (ADC_SC1_ADCH(this->bldcAdc0SectorCfg[this->ui16Sector]));
      else
      /* Set ADC_SC1_ADCH bitfield */
      this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] = 
        (this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] & ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) | 
          (ADC_SC1_ADCH(this->bldcAdc1SectorCfg[this->ui16Sector]));
      
      break;
      
    default:
      break;
    }
    
    return(s_statusPass);
}
