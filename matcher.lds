PHDRS {
	text PT_LOAD FLAGS(7);
}

SECTIONS {
	.text : { * (.text_main); * (.text); }:text = 0
	.data : { * (.data); }
	.rodata : { * (.rodata); }
	.bss : { * (.bss); }
}
