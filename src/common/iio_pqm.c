/**
 * @file iio_pqm.c
 * @author Andrei-Dan Danila (andrei.danila@analog.com)
 * @brief Source file for the pqm IIO driver.
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
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "no_os_error.h"
#include "no_os_util.h"
#include "no_os_alloc.h"
#include "iio.h"
#include "iio_pqm.h"
#include "pqm.h"

/**
 * @brief utility function for computing next upcoming channel.
 * @param ch_mask - active channels .
 * @param last_idx -  previous index.
 * @param new_idx - upcoming channel index, return param.
 * @return 1 if there are more channels, 0 if done.
 */
static bool get_next_ch_idx(uint32_t ch_mask, uint32_t last_idx,
			    uint32_t *new_idx)
{
	last_idx++;
	ch_mask >>= last_idx;
	if (!ch_mask) {
		*new_idx = -1;
		return 0;
	}
	while (!(ch_mask & 1)) {
		last_idx++;
		ch_mask >>= 1;
	}
	*new_idx = last_idx;

	return 1;
}

/**
 * @brief Read the available values for v_consel, flicker model and nominal frequency attributes.
 *
 * @param device    - The iio device structure
 * @param buf       - Buffer to be filled with requested data
 * @param len 	    - Length of the received command buffer in bytes
 * @param channel   - Command channel info
 * @param attr_type - attribute descriptor
 * @return          - The length of the buffer in case of success, negative value otherwise
 */
int read_available_values(void *device, char *buf, uint32_t len,
			  const struct iio_ch_info *channel, intptr_t attr_type)
{
	uint32_t val_cnt;
	size_t i;

	strcpy(buf, "");

	switch (attr_type) {
	case V_CONSEL:
		val_cnt = NO_OS_ARRAY_SIZE(pqm_v_consel_available);
		for (i = 0; i < val_cnt; i++) {
			strcat(buf, pqm_v_consel_available[i]);
			if (i != val_cnt - 1)
				strcat(buf, " ");
		}
		break;
	case FLICKER_MODEL:
		val_cnt = NO_OS_ARRAY_SIZE(pqm_flicker_model_available);
		for (i = 0; i < val_cnt; i++) {
			strcat(buf, pqm_flicker_model_available[i]);
			if (i != val_cnt - 1)
				strcat(buf, " ");
		}
		break;
	case NOMINAL_FREQUENCY:
		val_cnt = NO_OS_ARRAY_SIZE(pqm_nominal_frequency_available);
		for (i = 0; i < val_cnt; i++) {
			strcat(buf, pqm_nominal_frequency_available[i]);
			if (i != val_cnt - 1)
				strcat(buf, " ");
		}
		break;
	default:
		return -EINVAL;
	}

	return strlen(buf);
}

/**
 * @brief Read the v_consel device attribute.
 *
 * @param device    - The iio device structure
 * @param buf       - Buffer to be filled with requested data
 * @param len 	    - Length of the received command buffer in bytes
 * @param channel   - Command channel info
 * @param attr_type - attribute descriptor
 * @return          - The length of the buffer in case of success, negative value otherwise
 */
int read_v_consel_attr(void *device, char *buf, uint32_t len,
		       const struct iio_ch_info *channel, intptr_t attr_id)
{
	struct pqm_desc *desc;
	if (!device)
		return -ENODEV;
	desc = device;
	if (attr_id < PQM_DEVICE_ATTR_NUMBER) {
		int available_id = desc->pqm_global_attr[attr_id];
		strncpy(buf, pqm_v_consel_available[available_id], len);
		return strlen(buf);
	} else {
		return -EINVAL;
	}
}

/**
 * @brief Write the v_consel device attribute.
 *
 * @param device    - The iio device structure
 * @param buf       - Buffer to be filled with requested data
 * @param len 	    - Length of the received command buffer in bytes
 * @param channel   - Command channel info
 * @param attr_type - attribute descriptor
 * @return          - The length of the buffer in case of success, negative value otherwise
 */
int write_v_consel_attr(void *device, char *buf, uint32_t len,
			const struct iio_ch_info *channel, intptr_t attr_id)
{
	struct pqm_desc *desc;
	if (!device)
		return -ENODEV;
	desc = device;
	int data_size = NO_OS_ARRAY_SIZE(pqm_v_consel_available);
	for (int i = 0; i < data_size; i++) {
		if (strcmp(buf, pqm_v_consel_available[i]) == 0) {
			desc->pqm_global_attr[attr_id] = i;
			return len;
		}
	}
	return -EINVAL;
}

/**
 * @brief Read the flicker_model device attribute.
 *
 * @param device    - The iio device structure
 * @param buf       - Buffer to be filled with requested data
 * @param len 	    - Length of the received command buffer in bytes
 * @param channel   - Command channel info
 * @param attr_type - attribute descriptor
 * @return          - The length of the buffer in case of success, negative value otherwise
 */
int read_flicker_model_attr(void *device, char *buf, uint32_t len,
			    const struct iio_ch_info *channel, intptr_t attr_id)
{
	struct pqm_desc *desc;
	if (!device)
		return -ENODEV;
	desc = device;
	if (attr_id < PQM_DEVICE_ATTR_NUMBER) {
		int available_id = desc->pqm_global_attr[attr_id];
		strncpy(buf, pqm_flicker_model_available[available_id], len);
		return strlen(buf);
	} else {
		return -EINVAL;
	}
}

/**
 * @brief Write the flicker_model device attribute.
 *
 * @param device    - The iio device structure
 * @param buf       - Buffer to be filled with requested data
 * @param len 	    - Length of the received command buffer in bytes
 * @param channel   - Command channel info
 * @param attr_type - attribute descriptor
 * @return          - The length of the buffer in case of success, negative value otherwise
 */
int write_flicker_model_attr(void *device, char *buf, uint32_t len,
			     const struct iio_ch_info *channel, intptr_t attr_id)
{
	struct pqm_desc *desc;
	if (!device)
		return -ENODEV;
	desc = device;
	int data_size = NO_OS_ARRAY_SIZE(pqm_flicker_model_available);
	for (int i = 0; i < data_size; i++) {
		if (strcmp(buf, pqm_flicker_model_available[i]) == 0) {
			desc->pqm_global_attr[attr_id] = i;
			return len;
		}
	}
	return -EINVAL;
}

/**
 * @brief Read the nominal_frequency device attribute.
 *
 * @param device    - The iio device structure
 * @param buf       - Buffer to be filled with requested data
 * @param len 	    - Length of the received command buffer in bytes
 * @param channel   - Command channel info
 * @param attr_type - attribute descriptor
 * @return          - The length of the buffer in case of success, negative value otherwise
 */
int read_nominal_freq_attr(void *device, char *buf, uint32_t len,
			   const struct iio_ch_info *channel, intptr_t attr_id)
{
	struct pqm_desc *desc;
	if (!device)
		return -ENODEV;
	desc = device;
	if (attr_id < PQM_DEVICE_ATTR_NUMBER) {
		int available_id = desc->pqm_global_attr[attr_id];
		strncpy(buf, pqm_nominal_frequency_available[available_id], len);
		return strlen(buf);
	} else {
		return -EINVAL;
	}
}

/**
 * @brief Write the nominal_frequency device attribute.
 *
 * @param device    - The iio device structure
 * @param buf       - Buffer to be filled with requested data
 * @param len 	    - Length of the received command buffer in bytes
 * @param channel   - Command channel info
 * @param attr_type - attribute descriptor
 * @return          - The length of the buffer in case of success, negative value otherwise
 */
int write_nominal_freq_attr(void *device, char *buf, uint32_t len,
			    const struct iio_ch_info *channel, intptr_t attr_id)
{
	struct pqm_desc *desc;
	if (!device)
		return -ENODEV;
	desc = device;
	int data_size = NO_OS_ARRAY_SIZE(pqm_nominal_frequency_available);
	for (int i = 0; i < data_size; i++) {
		if (strcmp(buf, pqm_nominal_frequency_available[i]) == 0) {
			desc->pqm_global_attr[attr_id] = i;
			return len;
		}
	}
	return -EINVAL;
}

/**
 * @brief Read a pqm device attribute.
 *
 * @param device    - The iio device structure
 * @param buf       - Buffer to be filled with requested data
 * @param len 	    - Length of the received command buffer in bytes
 * @param channel   - Command channel info
 * @param attr_type - attribute descriptor
 * @return          - The length of the buffer in case of success, negative value otherwise
 */
int read_pqm_attr(void *device, char *buf, uint32_t len,
		  const struct iio_ch_info *channel, intptr_t attr_id)
{
	struct pqm_desc *desc;
	if (!device)
		return -ENODEV;
	desc = device;
	if (attr_id < PQM_DEVICE_ATTR_NUMBER) {
		return snprintf(buf, len, "%" PRIu32 "", desc->pqm_global_attr[attr_id]);
	} else {
		return -EINVAL;
	}
}

/**
 * @brief Write a pqm device attribute.
 *
 * @param device    - The iio device structure
 * @param buf       - Buffer to be filled with requested data
 * @param len 	    - Length of the received command buffer in bytes
 * @param channel   - Command channel info
 * @param attr_type - attribute descriptor
 * @return          - The length of the buffer in case of success, negative value otherwise
 */
int write_pqm_attr(void *device, char *buf, uint32_t len,
		   const struct iio_ch_info *channel, intptr_t attr_id)
{
	struct pqm_desc *desc;
	uint32_t value = no_os_str_to_uint32(buf);

	if (!device)
		return -ENODEV;
	desc = device;
	if (attr_id < PQM_DEVICE_ATTR_NUMBER) {
		desc->pqm_global_attr[attr_id] = value;
		return len;
	} else {
		return -EINVAL;
	}
}

/**
 * @brief Read a channel attribute.
 *
 * @param device    - The iio device structure
 * @param buf       - Buffer to be filled with requested data
 * @param len 	    - Length of the received command buffer in bytes
 * @param channel   - Command channel info
 * @param attr_type - attribute descriptor
 * @return          - The length of the buffer in case of success, negative value otherwise
 */
int read_ch_attr(void *device, char *buf, uint32_t len,
		 const struct iio_ch_info *channel, intptr_t attr_id)
{
	struct pqm_desc *desc;

	if (!device)
		return -ENODEV;
	desc = device;
	if (attr_id >= MAX_CH_ATTRS) {
		return -EINVAL;
	}
	switch (channel->type) {
	case IIO_VOLTAGE:
		return snprintf(buf, len, "%" PRIu32 "",
				desc->pqm_ch_attr[channel->ch_num][attr_id]);
	case IIO_CURRENT:
		return snprintf(buf, len, "%" PRIu32 "",
				desc->pqm_ch_attr[channel->ch_num + VOLTAGE_CH_NUMBER][attr_id]);
	default:
		return -EINVAL;
	}
	return -EINVAL;
}

/**
 * @brief function for reading samples from the device.
 * @param dev_data  - The iio device data structure.
 * @return the number of read samples.
 */
int32_t read_samples(struct iio_device_data *dev_data)
{
	struct pqm_desc *desc;
	uint32_t k = 0;
	uint32_t ch = -1;
	uint32_t buff[TOTAL_PQM_CHANNELS];
	uint32_t i;
	uint32_t *ch_buf_ptr;

	if (!dev_data)
		return -ENODEV;

	desc = (struct pqm_desc *)dev_data->dev;

	if (desc->ext_buff == NULL) {
		int offset_per_ch = TOTAL_PQM_CHANNELS;
		for (i = 0; i < dev_data->buffer->size / dev_data->buffer->bytes_per_scan;
		     i++) {
			while (get_next_ch_idx(desc->active_ch, ch, &ch))
				buff[k++] = sine_lut[(ch + i * offset_per_ch) % NO_OS_ARRAY_SIZE(sine_lut)];
			k = 0;
			iio_buffer_push_scan(dev_data->buffer, buff);
		}
		return dev_data->buffer->size / dev_data->buffer->bytes_per_scan;
	}

	for (i = 0; i < dev_data->buffer->size / dev_data->buffer->bytes_per_scan;
	     i++) {
		while (get_next_ch_idx(desc->active_ch, ch, &ch)) {
			ch_buf_ptr = (uint32_t *)desc->ext_buff + (ch * desc->ext_buff_len);
			buff[k++] = ch_buf_ptr[i];
		}
		k = 0;
		iio_buffer_push_scan(dev_data->buffer, buff);
	}
	return dev_data->buffer->size / dev_data->buffer->bytes_per_scan;
}

/**
 * @brief Handles trigger: reads one data-set and writes it to the buffer.
 *
 * @param dev_data  - The iio device data structure.
 *
 * @return ret - Result of the handling procedure. In case of success, the size
 * 				 of the written data is returned.
 */
int32_t pqm_trigger_handler(struct iio_device_data *dev_data)
{
	struct pqm_desc *desc;
	uint32_t k = 0;
	uint32_t ch = -1;
	uint32_t buff[TOTAL_PQM_CHANNELS];
	static uint32_t i = 0;
	uint32_t *ch_buf_ptr;

	if (!dev_data)
		return -EINVAL;

	desc = (struct pqm_desc *)dev_data->dev;

	if(desc->ext_buff == NULL) {
		int offset_per_ch = TOTAL_PQM_CHANNELS;
		while(get_next_ch_idx(desc->active_ch, ch, &ch))
			buff[k++] = sine_lut[(ch + i * offset_per_ch ) % NO_OS_ARRAY_SIZE(sine_lut)];
		if (i == NO_OS_ARRAY_SIZE(sine_lut))
			i = 0;
		else
			i++;

		return iio_buffer_push_scan(dev_data->buffer, buff);
	}

	while(get_next_ch_idx(desc->active_ch, ch, &ch)) {
		ch_buf_ptr = (uint32_t*)desc->ext_buff + (ch * desc->ext_buff_len);
		buff[k++] = ch_buf_ptr[i];
	}
	if (i == (desc->ext_buff_len - 1))
		i = 0;
	else
		i++;

	return iio_buffer_push_scan(dev_data->buffer, buff);
}

struct iio_attribute voltage_pqm_attributes[] = {
	{
		.name = "rms",
		.show = read_ch_attr,
		.priv = 0,
	},
	{
		.name = "angle",
		.show = read_ch_attr,
		.priv = 1,
	},
	{
		.name = "deviation_under",
		.show = read_ch_attr,
		.priv = 2,
	},
	{
		.name = "deviation_over",
		.show = read_ch_attr,
		.priv = 3,
	},
	{
		.name = "pinst",
		.show = read_ch_attr,
		.priv = 4,
	},
	{
		.name = "pst",
		.show = read_ch_attr,
		.priv = 5,
	},
	{
		.name = "plt",
		.show = read_ch_attr,
		.priv = 6,
	},
	{
		.name = "thd",
		.show = read_ch_attr,
		.priv = 7,
	},
	{
		.name = "harmonics",
		.show = read_ch_attr,
		.priv = 8,
	},
	{
		.name = "raw",
		.show = read_ch_attr,
		.priv = 9,
	},
	END_ATTRIBUTES_ARRAY,
};

struct iio_attribute current_pqm_attributes[] = {

	{
		.name = "rms",
		.show = read_ch_attr,
		.priv = 0,
	},
	{
		.name = "angle",
		.show = read_ch_attr,
		.priv = 1,
	},
	{
		.name = "thd",
		.show = read_ch_attr,
		.priv = 2,
	},
	{
		.name = "harmonics",
		.show = read_ch_attr,
		.priv = 3,
	},
	{
		.name = "raw",
		.show = read_ch_attr,
		.priv = 4,
	},
	END_ATTRIBUTES_ARRAY,
};

struct iio_attribute global_pqm_attributes[] = {
	{
		.name = "u2",
		.show = read_pqm_attr,
		.priv = 0,
	},
	{
		.name = "u0",
		.show = read_pqm_attr,
		.priv = 1,
	},
	{
		.name = "sneg_voltage",
		.show = read_pqm_attr,
		.priv = 2,
	},
	{
		.name = "spos_voltage",
		.show = read_pqm_attr,
		.priv = 3,
	},
	{
		.name = "szro_voltage",
		.show = read_pqm_attr,
		.priv = 4,
	},
	{
		.name = "i2",
		.show = read_pqm_attr,
		.priv = 5,
	},
	{
		.name = "i0",
		.show = read_pqm_attr,
		.priv = 6,
	},
	{
		.name = "sneg_current",
		.show = read_pqm_attr,
		.priv = 7,
	},
	{
		.name = "spos_current",
		.show = read_pqm_attr,
		.priv = 8,
	},
	{
		.name = "szro_current",
		.show = read_pqm_attr,
		.priv = 9,
	},
	{
		.name = "nominal_voltage",
		.show = read_pqm_attr,
		.store = write_pqm_attr,
		.priv = 10,
	},
	{
		.name = "voltage_scale",
		.show = read_pqm_attr,
		.store = write_pqm_attr,
		.priv = 11,
	},
	{
		.name = "current_scale",
		.show = read_pqm_attr,
		.store = write_pqm_attr,
		.priv = 12,
	},
	{
		.name = "i_consel_en",
		.show = read_pqm_attr,
		.store = write_pqm_attr,
		.priv = 13,
	},
	{
		.name = "dip_threshold",
		.show = read_pqm_attr,
		.store = write_pqm_attr,
		.priv = 14,
	},
	{
		.name = "dip_hysteresis",
		.show = read_pqm_attr,
		.store = write_pqm_attr,
		.priv = 15,
	},
	{
		.name = "swell_threshold",
		.show = read_pqm_attr,
		.store = write_pqm_attr,
		.priv = 16,
	},
	{
		.name = "swell_hysteresis",
		.show = read_pqm_attr,
		.store = write_pqm_attr,
		.priv = 17,
	},
	{
		.name = "intrp_threshold",
		.show = read_pqm_attr,
		.store = write_pqm_attr,
		.priv = 18,
	},
	{
		.name = "intrp_hysteresis",
		.show = read_pqm_attr,
		.store = write_pqm_attr,
		.priv = 19,
	},
	{
		.name = "rvc_threshold",
		.show = read_pqm_attr,
		.store = write_pqm_attr,
		.priv = 20,
	},
	{
		.name = "rvc_hysteresis",
		.show = read_pqm_attr,
		.store = write_pqm_attr,
		.priv = 21,
	},
	{
		.name = "msv_carrier_frequency",
		.show = read_pqm_attr,
		.store = write_pqm_attr,
		.priv = 22,
	},
	{
		.name = "msv_record_length",
		.show = read_pqm_attr,
		.store = write_pqm_attr,
		.priv = 23,
	},
	{
		.name = "msv_threshold",
		.show = read_pqm_attr,
		.store = write_pqm_attr,
		.priv = 24,
	},
	{
		.name = "sampling_frequency",
		.show = read_pqm_attr,
		.store = write_pqm_attr,
		.priv = 25,
	},
	{
		.name = "v_consel",
		.show = read_v_consel_attr,
		.store = write_v_consel_attr,
		.priv = 26,
	},
	{
		.name = "v_consel_available",
		.show = read_available_values,
		.priv = V_CONSEL,
	},
	{
		.name = "flicker_model",
		.show = read_flicker_model_attr,
		.store = write_flicker_model_attr,
		.priv = 27,
	},
	{
		.name = "flicker_model_available",
		.show = read_available_values,
		.priv = FLICKER_MODEL,
	},
	{
		.name = "nominal_frequency",
		.show = read_nominal_freq_attr,
		.store = write_nominal_freq_attr,
		.priv = 28,
	},
	{
		.name = "nominal_frequency_available",
		.show = read_available_values,
		.store = write_nominal_freq_attr,
		.priv = NOMINAL_FREQUENCY,
	},
	END_ATTRIBUTES_ARRAY,
};

struct scan_type pqm_scan_type = {
	.sign = 'u',
	.realbits = 24,
	.storagebits = 32,
	.shift = 0,
	.is_big_endian = true
};

#define PQM_VOLTAGE_CHANNEL(_idx, _scan_idx, _name) \
	{                                               \
		.name = _name,                              \
		.ch_type = IIO_VOLTAGE,                     \
		.channel = _idx,                            \
		.scan_index = _scan_idx,                    \
		.indexed = true,                            \
		.scan_type = &pqm_scan_type,                \
		.attributes = voltage_pqm_attributes,       \
		.ch_out = false                             \
	}

#define PQM_CURRENT_CHANNEL(_idx, _scan_idx, _name) \
	{                                               \
		.name = _name,                              \
		.ch_type = IIO_CURRENT,                     \
		.channel = _idx,                            \
		.scan_index = _scan_idx,                    \
		.indexed = true,                            \
		.scan_type = &pqm_scan_type,                \
		.attributes = current_pqm_attributes,       \
		.ch_out = false                             \
	}

static struct iio_channel iio_pqm_channels[] = {
	PQM_VOLTAGE_CHANNEL(0, 0, "ua"),
	PQM_VOLTAGE_CHANNEL(1, 1, "ub"),
	PQM_VOLTAGE_CHANNEL(2, 2, "uc"),
	PQM_CURRENT_CHANNEL(0, 3, "ia"),
	PQM_CURRENT_CHANNEL(1, 4, "ib"),
	PQM_CURRENT_CHANNEL(2, 5, "ic"),
	PQM_CURRENT_CHANNEL(3, 6, "in"),
};

struct iio_device pqm_iio_descriptor = {
	.num_ch = TOTAL_PQM_CHANNELS,
	.channels = iio_pqm_channels,
	.attributes = global_pqm_attributes,
	.debug_attributes = NULL,
	.buffer_attributes = NULL,
	.pre_enable = update_pqm_channels,
	.post_disable = close_pqm_channels,
	.trigger_handler = (int32_t (*)())pqm_trigger_handler,
	.submit = (int32_t (*)())read_samples,
};