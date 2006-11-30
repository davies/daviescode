import re,time,datetime
import work,Queue
from utils import *

def blog_info(title, release):
    title = title.replace('&amp;','&')
    days = (time.time() - time.mktime(time.strptime(release, '%B %d, %Y'))) / 3600 / 24 + 10
    words = re.sub(r'[,: -!?()&]+',' ',title)
    words = '+'.join(filter(None,words.split(' ')))
    url = 'http://www.blogpulse.com/trend?days=%d&x=39&y=16&label1=Movie&query1=%s' % (days,words)
    page = urlopen( url )
    p = re.compile(r'title="Movie: \((.*?), ([\d.]+)\)"')
    ll = p.findall(page)
    if not len(ll):
        clear_cache(url)
        raise IOError
    date_list = map(lambda (d,p):d, ll)
    percent_list = map(lambda (d,p):float(p), ll)
    max_p = max(percent_list)
    max_d = date_list[ percent_list.index(max_p) ]
    
    release = time.strftime('%b %d', time.strptime(release, '%B %d, %Y'))
    ri = date_list.index( release )
    pre_list = percent_list[ri-7:ri]
    total = sum( pre_list )
    res = [title, release, max_d, max_p] + pre_list + [total]
    res[3:] = map(lambda v:"%.4f" % v, res[3:])
    return res 

def get_latest_movies(filename):
    try:
        lines = open('latest_'+filename).read().split('\n')
        mlist = map(lambda l:l.split(Sep), lines)
    except IOError:
        infos = open('bo_'+filename).read().split('\n')
        infos = map(lambda s:s.split(Sep), infos)
        infos = map(lambda l:[l[0],l[3]], infos)
        def date_filter( l ):
            secs = time.mktime(time.strptime(l[1],'%B %d, %Y'))
            return secs + 170 * 24 * 3600 > time.time()
        mlist = filter(date_filter, infos )
        lines = map(lambda l:Sep.join(l), mlist)
        open('valid_'+filename,'w').write( '\n'.join(lines) )
    return mlist

def craw(movielist):
    wm = work.WorkerManager(min(len(movielist),5))
    for movie in movielist:
        wm.add_job( blog_info, movie[0], movie[1])

    all = []
    print '%d/%d' % (len(all),len(movielist))

    try:
        while True:  
            try:  
                info = wm.get_result(True, timeout = 90)
                print '[%d/%d]' % (len(all),len(movielist)),(info[0])
                all.append( info )
            except Queue.Empty:  
                break
    except:
        pass
    
    print 'Got %d/%d ' % (len(all),len(movielist))
    return all    

def do_craw(filename):
    import socket
    socket.setdefaulttimeout(60)    

    movielist = get_latest_movies(filename)

    infos = craw(movielist[:])

    lines = map(lambda info:Sep.join(info), infos)
    open('blog_'+filename,'w').write( '\n'.join(lines) )
    
if __name__ == '__main__':
    #print blog_info('FLAGS OF OUR FATHERS', 'October 20, 2006')
    #get_latest_movies('movie_all1.txt')
    do_craw('movies.txt')