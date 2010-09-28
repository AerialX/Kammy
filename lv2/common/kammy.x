OUTPUT_FORMAT ("binary")

SECTIONS {
	.text : {
		FILL (0)
		
		__text_start = . ;
		*(.init)
		*(.text)
		__ctors_start = . ;
		*(.ctors)
		__ctors_end = . ;
		*(.dtors)
		*(.rodata)
		*(.fini)
		__text_end  = . ;
	}
	.bss : {
		__bss_start = . ;
		*(.bss)
		__bss_end = . ;
	}
}
