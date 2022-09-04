#!/usr/bin/env python

import argparse
import sys

LONG_KEY = (
    0x6c, 0x1b, 0x99, 0x5f, 0xb9, 0xcd, 0x5f, 0x13,
    0xcb, 0x04, 0x20, 0x0e, 0x5e, 0x1c, 0xa1, 0x0e,
)
OTHER_LONG_KEY = (
    0x67, 0xa8, 0xd6, 0x26, 0xb6, 0xdd, 0x45, 0x1b,
    0x32, 0x7e, 0x22, 0x13, 0x15, 0xc2, 0x94, 0x37,
)


def decode_line(line: bytes, method: int) -> str:
    line = line.rstrip(b"\r\n")
    key = LONG_KEY
    seed = len(line) % 16
    dline = bytearray(len(line))
    for i, c in enumerate(line):
        if i >= 2:
            if line[i - 1] == b'/' and line[i - 2] == b'/':
                key = OTHER_LONG_KEY
        if method == 1:
            if c == ord(b'\t'):
                c = 0x80
            c -= 0x20
            if not (c & 0x80):
                c ^= key[seed]
                c &= 0x7f
                c += 0x20

            seed += 7
            seed %= 16
            if c == 0x80:
                c = ord(b'\t')
        else:
            if c == ord(b'\t'):
                c = 0x9f
            c -= 0x20
            c ^= key[seed]
            c &= 0x7f
            c += 0x20

            seed += 7
            seed %= 16
            if c == 0x9f:
                c = ord(b'\t')
        dline[i] = c
    return dline.decode(errors="replace")

def main():
    parser = argparse.ArgumentParser(allow_abbrev=False, description="Decode a Carmageddon encoded file")
    parser.add_argument("file", metavar="FILE", nargs="?", help="input file (default=stdin)")
    parser.add_argument("--method", choices=[1, 2], type=int, default=2, help="decryption method to use")
    args = parser.parse_args()

    istream = open(args.file, "rb") if args.file else sys.stdin.buffer

    for line in istream.readlines():
        if line[0] == ord(b'@'):
            dline = decode_line(line[1:], args.method)
            print(dline)
        else:
            print(line.decode())


if __name__ == "__main__":
    raise SystemExit(main())
