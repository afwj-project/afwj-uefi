#!python

'''
Python 3.x virtual environment required.
'''

from sys import argv
from sys import exit as sexit
from os import name, system

if __name__ == '__main__':
    if len(argv) < 2:
        print('Arguments missed!')
        sexit(2)
    args = ' '.join(argv[1:])
    if name == 'nt':
        system(f'ld {args}')
        sexit(0)
    elif name == 'posix':
        system(f'x86_64-w64-mingw32-ld {args}')
        sexit(0)
    else:
        print('Unsupported operating system!')
        sexit(1)
