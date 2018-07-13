PHDRS {
	text PT_LOAD FLAGS(7);
}

SECTIONS {
	.text : { * (.text_main); * (.text); }:text = 0
	.rodata : { * (.rodata); }
	.bss : { * (.bss); }
	.data : {
		* (.data);
		. = ALIGN(16);
		* (zImage.data);
	}
}
