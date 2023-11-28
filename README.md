# pqm_no_OS

To construct this project, follow these steps:

1. Refer to the No-OS build guide at https://wiki.analog.com/resources/no-os/build.
2. Duplicate the contents of this repository into a new directory within the project directory obtained from the No-OS repository.
3. Execute the command: "make".
4. Execute the command: "make run".

Network connection: 

1. Execute the "make" and "make run" commands with NO_OS_STATIC_IP=y flag. (This is the fastest way of testing the platform, since it doesn't require the DHCP configuration or DHCP timeout (in case of AutoIP))
2. This will result in using 169.254.97.40 as an IP address.
	In case a link local address is used, the user may need to add a static route if the PC has multiple network interfaces up. If using Linux, this can be done by running the following in a terminal:
	
	`sudo ip route add 169.254.97.40/32 dev if_name`
	`if_name` should be replaced with the name of the network interface the SWIOT1L board is connected on.
