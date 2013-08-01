SECTIONS {
	. = 0x10008000;
	.text : { * (.text); }
	.data : { * (.data); }
	.rodata : { * (.rodata); }
}
