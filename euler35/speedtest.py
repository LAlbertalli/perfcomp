import subprocess, sys
from itertools import islice

FILES = ['euler35.cpp', 'euler35_1.cpp', 'euler35_2.cpp', 'euler35_3.cpp']

def test(f, test = (1000000, 55)):
    size, expected = test
    p = subprocess.Popen([f], stdin = subprocess.PIPE, 
        stdout=subprocess.PIPE, stderr = subprocess.PIPE)
    p.stdin.write('%d\n'%size)
    p.wait()
    time = p.stderr.readlines()
    res = p.stdout.readlines()
    for i in res:
        print i.strip()
    return res[-1] == '%d\n'%expected

def spd_test(f,size = 1000000):
    p = subprocess.Popen(['time', f], stdin = subprocess.PIPE, 
        stdout=subprocess.PIPE, stderr = subprocess.PIPE)
    p.stdin.write('%d\n'%size)
    p.wait()
    time = p.stderr.readlines()
    res = p.stdout.readlines()
    return parse_time(time[0])

def parse_time(line):
    data = filter(lambda x:x!='', line.strip().split(' '))
    return dict(zip(islice(data,1,None,2),map(float,islice(data,0,None,2))))

def avg_time(f, size = 1000000, loops = 20):
    acc = [spd_test(f,size)['user'] for _ in xrange(loops)]
    return sum(acc)/len(acc)

def compile(fname, level):
    out = 'build/'+fname.split('.')[0]+'_l%d'%level
    if level == 0:
        p = subprocess.Popen(['g++', '-o', out, '-O%d'%level, '-std=c++11', '-g' , fname])
    else:
        p = subprocess.Popen(['g++', '-o', out, '-O%d'%level, '-std=c++11', fname])
    p.wait()
    return out

def main(args):
    if len(args)>1 and args[1]=='-t':
        #Just Test
        if len(args)>2:
            files = [FILES[int(i)] for i in args[2:]]
        else:
            files = FILES
        for f in files:
            print("Testing %s"%f)
            print("Compiling...")
            out = compile(f,0)
            print("Done!\nTesting...")
            if test(out):
                print("Success!\n")
            else:
                print("Fail!\n")
    else:
        #Speed Test
        baseline = -1
        for f in FILES:
            print("Testing %s"%f)
            print("Compiling...")
            out1 = compile(f,0)
            out2 = compile(f,3)
            print("Done!\nTesting...")
            t1 = avg_time(out1)
            t2 = avg_time(out2)
            if baseline == -1:
                baseline = t2
            print("Done!\nTime was %f for -O0 and %f for -O3 -> %.2f%% improvement"
                %(t1,t2,(t1-t2)/t1*100))
            print("Compared to baseline, %.2f%% improvement"%((baseline-t2)/baseline*100))

if __name__ == '__main__':
    main(sys.argv)