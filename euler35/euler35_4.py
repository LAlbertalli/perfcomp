# Prototype of implementing a Sieve algorithm in Python
# I don't expect it to be blazing fast, especially running on CPython

from math import sqrt
import sys

def rotate(num):
    s = "%d"%num
    for i in xrange(len(s)):
        yield(int(s[i:]+s[:i]))

def euler35(n):
    base_list = [0, 0, 1] + [1, 0]*((n-2)/2)
    for i in xrange(3,int(sqrt(n))+1):
        if base_list[i]:
            for j in xrange(i*2,n,i):
                base_list[j] = 0
    for e,i in enumerate(base_list):
        if i and not all(base_list[j] for j in rotate(e)):
            for j in rotate(e):
                # Don't delete lower than e, they are actually have already been tested
                if j >= e:
                    base_list[j] = 0
    return base_list

if __name__ == '__main__':
    num = int(sys.stdin.readline())
    l = euler35(num)
    print(sum(l))