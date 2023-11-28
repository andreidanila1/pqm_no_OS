/**
 * @file pqm.h
 * @author Andrei-Dan Danila (andrei.danila@analog.com)
 * @brief Header file for pqm driver.
********************************************************************************
 * Copyright 2023(c) Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  - Neither the name of Analog Devices, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *  - The use of this software may or may not infringe the patent rights
 *    of one or more patent holders.  This license does not release you
 *    from the requirement that you obtain separate licenses from these
 *    patent holders to use this software.
 *  - Use of the software either in source or binary form, must be run
 *    on or directly connected to an Analog Devices Inc. component.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/
#ifndef PQM_H
#define PQM_H

#include <stdint.h>
#include "iio_types.h"
#include "no_os_irq.h"
#include "adin1110.h"

#define TOTAL_PQM_CHANNELS 7
#define VOLTAGE_CH_NUMBER 3
#define MAX_CH_ATTRS 10
#define PQM_DEVICE_ATTR_NUMBER 29

extern const uint32_t sine_lut[112];

enum availavle_values_type {
	V_CONSEL,
	FLICKER_MODEL,
	NOMINAL_FREQUENCY
};

enum v_consel_values {
	_4W_WYE,
	_4W_WYE_NON_BLONDEL,
	_3W_DELTA,
	_3W_DELTA_2,
	_4W_DELTA_NON_BLONDEL
};

enum flicker_model_values {
	_230V_50HZ,
	_120V_50HZ,
	_230V_60HZ,
	_120V_60HZ
};

enum nominal_frequency_values {
	_50,
	_60,
};

static const char *const pqm_v_consel_available[] = {
	[_4W_WYE] = "4W_WYE",
	[_4W_WYE_NON_BLONDEL] = "4W_WYE_NON_BLONDEL",
	[_3W_DELTA] = "3W_DELTA",
	[_3W_DELTA_2] = "3W_DELTA_2",
	[_4W_DELTA_NON_BLONDEL] = "4W_DELTA_NON_BLONDEL",
};

static const char *const pqm_flicker_model_available[] = {
	[_230V_50HZ] = "230V_50HZ",
	[_120V_50HZ] = "120V_50HZ",
	[_230V_60HZ] = "230V_60HZ",
	[_120V_60HZ] = "120V_60HZ",
};

static const char *const pqm_nominal_frequency_available[] = {
	[_50] = "50",
	[_60] = "60",
};

struct pqm_desc {
	/** Dummy registers of device for testing */
	uint8_t reg[TOTAL_PQM_CHANNELS];
	uint32_t pqm_global_attr[PQM_DEVICE_ATTR_NUMBER];
	uint32_t pqm_ch_attr[TOTAL_PQM_CHANNELS][MAX_CH_ATTRS];
	uint32_t active_ch;
	uint32_t ext_buff_len;
	uint16_t **ext_buff;
};

struct pqm_init_para {
	uint32_t dev_global_attr[PQM_DEVICE_ATTR_NUMBER];
	uint32_t dev_ch_attr[TOTAL_PQM_CHANNELS][MAX_CH_ATTRS];
	uint32_t ext_buff_len;
	uint16_t **ext_buff;
};

int32_t pqm_init(struct pqm_desc **desc,
		 struct pqm_init_para *param);

int32_t pqm_remove(struct pqm_desc *desc);

int32_t update_pqm_channels(void *dev, uint32_t mask);
int32_t close_pqm_channels(void* dev);

#endif