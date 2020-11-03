	.text
	.globl get_parity
#edi contains n	
get_parity:

	#@TODO: add code here to set eax to 1 iff edi has even parity
	test 	%eax, %eax
	jpe	if_even
	xorl	%eax, %eax
	ret

if_even:
	movl $1, %eax
	ret
	
