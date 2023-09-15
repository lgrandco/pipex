from json import dumps, loads
from typing import List
import sys


def run(l):
    x = y = 0
    d = {0: 'R', 1: 'D', 2: 'L', 3: 'U'}
    dir = 0
    for e in l:
        if e == 'FORWARD':
            if d[dir] == 'R':
                x += 1
            elif dir == 'L':
                x -= 1
            elif dir == 'U':
                y += 1
            elif dir == 'D':
                y -= 1
        elif e == 'BACK':
            if dir == 'R':
                x -= 1
            elif dir == 'L':
                x += 1
            elif dir == 'U':
                y -= 1
            elif dir == 'D':
                y += 1
        elif e == 'TURN LEFT':
            dir = 'L'
        elif e == 'TURN RIGHT':
            dir = 'R'
