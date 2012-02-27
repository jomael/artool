SECTIONS
{
	.rodata :
	{
		. = ALIGN(16) ;
		__beg_tokens = . ;
		*(.token)
		__end_tokens = . ;
	}
}
