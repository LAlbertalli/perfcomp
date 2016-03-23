# My original implementation.
# I haven't designed it with speed in mind but for 
# efficiency (aka, I wrote it in 5 min, if it returns 
# the results in few seconds is good enough)

from math import sqrt
import sys

def rotate(num):
    s = "%d"%num
    for i in xrange(len(s)):
        yield(int(s[i:]+s[:i]))

def addIfPrime(num, plist):
    if num < 2:
        return
    if num == 2:
        plist += [2]
        return
    if num%2 == 0:
        return
    stop = sqrt(num)
    for i in plist:
        if i > stop:
            break
        if num%i == 0:
            return
    plist += [num]


def euler35(n):
    plist = []
    for i in xrange(2,n):
        addIfPrime(i,plist)
    plist = set(plist)
    for i in plist:
        if all(j in plist for j in rotate(i)):
            yield i

if __name__ == '__main__':
    num = int(sys.stdin.readline())
    l = list(euler35(num))
    print(len(l))