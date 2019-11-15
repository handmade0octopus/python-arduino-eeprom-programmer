#   Simple code for alternate LED blinking
code = bytearray([
    0xa9, 0xff,         # lda $ff
    0x8d, 0x02, 0x60,   # sta $6002

    0xa9, 0x55,         # lda $55
    0x8d, 0x00, 0x60,   # sta $6000

    0xa9, 0xaa,         # lda $aa
    0x8d, 0x00, 0x60,   # sta $6000

    0x4c, 0x05, 0x80,   #jmp $8005
    ])

rom = code + bytearray([0xea] * (1024 - len(code)))

#   Swap the code from below if you want to have full ROM
#   for your first programming.
#   Otherwise I suggest programming only 1024 bytes to make it faster
#   Full 32KB takes over 5 minutes!


#   rom = code + bytearray([0xea] * (32768 - len(code)))
#   rom[0x7ffc] = 0x00
#   rom[0x7ffd] = 0x80

with open("bin1.bin", "wb") as out_file:
    out_file.write(rom)
    print("Done")
