import serial

import struct

value = 5.1245
# print(str(value))
print(bytes(str(value)))

# ba = bytearray(struct.pack("f", value))
# print(ba)


# turn the byte array back into a float
