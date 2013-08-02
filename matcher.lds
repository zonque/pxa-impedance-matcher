SECTIONS {
	.text : { * (.text_main); * (.text); }
	.data : { * (.data); }
	.rodata : { * (.rodata); }
	.bss :
	{
		* (.bss);
	}
}
