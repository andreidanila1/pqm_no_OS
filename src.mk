LIBRARIES += lwip
TINYIIOD=y

include $(PROJECT)/src/platform/$(PLATFORM)/platform_src.mk

INCS += $(NO-OS)/network/tcp_socket.h
INCS += $(NO-OS)/network/noos_mbedtls_config.h
INCS += $(NO-OS)/network/network_interface.h

SRCS += $(NO-OS)/network/tcp_socket.c

SRCS += $(PROJECT)/src/platform/$(PLATFORM)/main.c

INCS += $(PROJECT)/src/common/app_config.h

INCS += $(PROJECT)/src/common/iio_pqm.h
SRCS += $(PROJECT)/src/common/iio_pqm.c

INCS += $(PROJECT)/src/common/pqm.h
SRCS += $(PROJECT)/src/common/pqm.c

INCS += $(PROJECT)/src/common/common_data.h
SRCS += $(PROJECT)/src/common/common_data.c

INCS += $(PROJECT)/src/platform/platform_includes.h

INCS += $(PROJECT)/src/platform/$(PLATFORM)/parameters.h
SRCS += $(PROJECT)/src/platform/$(PLATFORM)/parameters.c 

SRCS += $(DRIVERS)/api/no_os_uart.c     \
        $(DRIVERS)/api/no_os_spi.c      \
        $(NO-OS)/util/no_os_fifo.c      \
        $(NO-OS)/util/no_os_list.c      \
        $(NO-OS)/util/no_os_util.c      \
        $(NO-OS)/util/no_os_alloc.c     \
        $(NO-OS)/util/no_os_mutex.c     \
        $(DRIVERS)/api/no_os_gpio.c     \
        $(DRIVERS)/api/no_os_i2c.c  

INCS += $(INCLUDE)/no_os_delay.h     \
        $(INCLUDE)/no_os_error.h     \
        $(INCLUDE)/no_os_fifo.h      \
        $(INCLUDE)/no_os_irq.h       \
        $(INCLUDE)/no_os_lf256fifo.h \
        $(INCLUDE)/no_os_list.h      \
        $(INCLUDE)/no_os_timer.h     \
        $(INCLUDE)/no_os_uart.h      \
        $(INCLUDE)/no_os_spi.h       \
        $(INCLUDE)/no_os_util.h      \
        $(INCLUDE)/no_os_alloc.h     \
        $(INCLUDE)/no_os_mutex.h     \
        $(INCLUDE)/no_os_gpio.h      \
        $(INCLUDE)/no_os_i2c.h       \
        $(INCLUDE)/no_os_init.h      \
        $(INCLUDE)/no_os_print_log.h \
        $(INCLUDE)/no_os_units.h 

SRCS += $(PROJECT)/src/pqm_fw.c
INCS += $(PROJECT)/src/pqm_fw.h

INCS += $(INCLUDE)/no_os_crc8.h
INCS += $(DRIVERS)/net/adin1110/adin1110.h
INCS += $(NO-OS)/network/lwip_raw_socket/netdevs/adin1110/lwip_adin1110.h
SRCS += $(NO-OS)/network/lwip_raw_socket/netdevs/adin1110/lwip_adin1110.c
SRCS += $(DRIVERS)/net/adin1110/adin1110.c
SRCS += $(NO-OS)/util/no_os_crc8.c

SRC_DIRS += $(NO-OS)/iio/iio_app

INCS += $(INCLUDE)/no_os_list.h \
        $(PLATFORM_DRIVERS)/$(PLATFORM)_uart.h


