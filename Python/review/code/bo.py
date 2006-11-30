import re,time,datetime,string
import work,Queue
from utils import *

def get_bo_pages(name):
    name = re.sub(r'\([^\d]+\)','', name).strip()
    words = re.sub(r'[#$*!,? ]+',' ',name)
    search_url = 'http://boxofficemojo.com/search/?q='+urllib.quote(words)+'&p=.htm'
    page = urlopen( search_url )
    #print search_url
    p = re.compile(r'href="(/movies/\?id=.*?\.htm)">%s' % re.escape(name).replace('&','&amp;') )
    m = p.search(page)
    if m:
        return 'http://boxofficemojo.com'+m.group(1)
    else:
        print name, words

def tidy(s):
    s = re.sub(r'<br>',SubSep,s)
    s = re.compile(r'<.+?>', re.DOTALL).sub('',s)
    s = re.sub(r'[\s|]+', ' ', s)
    s = s.strip()
    return s

Props = {'total':r'Domestic Total.*?(\$[\d,]+)',
         'distri':r'Distributor: <b>(.*?)</b>',
         'release':r'Release Date: <b><nobr>(.*?)</nobr>',
         'genre':r'Genre: <b>(.*?)</b>',
         'mpaa':r'MPAA Rating: <b>(.*?)</b>',
         'budget':r'Production Budget: <b>(.*?)</b>',
         'weekend':r'Weekend:.*?(\$[\d,]+)',
         'theaters':r'([\d,]+) theaters</b>',
         'director':r'Directors?:</td><td>(.*?)</td>',
         'writer':r'Writers?:</td><td>(.*?)</td>',
         'actors':r'Actors?:</td><td>(.*?)</td>',
         'producer':r'Producers?:</td><td>(.*?)</td>',
         }

def get_daily_bo(id, date):
        try:
            url = 'http://www.boxofficemojo.com/daily/chart/?sortdate=%s&p=.htm' % time.strftime('%Y-%m-%d',date)
            page = urlopen( url )
            return re.search(r'<a href="/movies/\?id=%s.htm">.*?<b>.*?(\$[\d,]+)' % id, page ).group(1)
        except:
            return 'N/A'

def bo_info(url):
    #print url
    page = urlopen( url )
    page = re.search(r'<!--------------------------Site Body---------------------------------->(.*)',page,re.DOTALL).group(1)
    page = page.replace('&nbsp;',' ')
    def get_props(pattern):
        try:
            return tidy(re.search(pattern, page, re.DOTALL).group(1))
        except:
            return 'N/A'
    info = dict(map(lambda (k,v):(k,get_props(v)), Props.items() ))

    #wide release
    m = re.search(r'Release Dates:.*?\(limited\).*?<b>(.*?)</b> \(wide\)', page, re.DOTALL)
    if m :
        info['release'] = m.group(1).strip()
        info['weekend'] = get_props(r'Opening Weekend:.*?(\$[\d,]+)[^$]+wide')
        
    id = re.search(r'id=(.*?)\.htm',url).group(1)
    date = time.strptime(info['release'],'%B %d, %Y')
    d1 = get_daily_bo(id, date)
    d2 = get_daily_bo(id, date = (datetime.datetime(date[0],date[1],date[2]) + datetime.timedelta(1)).timetuple() )
   
    info = [info['distri'],
            info['release'],
            info['genre'],
            info['mpaa'],
            info['budget'],
            info['total'],
            info['weekend'],
            info['theaters'],
            info['director'],
            info['writer'],
            info['actors'],
            info['producer'],
            d1,
            d2]
    
    return info

def bo(title):
    url = get_bo_pages( title )
    if url:
        return (title,[url] + bo_info(url))
    else:
        return (title,None)

def craw(movielist, all = {}):
    wm = work.WorkerManager(min(len(movielist),21))
    for movie in movielist:
        wm.add_job( bo, movie)
    print '%d/%d' % (len(all),len(movielist))

    try:
        while True:  
            try:  
                title,info = wm.get_result(True, timeout = 90)
                if info: all[title] = info
                print '[%d/%d]' % (len(all),len(movielist)),title
            except Queue.Empty:  
                break
    except:
        pass
    
    print 'Got %d/%d ' % (len(all),len(movielist))
    return all

def do_craw(filename):
    import socket
    socket.setdefaulttimeout(20)    

    movielist = filter(None, open(filename).read().split('\n') )

    try:
        infos = open('bo_'+filename).read().split('\n')
        infos = map(lambda s:s.split(Sep), infos)
        infos = dict(map(lambda l:(l[0],l[1:]), infos))
    except:
        infos = {}

    movielist = [m for m in movielist if not infos.has_key(m) ]
    infos = craw(movielist, infos)
    
    infos = map(lambda (k,v):[k]+v, infos.items())
    infos = map(lambda l:Sep.join(l), infos)
    infos.sort()
    infos = '\n'.join(infos)
    open('bo_'+filename,'w').write( infos )

def get_release_date(filename):
    infos = filter(None, open('bo_'+filename).read().split('\n') )
    infos = map(lambda s:s.split(Sep), infos)
    dates = dict(map(lambda l:(l[0],time.strptime(l[3],"%B %d, %Y")), infos))
    return dates
    
if __name__ == '__main__':
    #print bo('About Schmidt')
    #do_craw( 'movie.txt' )
    get_release_date('movie_all.txt')