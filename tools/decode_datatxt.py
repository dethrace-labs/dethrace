#!/usr/bin/env python3

import argparse
import enum
import sys

LONG_KEY = (
    0x6c, 0x1b, 0x99, 0x5f, 0xb9, 0xcd, 0x5f, 0x13,
    0xcb, 0x04, 0x20, 0x0e, 0x5e, 0x1c, 0xa1, 0x0e,
)
OTHER_LONG_KEY = (
    0x67, 0xa8, 0xd6, 0x26, 0xb6, 0xdd, 0x45, 0x1b,
    0x32, 0x7e, 0x22, 0x13, 0x15, 0xc2, 0x94, 0x37,
)
DEMO_KEY = (
    0x58, 0x50, 0x3A, 0x76, 0xCB, 0xB6, 0x85, 0x65,
    0x15, 0xCD, 0x5B, 0x07, 0xB1, 0x68, 0xDE, 0x3A,
)

class Method(enum.Enum):
    Method1 = "1"
    Method2 = "2"
    Demo = "demo"

class Byte:
    def __init__(self, v: int):
        self.v = v & 0xff

    def __add__(self, v: int):
        return Byte(self.v + v)

    def __iadd__(self, v: "Byte"):
        self.v = (self.v + v) & 0xff
        return self

    def __sub__(self, v: int):
        return Byte(self.v - v)

    def __isub__(self, v: "Byte"):
        self.v = (self.v - v) & 0xff
        return self

    def __mod__(self, v: int):
        return Byte(self.v % v)

    def __imod__(self, v: int):
        self.v %= v
        return self

    def __xor__(self, v: int):
        return Byte(self.v ^ v)

    def __ixor__(self, v: int):
        self.v = (self.v ^ v) & 0xff
        return self

    def __and__(self, v: int):
        return Byte(self.v & v)

    def __iand__(self, v: int):
        self.v = (self.v & v) & 0xff
        return self

    def __or__(self, v: int):
        return Byte(self.v | v)

    def __ior__(self, v: int):
        self.v = (self.v | v) & 0xff
        return self

    def __eq__(self, other):
        if isinstance(other, Byte):
            return self.v == other.v
        elif isinstance(other, int):
            return self.v == (other & 0xff)
        raise ValueError(f"Object {other:r} of invalid type {type(other)}")

    def __repr__(self):
        return f"(byte 0x{self.v:02x})"


class Codec1:
    def encode_line(self, line: bytes) -> bytes:
        line = line.rstrip(b"\r\n")
        key = LONG_KEY
        seed = len(line) % len(key)
        count = 0
        eline = bytearray(len(line))
        for i, c in enumerate(line):
            if count == 2:
                key = OTHER_LONG_KEY
            if c == ord('/'):
                count += 1
            else:
                count = 0
            if c == ord('\t'):
                c = 0x9f

            b = Byte(c)
            b -= 0x20
            b ^= key[seed]
            b &= 0x7f
            b += 0x20

            seed += 7
            seed %= len(key)

            if b == 0x9f:
                b = Byte(ord('\t'))
            eline[i] = b.v
        return bytes(eline)

    def decode_line(self, line: bytes) -> bytes:
        line = line.rstrip(b"\r\n")
        key = LONG_KEY
        seed = len(line) % len(key)
        dline = bytearray(len(line))
        for i, c in enumerate(line):
            b = Byte(c)
            if dline[i - 2:i] == b'//':
                key = OTHER_LONG_KEY

            if b == ord(b'\t'):
                b = Byte(0x9f)

            b -= 0x20
            b ^= key[seed]
            b &= 0x7f
            b += 0x20

            seed += 7
            seed %= len(key)

            if b == 0x9f:
                b = Byte(ord(b'\t'))
            dline[i] = b.v
        return dline


class Codec2:
    def encode_line(self, line: bytes) -> bytes:
        line = line.rstrip(b"\r\n")
        key = LONG_KEY
        seed = len(line) % len(key)
        count = 0
        eline = bytearray(len(line))
        for i, c in enumerate(line):
            if count == 2:
                key = OTHER_LONG_KEY
            if c == ord('/'):
                count += 1
            else:
                count = 0

            if c == ord('\t'):
                c = 0x80
            b = Byte(c - 0x20)
            if (b & 0x80) == 0:
                b ^= key[seed] & 0x7f
            b += 0x20

            seed += 7
            seed %= len(key)

            if b == 0x80:
                b = Byte(ord('\t'))
            eline[i] = b.v
        return bytes(eline)

    def decode_line(self, line: bytes) -> bytes:
        line = line.rstrip(b"\r\n")
        key = LONG_KEY
        seed = len(line) % len(key)
        dline = bytearray(len(line))
        for i, c in enumerate(line):
            b = Byte(c)
            if dline[i - 2:i] == b'//':
                key = OTHER_LONG_KEY

            if b == ord(b'\t'):
                b = Byte(0x80)

            b -= 0x20
            if (b & 0x80) == 0:
                b ^= key[seed] & 0x7f
            b += 0x20

            seed += 7
            seed %= len(key)

            if b == 0x80:
                b = Byte(ord(b'\t'))

            dline[i] = b.v
        return dline


class CodecDemo:
    def encode_line(self, line: bytes) -> bytes:
        line = line.rstrip(b"\r\n")
        key = DEMO_KEY
        seed = len(line) % len(key)
        dline = bytearray(len(line))
        for i, c in enumerate(line):
            b = Byte(c)

            if b == ord('\t'):
                b = Byte(0x9f)

            b -= 0x20
            b ^= key[seed]
            b &= 0x7f
            b += 0x20
            if b == 0x9f:
                b = Byte(ord('\t'))
            if b == ord('\n') or b == ord('\r'):
                b |= 0x80

            seed += 7
            seed %= len(key)

            dline[i] = b.v
        return dline

    def decode_line(self, line: bytes) -> bytes:
        line = line.rstrip(b"\r\n")
        key = DEMO_KEY
        seed = len(line) % len(key)
        dline = bytearray(len(line))
        for i, c in enumerate(line):
            b = Byte(c)

            if b == ord('\t'):
                b = Byte(0x9f)

            b -= 0x20
            b ^= key[seed]
            b &= 0x7f
            b += 0x20

            seed += 7
            seed %= len(key)

            if b == 0x9f:
                b = Byte(ord('\t'))

            dline[i] = b.v
        return dline


CODECS = {
    Method.Method1: Codec1,
    Method.Method2: Codec2,
    Method.Demo: CodecDemo,
}


def main():
    method_choices = tuple(e.value for e in Method.__members__.values())
    parser = argparse.ArgumentParser(allow_abbrev=False, description="Decode/encode a Carmageddon text file")
    parser.add_argument("file", metavar="FILE", nargs="?", help="input file (default=stdin)")
    parser.add_argument("--method", choices=method_choices, default=Method.Method2.value,
        help=f"encryption method to use (default={Method.Method2.value}, choices={','.join(method_choices)})")
    args = parser.parse_args()

    method = Method(args.method)
    codec = CODECS[method]()

    istream = open(args.file, "rb") if args.file else sys.stdin.buffer

    for line in istream.readlines():
        if line[0] == ord(b"@"):
            dline = codec.decode_line(line[1:])
            sys.stdout.buffer.write(dline)
        else:
            eline = b"@" + codec.encode_line(line)
            sys.stdout.buffer.write(eline)
        sys.stdout.buffer.write(b'\n')


if __name__ == "__main__":
    raise SystemExit(main())
