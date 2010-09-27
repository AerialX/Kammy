.global Kammy_Stub
.global Kammy_Stub_Address
.global Kammy_Stub_End
Kammy_Stub:
	mflr	%r0
	std		%r0, 0x10(%r1)
	stdu	%r1, -0x80(%r1)
	std		%r2, 0x70(%r1)
Kammy_Stub_Address:
	lis		%r2, 0
	ori		%r2, %r2, 0
	sldi	%r2, %r2, 32
	oris	%r2, %r2, 0
	ori		%r2, %r2, 0
	ld		%r0, 0x00(%r2)
	ld		%r2, 0x08(%r2)
	mtctr	%r0
	bctrl
	ld		%r2, 0x70(%r1)
	addi	%r1, %r1, 0x80
	ld		%r0, 0x10(%r1)
	mtlr	%r0
	blr
Kammy_Stub_End:
	nop
