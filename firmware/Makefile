all:
	$(MAKE) -C caboodle
	$(MAKE) -C LEDtable
	$(MAKE) programmed.txt
	
programmed.txt : LEDtable/led_table.bin
	ST-LINK_CLI.exe -P LEDtable/led_table.bin 0x8000000 -Rst
	echo `date` >> programmed.txt