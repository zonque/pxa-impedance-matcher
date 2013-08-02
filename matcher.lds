SECTIONS {
	.text : { * (.text_main); * (.text); }
	.data : { * (.data); }
	.rodata : { * (.rodata); }
	.bss :
	{
		* (.bss);
	}
	PROVIDE (__end_of_image = .) ;
}
