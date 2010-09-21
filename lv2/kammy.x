OUTPUT_FORMAT ("binary")

SECTIONS {
	.text : {
		FILL (0)
		
		__text_start = . ;
		*(.init)
		*(.text)
		*(.ctors)
		*(.dtors)
		*(.rodata)
		*(.fini)
		__text_end  = . ;
	}
}
