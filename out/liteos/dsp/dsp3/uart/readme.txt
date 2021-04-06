1、if you want to change the output uart port, please modify UART_INDEX in the uart.c.
2、if you don't want to use physical uart, please modify the function serial_init and outbyte in the uart.c.
	Modify the function as bellow:
	int serial_init(void)
	{
		return 0;
	}
	
	void outbyte( int c )
	{
		if (c == '\n')
		{
			(void)virt_tty_direct_write((const char *)"\r", 1); /* virtual uart */
		}

		(void)virt_tty_direct_write((const char *)&c, 1); /* virtual uart */
	}
	