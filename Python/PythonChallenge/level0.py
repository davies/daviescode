#/usr/bin/python

import string,re
import urllib

# solve python challange question
# http://

# for warnning up
def level0_url():
    # get 2^28 as the url
    print pow( 2, 38 )


def level1_map():
    """
    map between (K,M) and (O,Q) and (E,G)
    """
    old_url = "map"
    new_url = ""
    for char in old_url:
        new_url = new_url + chr( ord( char ) + 2 )

    # another method
    #tranlate
    table = ""
    for i in range( 256 ):
        table = table + chr( i )
    print table
    table = table[2:] + table[:2]
    new_url = string.translate( old_url , table )
    print new_url

def level2_ocr():
    data_file = "level2_ocr.txt"
    data = open( data_file ).read()
    # find all characters
    chars = re.findall(r'[a-zA-Z]+',data)
    print string.join( chars,"" )

def level3_re():
    data_file = "level3_re.txt"
    data = open( data_file ).read()
    # a small character, EXCACTLY tree BIG character on both sides
    # find all the small charater
    result = re.findall(r'[~A-Z][A-Z]{3,3}([a-z])[A-Z]{3,3}[~A-Z]',data)
    print string.join( result ,"")

def level4_chain():
    # follow the train
    url = "http://www.pythonchallenge.com/pc/def/linkedlist.php?nothing=%s"
    nothing = "46059"
    res = urllib.urlopen( url % nothing ).read()
    m = re.search(r'and the next nothing is (?P<num>\d+)', res)
    count = 1
    while m :
        nothing = m.group( 1 )
        print count," : ",nothing
        count = count + 1
        res = urllib.urlopen( url % nothing ).read()
        m = re.search(r'and the next nothing is (\d+)', res)
    print "the final nothing is %s" % nothing

def level5_peakhell():
    import pickle
    banner = pickle.load( open("level5_banner.p") )
    for line in banner:
        #print line
        str = ""
        for (char,count) in line:
            str = str + char * count
        print str

def level6_zip():
    import zipfile
    zf = zipfile.ZipFile('channel.zip')
    number = '90052'
    results = []
    while 1:
        try:
            filename = "%s.txt" % number
            results.append(str(zf.getinfo(filename).comment))
            data = zf.read(filename)
            number = data.split()[-1]
        except:
            print data
            break
    print string.join(results,"")
    # the letters: oxygen

def level7_smarty():
    import Image
    im = Image.open('oxygen.png')
    msg = []
    for y in range(0,608,7):
        pixel = im.getpixel( (y,43) )
        msg.append( chr( pixel[0] ) )
    msg = "".join(msg)
    result = [105, 110, 116, 101, 103, 114, 105, 116, 121]
    url = ""
    for c in result:
        url += chr(c)
    print url
            
def level8_workhard():
    un = 'BZh91AY&SYA\xaf\x82\r\x00\x00\x01\x01\x80\x02\xc0\x02\x00 \x00!\x9ah3M\x07<]\xc9\x14\xe1BA\x06\xbe\x084'
    pw = 'BZh91AY&SY\x94$|\x0e\x00\x00\x00\x81\x00\x03$ \x00!\x9ah3M\x13<]\xc9\x14\xe1BBP\x91\xf08'
    import bz2
    print bz2.decompress(un)
    print bz2.decompress(pw)

def level9_connect():
    first=[146,399,163,403,170,393,169,391,166,386,170,381,170,371,170,355,169,346,167,335,170,329,170,320,170,\
        310,171,301,173,290,178,289,182,287,188,286,190,286,192,291,194,296,195,305,194,307,191,312,190,316,\
        190,321,192,331,193,338,196,341,197,346,199,352,198,360,197,366,197,373,196,380,197,383,196,387,192,\
        389,191,392,190,396,189,400,194,401,201,402,208,403,213,402,216,401,219,397,219,393,216,390,215,385,\
        215,379,213,373,213,365,212,360,210,353,210,347,212,338,213,329,214,319,215,311,215,306,216,296,218,\
        290,221,283,225,282,233,284,238,287,243,290,250,291,255,294,261,293,265,291,271,291,273,289,278,287,\
        279,285,281,280,284,278,284,276,287,277,289,283,291,286,294,291,296,295,299,300,301,304,304,320,305,\
        327,306,332,307,341,306,349,303,354,301,364,301,371,297,375,292,384,291,386,302,393,324,391,333,387,\
        328,375,329,367,329,353,330,341,331,328,336,319,338,310,341,304,341,285,341,278,343,269,344,262,346,\
        259,346,251,349,259,349,264,349,273,349,280,349,288,349,295,349,298,354,293,356,286,354,279,352,268,\
        352,257,351,249,350,234,351,211,352,197,354,185,353,171,351,154,348,147,342,137,339,132,330,122,327,\
        120,314,116,304,117,293,118,284,118,281,122,275,128,265,129,257,131,244,133,239,134,228,136,221,137,\
        214,138,209,135,201,132,192,130,184,131,175,129,170,131,159,134,157,134,160,130,170,125,176,114,176,\
        102,173,103,172,108,171,111,163,115,156,116,149,117,142,116,136,115,129,115,124,115,120,115,115,117,\
        113,120,109,122,102,122,100,121,95,121,89,115,87,110,82,109,84,118,89,123,93,129,100,130,108,132,110,\
        133,110,136,107,138,105,140,95,138,86,141,79,149,77,155,81,162,90,165,97,167,99,171,109,171,107,161,\
        111,156,113,170,115,185,118,208,117,223,121,239,128,251,133,259,136,266,139,276,143,290,148,310,151,\
        332,155,348,156,353,153,366,149,379,147,394,146,399]
    second = [156,141,165,135,169,131,176,130,187,134,191,140,191,146,186,150,179,155,175,157,168,157,163,157,159,\
        157,158,164,159,175,159,181,157,191,154,197,153,205,153,210,152,212,147,215,146,218,143,220,132,220,\
        125,217,119,209,116,196,115,185,114,172,114,167,112,161,109,165,107,170,99,171,97,167,89,164,81,162,\
        77,155,81,148,87,140,96,138,105,141,110,136,111,126,113,129,118,117,128,114,137,115,146,114,155,115,\
        158,121,157,128,156,134,157,136,156,136]
    def splitseq( seq, size):
        return [seq[i:i+size] for i in range(0,len(seq),size)]
    first = splitseq(first,2)
    second = splitseq(second,2)
    import Image
    original = Image.open('good.jpg')
    new = Image.new(original.mode,original.size)
    for x,y in first:
        new.putpixel( (x,y),(255,255,255) )
    for x,y in second:
        new.putpixel( (x,y), (255,255,255) )
    new.show()
    # then got a bull

def level10_sequence():
    a = ['1']
    def next_seque( str ):
        seq = ''
        last = ''
        count = 0
        for c in str:
            if last == c :
               count += 1
            else:
                if last != '': seq += '%d%s' % (count,last)
                last = c
                count = 1
        seq += '%d%s' % (count,last)
        return seq
    size = 30
    for i in range(size):
        a.append( next_seque( a[len(a)-1]) ) 
    print len(a[size])

def level11_even_odd():
    import Image
    original = Image.open('cave.jpg')
    size = original.size
    newsize = (size[0]/2,size[1]/2)
    new = Image.new(original.mode,newsize )
    for x in range( newsize[0] ):
        for y in range( newsize[1] ):
            new.putpixel( (x,y), original.getpixel((x*2,y*2)) )
    new.show()
    # use acdsee
    pass

def level12_evil():
    import Image
    original = Image.open('evil1.jpg')
    size = original.size
    newsize = (size[0]/2,size[1]/8)
    new = Image.new(original.mode,newsize )
    for x in range( newsize[0] ):
        for y in range( newsize[1] ):
            new.putpixel( (x,y), original.getpixel((x*2,y*8)) )
    new.show()
    # use acdsee

#level0_url()    
#level1_map()
#level2_ocr()
#level3_re()
#level4_chain()
#level5_peakhell()
#level6_zip()
#level7_smarty()
#level8_workhard()
#level9_connect()
#level10_sequence()
#level11_even_odd()
level12_evil()