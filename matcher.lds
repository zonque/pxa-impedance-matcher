SECTIONS {
	. = 0xa0008000;
	.text : { * (.text); }
	.data : { * (.data); }
}
