#!/usr/bin/env python3
import subprocess
import time

def run(command):
    process = subprocess.run(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    out = process.stdout.decode('utf8')
    code = process.returncode
    return code, out

def test(command, expected_code, expected_value):
    print(command)
    code, out = run(command)
    if code != expected_code:
        print('return value', expected_code, '(expected) !=', code)
        return
    i = 0
    for line in out.splitlines():
        try:
            if line != expected_value[i]:
                print(line)
                print(expected_value[i], '(expected) !=', line)
                return
            i += 1
        except ValueError:
            print('invalid output')
            return
        except IndexError:
            print('invalid output')
            return
    if i != len(expected_value):
        print('empty output')
        return
    print('ok')

test('./test "2"', 0, [ '2' ])
test('./test "-2"', 0, [ '-2' ])
test('./test "2 + 2"', 0, [ '4' ])
test('./test "2 + 2   "', 0, [ '4' ])
test('./test "2 +- 2"', 0, [ '0' ])
test('./test "   2+-4"', 0, [ '-2' ])
test('./test "-    4- -4"', 0, [ '0' ])
test('./test "2-3*4+-5/2"', 0, [ '-12' ])
test('./test "2-3*4*2+1--2+-5/2"', 0, [ '-21' ])
test('./test', 1, [ 'error' ])
test('./test 2 + 3', 1, [ 'error' ])
test('./test "2/0"', 1, [ 'error' ])
test('./test "2/"', 1, [ 'error' ])
test('./test "3 + a"', 1, [ 'error' ])
