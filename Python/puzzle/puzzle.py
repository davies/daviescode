#!/usr/bin/python

AUTHOR = "Davies Liu (davies.liu@gmail.com)"
DATE = "8/2/2006"

"""
Engineering Puzzle

You have four colored cubes. Each side of each cube is a single color,
and there are four colors: blue (B), red (R), green (G) and yellow (Y)
Describing the six faces as front, back, left, right, top, bottom,
the cube colors are:
Cube    Front   Back    Left    Right   Top     Bottom
1       R       B       G       Y       B       Y
2       R       G       G       Y       B       B
3       Y       B       R       G       Y       R
4       Y       G       B       R       R       R

The objective is to find ways to stack the four cubes as a vertical column
so that each side of the column is showing all four colors.
"""

def rotate(n):
    """   rotate four side of a cube    """
    r = [n]
    tb,others = n & 0xff0000, n & 0xffff
    for i in range(3):
        others = ((others&0xf000) >> 12) | ((others&0xff) << 8) | ((others&0xf00) >> 4)
        r.append(tb|others)
    return r

def turnover(n):
    """turn over the cube"""
    return [n,((n&0xf0f000)>>4) | ((n&0x0f0f00)<<4) | (n&0xff) ]

def turn(n, Rotate = True):
    """ turn the cube"""
    r = [n,
         ((n&0x0f)<<20) | ((n&0xf0)<<12) | ((n&0xff0000)>>16) | (n&0xff00),
         ((n&0x0f00)<<12) | ((n&0xf000)<<4) | ((n&0xff0000)>>8) | (n&0xff)]
    if Rotate:
        r = sum(map(turnover,r),[])
        r = sum(map(rotate,r),[])
    return r

def samecolor(aboves,below):
    """check if the below cube has the same color with any of above cubes, on any side"""
    for above in aboves:
        if above & below & 0xffff : return True
    return False

def combine(aboves,below):
    """put a cube below, generate a new sequence"""
    return [a+[b] for a in aboves for b in below if not samecolor(a,b) ]

def solve(cube):
    """solve the puzzle """
    # generate the permunation of the directions of the four cubes
    permunation = [turn(cube[0],False)] + map(turn,cube[1:])
    # put four cubes together, and validate them
    return reduce(combine,permunation,[[]])

def output(result):
    print "Don't change position of four cubes and don't turn them in the same time,"
    print "there are %d way to stack the four cubes, they are listed below:\n" % len(result)
    
    colors = ['']*9
    colors[1],colors[2],colors[4],colors[8] = 'R','G','B','Y'

    print "Cube    Top     Bottom   Front   Back    Left    Right   "
    for r in result:
        for i in range(len(r)):
            print i+1,
            for j in range(6):
                c = 0xf & (r[i] >> ((5-j)*4))
                print "\t%s" % colors[ c ],
            print
        print
                             

def main():
    # use a number to identity the direnction of a cube, four bits per side
    # Top     Bottom   Front   Back     Left   Right
    # 0x01 R
    # 0x02 G
    # 0x04 B
    # 0x08 Y
    cubes = [ 0x481428,
              0x441228,
              0x818412,
              0x118241]  
    result = solve(cubes)
    output(result)

def test():
    n = 0x123456
    print map(hex,rotate(n))
    print map(hex,turnover(n))
    print map(hex,turn(n))
    
if __name__ == '__main__':
    #test()
    main()
