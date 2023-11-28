/**
 * @file pqm.c
 * @author Andrei-Dan Danila (andrei.danila@analog.com)
 * @brief Source file for pqm driver.
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
#include "pqm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "no_os_error.h"
#include "no_os_util.h"
#include "no_os_alloc.h"

/* default sine lookup table to be used if ext_buff is not available */
const uint32_t sine_lut[112] = {
	0x0001, 0x0008, 0x000C, 0x0010, 0x0020, 0x0030, 0x0040,
	0x0001, 0x0008, 0x000C, 0x0010, 0x0020, 0x0030, 0x0040,
	0x0001, 0x0008, 0x000C, 0x0010, 0x0020, 0x0030, 0x0040,
	0x0001, 0x0008, 0x000C, 0x0010, 0x0020, 0x0030, 0x0040,
	0x0001, 0x0008, 0x000C, 0x0010, 0x0020, 0x0030, 0x0040,
	0x0001, 0x0008, 0x000C, 0x0010, 0x0020, 0x0030, 0x0040,
	0x0001, 0x0008, 0x000C, 0x0010, 0x0020, 0x0030, 0x0040,
	0x0001, 0x0008, 0x000C, 0x0010, 0x0020, 0x0030, 0x0040,
	0x0001, 0x0008, 0x000C, 0x0010, 0x0020, 0x0030, 0x0040,
	0x0001, 0x0008, 0x000C, 0x0010, 0x0020, 0x0030, 0x0040,
	0x0001, 0x0008, 0x000C, 0x0010, 0x0020, 0x0030, 0x0040,
	0x0001, 0x0008, 0x000C, 0x0010, 0x0020, 0x0030, 0x0040,
	0x0001, 0x0008, 0x000C, 0x0010, 0x0020, 0x0030, 0x0040,
	0x0001, 0x0008, 0x000C, 0x0010, 0x0020, 0x0030, 0x0040,
	0x0001, 0x0008, 0x000C, 0x0010, 0x0020, 0x0030, 0x0040,
	0x0001, 0x0008, 0x000C, 0x0010, 0x0020, 0x0030, 0x0040
};

int32_t pqm_init(struct pqm_desc **desc,
		 struct pqm_init_para *param)
{
	struct pqm_desc *d;
	d = (struct pqm_desc *)no_os_calloc(1, sizeof(*d));

	if (!d)
		return -ENOMEM;

	d->ext_buff = param->ext_buff;
	d->ext_buff_len = param->ext_buff_len;
	for (int i = 0; i < TOTAL_PQM_CHANNELS; i++) {
		for (int j = 0; j < MAX_CH_ATTRS; j++) {
			d->pqm_ch_attr[i][j] = param->dev_ch_attr[i][j];
		}
	}
	for (int i = 0; i < PQM_DEVICE_ATTR_NUMBER; i++) {
		d->pqm_global_attr[i] = param->dev_global_attr[i];
	}
	*desc = d;

	return 0;
}

int32_t pqm_remove(struct pqm_desc *desc)
{
	if (!desc)
		return -EINVAL;
	no_os_free(desc);

	return 0;
}

/**
 * @brief active pqm channels
 * @param dev - descriptor for the pqm
 * @param mask - active channels mask
 * @return 0 in case of success, -1 otherwise.
 */
int32_t update_pqm_channels(void *dev, uint32_t mask)
{
	struct pqm_desc *desc;

	if(!dev)
		return -ENODEV;

	desc = dev;

	desc->active_ch = mask;
	/* If a real device. Here needs to be selected the channels to be read*/

	return 0;
}

/**
 * @brief close all channels
 * @param dev - physical instance of an pqm device
 * @return 0 in case of success.
 */
int32_t close_pqm_channels(void* dev)
{
	struct pqm_desc *desc;

	if(!dev)
		return -ENODEV;

	desc = dev;

	desc->active_ch = 0;

	return 0;
}