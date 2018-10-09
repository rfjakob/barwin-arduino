	.file	"wiring_pulse.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.stabs	"/opt/arduino-1.8.7/hardware/arduino/avr/cores/arduino/wiring_pulse.c",100,0,2,.Ltext0
	.text
.Ltext0:
	.stabs	"gcc2_compiled.",60,0,0,0
	.stabs	"int:t(0,1)=r(0,1);-32768;32767;",128,0,0,0
	.stabs	"char:t(0,2)=r(0,2);0;127;",128,0,0,0
	.stabs	"long int:t(0,3)=r(0,3);-2147483648;2147483647;",128,0,0,0
	.stabs	"unsigned int:t(0,4)=r(0,4);0;65535;",128,0,0,0
	.stabs	"long unsigned int:t(0,5)=r(0,5);0;4294967295;",128,0,0,0
	.stabs	"__int128:t(0,6)=r(0,6);0;-1;",128,0,0,0
	.stabs	"__int128 unsigned:t(0,7)=r(0,7);0;-1;",128,0,0,0
	.stabs	"long long int:t(0,8)=r(0,8);-9223372036854775808;9223372036854775807;",128,0,0,0
	.stabs	"long long unsigned int:t(0,9)=r(0,9);0;-1;",128,0,0,0
	.stabs	"short int:t(0,10)=r(0,10);-32768;32767;",128,0,0,0
	.stabs	"short unsigned int:t(0,11)=r(0,11);0;65535;",128,0,0,0
	.stabs	"signed char:t(0,12)=r(0,12);-128;127;",128,0,0,0
	.stabs	"unsigned char:t(0,13)=r(0,13);0;255;",128,0,0,0
	.stabs	"float:t(0,14)=r(0,1);4;0;",128,0,0,0
	.stabs	"double:t(0,15)=r(0,1);4;0;",128,0,0,0
	.stabs	"long double:t(0,16)=r(0,1);4;0;",128,0,0,0
	.stabs	"_Float32:t(0,17)=r(0,1);4;0;",128,0,0,0
	.stabs	"short _Fract:t(0,18)=r(0,1);1;0;",128,0,0,0
	.stabs	"_Fract:t(0,19)=r(0,1);2;0;",128,0,0,0
	.stabs	"long _Fract:t(0,20)=r(0,1);4;0;",128,0,0,0
	.stabs	"long long _Fract:t(0,21)=r(0,1);8;0;",128,0,0,0
	.stabs	"unsigned short _Fract:t(0,22)=r(0,1);1;0;",128,0,0,0
	.stabs	"unsigned _Fract:t(0,23)=r(0,1);2;0;",128,0,0,0
	.stabs	"unsigned long _Fract:t(0,24)=r(0,1);4;0;",128,0,0,0
	.stabs	"unsigned long long _Fract:t(0,25)=r(0,1);8;0;",128,0,0,0
	.stabs	"_Sat short _Fract:t(0,26)=r(0,1);1;0;",128,0,0,0
	.stabs	"_Sat _Fract:t(0,27)=r(0,1);2;0;",128,0,0,0
	.stabs	"_Sat long _Fract:t(0,28)=r(0,1);4;0;",128,0,0,0
	.stabs	"_Sat long long _Fract:t(0,29)=r(0,1);8;0;",128,0,0,0
	.stabs	"_Sat unsigned short _Fract:t(0,30)=r(0,1);1;0;",128,0,0,0
	.stabs	"_Sat unsigned _Fract:t(0,31)=r(0,1);2;0;",128,0,0,0
	.stabs	"_Sat unsigned long _Fract:t(0,32)=r(0,1);4;0;",128,0,0,0
	.stabs	"_Sat unsigned long long _Fract:t(0,33)=r(0,1);8;0;",128,0,0,0
	.stabs	"short _Accum:t(0,34)=r(0,1);2;0;",128,0,0,0
	.stabs	"_Accum:t(0,35)=r(0,1);4;0;",128,0,0,0
	.stabs	"long _Accum:t(0,36)=r(0,1);8;0;",128,0,0,0
	.stabs	"long long _Accum:t(0,37)=r(0,1);8;0;",128,0,0,0
	.stabs	"unsigned short _Accum:t(0,38)=r(0,1);2;0;",128,0,0,0
	.stabs	"unsigned _Accum:t(0,39)=r(0,1);4;0;",128,0,0,0
	.stabs	"unsigned long _Accum:t(0,40)=r(0,1);8;0;",128,0,0,0
	.stabs	"unsigned long long _Accum:t(0,41)=r(0,1);8;0;",128,0,0,0
	.stabs	"_Sat short _Accum:t(0,42)=r(0,1);2;0;",128,0,0,0
	.stabs	"_Sat _Accum:t(0,43)=r(0,1);4;0;",128,0,0,0
	.stabs	"_Sat long _Accum:t(0,44)=r(0,1);8;0;",128,0,0,0
	.stabs	"_Sat long long _Accum:t(0,45)=r(0,1);8;0;",128,0,0,0
	.stabs	"_Sat unsigned short _Accum:t(0,46)=r(0,1);2;0;",128,0,0,0
	.stabs	"_Sat unsigned _Accum:t(0,47)=r(0,1);4;0;",128,0,0,0
	.stabs	"_Sat unsigned long _Accum:t(0,48)=r(0,1);8;0;",128,0,0,0
	.stabs	"_Sat unsigned long long _Accum:t(0,49)=r(0,1);8;0;",128,0,0,0
	.stabs	"void:t(0,50)=(0,50)",128,0,0,0
	.stabs	"/opt/arduino-1.8.7/hardware/arduino/avr/cores/arduino/wiring_private.h",130,0,0,0
	.stabs	"/usr/avr/include/avr/io.h",130,0,0,0
	.stabs	"/usr/avr/include/avr/sfr_defs.h",130,0,0,0
	.stabs	"/usr/avr/include/inttypes.h",130,0,0,0
	.stabs	"/usr/lib/gcc/avr/8.2.0/include/stdint.h",130,0,0,0
	.stabs	"/usr/avr/include/stdint.h",130,0,0,0
	.stabs	"int8_t:t(6,1)=(0,12)",128,0,0,0
	.stabs	"uint8_t:t(6,2)=(0,13)",128,0,0,0
	.stabs	"int16_t:t(6,3)=(0,1)",128,0,0,0
	.stabs	"uint16_t:t(6,4)=(0,4)",128,0,0,0
	.stabs	"int32_t:t(6,5)=(0,3)",128,0,0,0
	.stabs	"uint32_t:t(6,6)=(0,5)",128,0,0,0
	.stabs	"int64_t:t(6,7)=(0,8)",128,0,0,0
	.stabs	"uint64_t:t(6,8)=(0,9)",128,0,0,0
	.stabs	"intptr_t:t(6,9)=(6,3)",128,0,0,0
	.stabs	"uintptr_t:t(6,10)=(6,4)",128,0,0,0
	.stabs	"int_least8_t:t(6,11)=(6,1)",128,0,0,0
	.stabs	"uint_least8_t:t(6,12)=(6,2)",128,0,0,0
	.stabs	"int_least16_t:t(6,13)=(6,3)",128,0,0,0
	.stabs	"uint_least16_t:t(6,14)=(6,4)",128,0,0,0
	.stabs	"int_least32_t:t(6,15)=(6,5)",128,0,0,0
	.stabs	"uint_least32_t:t(6,16)=(6,6)",128,0,0,0
	.stabs	"int_least64_t:t(6,17)=(6,7)",128,0,0,0
	.stabs	"uint_least64_t:t(6,18)=(6,8)",128,0,0,0
	.stabs	"int_fast8_t:t(6,19)=(6,1)",128,0,0,0
	.stabs	"uint_fast8_t:t(6,20)=(6,2)",128,0,0,0
	.stabs	"int_fast16_t:t(6,21)=(6,3)",128,0,0,0
	.stabs	"uint_fast16_t:t(6,22)=(6,4)",128,0,0,0
	.stabs	"int_fast32_t:t(6,23)=(6,5)",128,0,0,0
	.stabs	"uint_fast32_t:t(6,24)=(6,6)",128,0,0,0
	.stabs	"int_fast64_t:t(6,25)=(6,7)",128,0,0,0
	.stabs	"uint_fast64_t:t(6,26)=(6,8)",128,0,0,0
	.stabs	"intmax_t:t(6,27)=(6,7)",128,0,0,0
	.stabs	"uintmax_t:t(6,28)=(6,8)",128,0,0,0
	.stabn	162,0,0,0
	.stabn	162,0,0,0
	.stabs	"int_farptr_t:t(4,1)=(6,5)",128,0,0,0
	.stabs	"uint_farptr_t:t(4,2)=(6,6)",128,0,0,0
	.stabn	162,0,0,0
	.stabn	162,0,0,0
	.stabs	"/usr/avr/include/avr/fuse.h",130,0,0,0
	.stabs	"__fuse_t:t(7,1)=(7,2)=s3low:(0,13),0,8;high:(0,13),8,8;extended:(0,13),16,8;;",128,0,0,0
	.stabn	162,0,0,0
	.stabn	162,0,0,0
	.stabs	"/usr/avr/include/stdio.h",130,0,0,0
	.stabs	"/usr/lib/gcc/avr/8.2.0/include/stdarg.h",130,0,0,0
	.stabs	"__gnuc_va_list:t(9,1)=(9,2)=*(0,50)",128,0,0,0
	.stabs	"va_list:t(9,3)=(9,1)",128,0,0,0
	.stabn	162,0,0,0
	.stabs	"/usr/lib/gcc/avr/8.2.0/include/stddef.h",130,0,0,0
	.stabs	"size_t:t(10,1)=(0,4)",128,0,0,0
	.stabn	162,0,0,0
	.stabs	"__file:T(8,1)=s14buf:(8,2)=*(0,2),0,16;unget:(0,13),16,8;flags:(6,2),24,8;size:(0,1),32,16;len:(0,1),48,16;put:(8,3)=*(8,4)=f(0,1),64,16;get:(8,5)=*(8,6)=f(0,1),80,16;udata:(9,2),96,16;;",128,0,0,0
	.stabs	"FILE:t(8,7)=(8,1)",128,0,0,0
	.stabs	"fpos_t:t(8,8)=(0,8)",128,0,0,0
	.stabn	162,0,0,0
	.stabs	"/opt/arduino-1.8.7/hardware/arduino/avr/cores/arduino/Arduino.h",130,0,0,0
	.stabs	"/usr/avr/include/stdlib.h",130,0,0,0
	.stabs	"/usr/lib/gcc/avr/8.2.0/include/stddef.h",130,0,0,0
	.stabs	"wchar_t:t(13,1)=(0,1)",128,0,0,0
	.stabn	162,0,0,0
	.stabs	"div_t:t(12,1)=(12,2)=s4quot:(0,1),0,16;rem:(0,1),16,16;;",128,0,0,0
	.stabs	"ldiv_t:t(12,3)=(12,4)=s8quot:(0,3),0,32;rem:(0,3),32,32;;",128,0,0,0
	.stabs	"__compar_fn_t:t(12,5)=(12,6)=*(12,7)=f(0,1)",128,0,0,0
	.stabn	162,0,0,0
	.stabs	"word:t(11,1)=(0,4)",128,0,0,0
	.stabs	"boolean:t(11,2)=(11,3)=eFalse:0,True:1,;",128,0,0,0
	.stabs	"byte:t(11,4)=(6,2)",128,0,0,0
	.stabn	162,0,0,0
	.stabs	"voidFuncPtr:t(1,1)=(1,2)=*(1,3)=f(0,50)",128,0,0,0
	.stabn	162,0,0,0
	.stabs	"A11:c=i29",128,0,0,0
	.stabs	"A10:c=i28",128,0,0,0
	.stabs	"A9:c=i27",128,0,0,0
	.stabs	"A8:c=i26",128,0,0,0
	.stabs	"A7:c=i25",128,0,0,0
	.stabs	"A6:c=i24",128,0,0,0
	.stabs	"A5:c=i23",128,0,0,0
	.stabs	"A4:c=i22",128,0,0,0
	.stabs	"A3:c=i21",128,0,0,0
	.stabs	"A2:c=i20",128,0,0,0
	.stabs	"A1:c=i19",128,0,0,0
	.stabs	"A0:c=i18",128,0,0,0
	.stabs	"SCK:c=i15",128,0,0,0
	.stabs	"MISO:c=i14",128,0,0,0
	.stabs	"MOSI:c=i16",128,0,0,0
	.stabs	"SS:c=i17",128,0,0,0
	.stabs	"SCL:c=i3",128,0,0,0
	.stabs	"SDA:c=i2",128,0,0,0
	.section	.text.pulseIn,"ax",@progbits
	.stabs	"pulseIn:F(0,5)",36,0,0,pulseIn
	.stabs	"pin:P(6,2)",64,0,0,24
	.stabs	"state:P(6,2)",64,0,0,22
	.stabs	"timeout:P(0,5)",64,0,0,18
.global	pulseIn
	.type	pulseIn, @function
pulseIn:
	.stabn	68,0,34,.LM0-.LFBB1
.LM0:
.LFBB1:
	push r16
	push r17
/* prologue: function */
/* frame size = 0 */
/* stack size = 2 */
.L__stack_usage = 2
.LBB2:
	.stabn	68,0,38,.LM1-.LFBB1
.LM1:
	ldi r25,0
	movw r30,r24
	subi r30,lo8(-(digital_pin_to_bit_mask_PGM))
	sbci r31,hi8(-(digital_pin_to_bit_mask_PGM))
/* #APP */
 ;  38 "/opt/arduino-1.8.7/hardware/arduino/avr/cores/arduino/wiring_pulse.c" 1
	lpm r23, Z
	
 ;  0 "" 2
/* #NOAPP */
.LBE2:
.LBB3:
	.stabn	68,0,39,.LM2-.LFBB1
.LM2:
	movw r30,r24
	subi r30,lo8(-(digital_pin_to_port_PGM))
	sbci r31,hi8(-(digital_pin_to_port_PGM))
/* #APP */
 ;  39 "/opt/arduino-1.8.7/hardware/arduino/avr/cores/arduino/wiring_pulse.c" 1
	lpm r30, Z
	
 ;  0 "" 2
/* #NOAPP */
.LBE3:
	.stabn	68,0,40,.LM3-.LFBB1
.LM3:
	cpse r22,__zero_reg__
	mov r22,r23
.L2:
.LBB4:
	.stabn	68,0,46,.LM4-.LFBB1
.LM4:
	ldi r31,0
	lsl r30
	rol r31
	subi r30,lo8(-(port_to_input_PGM))
	sbci r31,hi8(-(port_to_input_PGM))
/* #APP */
 ;  46 "/opt/arduino-1.8.7/hardware/arduino/avr/cores/arduino/wiring_pulse.c" 1
	lpm r24, Z+
	lpm r25, Z
	
 ;  0 "" 2
/* #NOAPP */
.LBE4:
	.stabn	68,0,44,.LM5-.LFBB1
.LM5:
	movw r16,r18
	movw r18,r20
	andi r19,15
	.stabn	68,0,46,.LM6-.LFBB1
.LM6:
	mov r20,r22
	mov r22,r23
	call countPulseASM
	.stabn	68,0,49,.LM7-.LFBB1
.LM7:
	cp r22,__zero_reg__
	cpc r23,__zero_reg__
	cpc r24,__zero_reg__
	cpc r25,__zero_reg__
	breq .L1
	.stabn	68,0,50,.LM8-.LFBB1
.LM8:
	subi r22,-1
	sbci r23,-1
	sbci r24,-1
	sbci r25,-1
	andi r25,15
.L1:
/* epilogue start */
	.stabn	68,0,53,.LM9-.LFBB1
.LM9:
	pop r17
	pop r16
	ret
	.size	pulseIn, .-pulseIn
	.stabs	"width:r(0,5)",64,0,0,22
	.stabn	192,0,0,.LFBB1-.LFBB1
	.stabs	"__result:r(6,2)",64,0,0,23
	.stabn	192,0,0,.LBB2-.LFBB1
	.stabn	224,0,0,.LBE2-.LFBB1
	.stabs	"__result:r(6,2)",64,0,0,30
	.stabn	192,0,0,.LBB3-.LFBB1
	.stabn	224,0,0,.LBE3-.LFBB1
	.stabs	"__addr16:r(6,4)",64,0,0,30
	.stabn	192,0,0,.LBB4-.LFBB1
	.stabn	224,0,0,.LBE4-.LFBB1
	.stabn	224,0,0,.Lscope1-.LFBB1
.Lscope1:
	.section	.text.pulseInLong,"ax",@progbits
	.stabs	"pulseInLong:F(0,5)",36,0,0,pulseInLong
	.stabs	"pin:P(6,2)",64,0,0,24
	.stabs	"state:P(6,2)",64,0,0,16
	.stabs	"timeout:P(0,5)",64,0,0,8
.global	pulseInLong
	.type	pulseInLong, @function
pulseInLong:
	.stabn	68,0,64,.LM10-.LFBB2
.LM10:
.LFBB2:
	push r4
	push r5
	push r6
	push r7
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	push r16
	push r17
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 16 */
.L__stack_usage = 16
	mov r16,r22
	movw r8,r18
	movw r10,r20
.LBB5:
	.stabn	68,0,68,.LM11-.LFBB2
.LM11:
	ldi r25,0
	movw r30,r24
	subi r30,lo8(-(digital_pin_to_bit_mask_PGM))
	sbci r31,hi8(-(digital_pin_to_bit_mask_PGM))
/* #APP */
 ;  68 "/opt/arduino-1.8.7/hardware/arduino/avr/cores/arduino/wiring_pulse.c" 1
	lpm r17, Z
	
 ;  0 "" 2
/* #NOAPP */
.LBE5:
.LBB6:
	.stabn	68,0,69,.LM12-.LFBB2
.LM12:
	subi r24,lo8(-(digital_pin_to_port_PGM))
	sbci r25,hi8(-(digital_pin_to_port_PGM))
	movw r30,r24
/* #APP */
 ;  69 "/opt/arduino-1.8.7/hardware/arduino/avr/cores/arduino/wiring_pulse.c" 1
	lpm r28, Z
	
 ;  0 "" 2
/* #NOAPP */
.LBE6:
	.stabn	68,0,70,.LM13-.LFBB2
.LM13:
	cpse r22,__zero_reg__
	mov r16,r17
.L9:
	.stabn	68,0,72,.LM14-.LFBB2
.LM14:
	call micros
	movw r12,r22
	movw r14,r24
.LBB7:
	.stabn	68,0,75,.LM15-.LFBB2
.LM15:
	ldi r29,0
	lsl r28
	rol r29
	subi r28,lo8(-(port_to_input_PGM))
	sbci r29,hi8(-(port_to_input_PGM))
.L10:
	.stabn	68,0,75,.LM16-.LFBB2
.LM16:
	movw r30,r28
/* #APP */
 ;  75 "/opt/arduino-1.8.7/hardware/arduino/avr/cores/arduino/wiring_pulse.c" 1
	lpm r26, Z+
	lpm r27, Z
	
 ;  0 "" 2
/* #NOAPP */
.LBE7:
	.stabn	68,0,75,.LM17-.LFBB2
.LM17:
	ld r24,X
	.stabn	68,0,75,.LM18-.LFBB2
.LM18:
	and r24,r17
	.stabn	68,0,75,.LM19-.LFBB2
.LM19:
	cp r24,r16
	breq .L12
.L13:
.LBB8:
	.stabn	68,0,81,.LM20-.LFBB2
.LM20:
	movw r30,r28
/* #APP */
 ;  81 "/opt/arduino-1.8.7/hardware/arduino/avr/cores/arduino/wiring_pulse.c" 1
	lpm r26, Z+
	lpm r27, Z
	
 ;  0 "" 2
/* #NOAPP */
.LBE8:
	.stabn	68,0,81,.LM21-.LFBB2
.LM21:
	ld r24,X
	.stabn	68,0,81,.LM22-.LFBB2
.LM22:
	and r24,r17
	.stabn	68,0,81,.LM23-.LFBB2
.LM23:
	cpse r24,r16
	rjmp .L15
	.stabn	68,0,86,.LM24-.LFBB2
.LM24:
	call micros
	movw r4,r22
	movw r6,r24
.L16:
.LBB9:
	.stabn	68,0,88,.LM25-.LFBB2
.LM25:
	movw r30,r28
/* #APP */
 ;  88 "/opt/arduino-1.8.7/hardware/arduino/avr/cores/arduino/wiring_pulse.c" 1
	lpm r26, Z+
	lpm r27, Z
	
 ;  0 "" 2
/* #NOAPP */
.LBE9:
	.stabn	68,0,88,.LM26-.LFBB2
.LM26:
	ld r24,X
	.stabn	68,0,88,.LM27-.LFBB2
.LM27:
	and r24,r17
	.stabn	68,0,88,.LM28-.LFBB2
.LM28:
	cp r24,r16
	breq .L17
	.stabn	68,0,92,.LM29-.LFBB2
.LM29:
	call micros
	.stabn	68,0,92,.LM30-.LFBB2
.LM30:
	sub r22,r4
	sbc r23,r5
	sbc r24,r6
	sbc r25,r7
	rjmp .L8
.L12:
	.stabn	68,0,76,.LM31-.LFBB2
.LM31:
	call micros
	.stabn	68,0,76,.LM32-.LFBB2
.LM32:
	sub r22,r12
	sbc r23,r13
	sbc r24,r14
	sbc r25,r15
	.stabn	68,0,76,.LM33-.LFBB2
.LM33:
	cp r8,r22
	cpc r9,r23
	cpc r10,r24
	cpc r11,r25
	brsh .L10
.L14:
	.stabn	68,0,77,.LM34-.LFBB2
.LM34:
	ldi r22,0
	ldi r23,0
	movw r24,r22
.L8:
/* epilogue start */
	.stabn	68,0,93,.LM35-.LFBB2
.LM35:
	pop r29
	pop r28
	pop r17
	pop r16
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop r7
	pop r6
	pop r5
	pop r4
	ret
.L15:
	.stabn	68,0,82,.LM36-.LFBB2
.LM36:
	call micros
	.stabn	68,0,82,.LM37-.LFBB2
.LM37:
	sub r22,r12
	sbc r23,r13
	sbc r24,r14
	sbc r25,r15
	.stabn	68,0,82,.LM38-.LFBB2
.LM38:
	cp r8,r22
	cpc r9,r23
	cpc r10,r24
	cpc r11,r25
	brlo .+2
	rjmp .L13
	rjmp .L14
.L17:
	.stabn	68,0,89,.LM39-.LFBB2
.LM39:
	call micros
	.stabn	68,0,89,.LM40-.LFBB2
.LM40:
	sub r22,r12
	sbc r23,r13
	sbc r24,r14
	sbc r25,r15
	.stabn	68,0,89,.LM41-.LFBB2
.LM41:
	cp r8,r22
	cpc r9,r23
	cpc r10,r24
	cpc r11,r25
	brlo .+2
	rjmp .L16
	rjmp .L14
	.size	pulseInLong, .-pulseInLong
	.stabs	"startMicros:r(0,5)",64,0,0,12
	.stabs	"start:r(0,5)",64,0,0,4
	.stabn	192,0,0,.LFBB2-.LFBB2
	.stabs	"__result:r(6,2)",64,0,0,17
	.stabn	192,0,0,.LBB5-.LFBB2
	.stabn	224,0,0,.LBE5-.LFBB2
	.stabs	"__result:r(6,2)",64,0,0,28
	.stabn	192,0,0,.LBB6-.LFBB2
	.stabn	224,0,0,.LBE6-.LFBB2
	.stabn	224,0,0,.Lscope2-.LFBB2
.Lscope2:
	.text
	.stabs	"",100,0,0,.Letext0
.Letext0:
	.ident	"GCC: (GNU) 8.2.0"
