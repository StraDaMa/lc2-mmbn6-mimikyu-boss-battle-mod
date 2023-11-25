.macro .vorg, gOffset, fOffset
	.if _version == 1
		.org fOffset
	.else
		.org gOffset
	.endif
.endmacro

.macro .vword, usWord, jpWord
	.if _version == 1
		.word jpWord
	.else
		.word usWord
	.endif
.endmacro

.macro .vimport, usFile, jpFile
	.if _version == 1
		.import jpFile
	.else
		.import usFile
	.endif
.endmacro

.macro .vdefinelabel, labelName, gOffset, fOffset
	.if _version == 1
		.definelabel labelName, fOffset
	.else
		.definelabel labelName, gOffset
	.endif
.endmacro
;eof