import re,time
import work,Queue
from utils import *

review_dates = {}

def get_author_id(author):
    if author == 'Michael Phillips Chicago Tribune':
        return 11845
    url = 'http://www.rottentomatoes.com/search/full_search.php?search='+author.replace(' ','+')
    page = urlopen(url)
    p = re.compile(r'CRITIC SEARCH RESULTS.*?<a class=person-lg-link href="/author-(\d+)/">',re.DOTALL)
    m = p.search(page)
    if m :
        return m.group(1)



def craw(url):
##    info_pattern = re.compile(r'<tr valign=middle>.*?<span .*?>(.*?)</span>.*?'
##                              +r'(<a href=".*?rid=(\d+).*?">)?.*?<em>Posted (?P<date>.*?)</em>.*?'
##                              +r'<a class="movie-link" href="/m/.*?/">(?P<title>.*?)</a> '
##                              +r'\((\d*?)\)</td>',re.DOTALL)
    info_pattern = re.compile(r'<tr valign=middle>.*?<em>Posted (?P<date>.*?)</em>.*?'
                              +r'<a class="movie-link" href="/m/.*?/">(?P<title>.*?)</a> '
                              +r'\((\d*?)\)</td>',re.DOTALL)
    page = urlopen(url)
    return (url,info_pattern.findall(page) )

def craw_review_date(author):
    import socket
    socket.setdefaulttimeout(30)
    
    print "\nstart to craw %s's reviews" % author
    id = get_author_id(author)
    if not id:
        print 'no such author:', author
        return {}

    url = 'http://www.rottentomatoes.com/author-%s/' % id
    tp = re.compile(r'Total Reviews: (\d+)')
    page = urlopen(url)
    try:
        total = int(tp.search(page).group(1))
    except:
        clear_cache(url)
        return {}
    page_count = total/50 + 1

    all = {}
    wm = work.WorkerManager(min(page_count,10))
    for i in range(page_count):
        wm.add_job( craw, url+'?page=%d' % (i+1))

    wm.wait_for_complete()
    
    while True:  
        try:  
            ul,ll = wm.get_result(False)
            for date, title, year in ll:
                if title.find('>') != -1:
                    print 'paser error:',ul
                    #clear_cache( ul )
                    #wm.add_job( craw, ul)
                else:
                    all[title] = [year, date]
        except Queue.Empty:  
            break

    print 'Got %d/%d of %s' % (len(all), total, author)
    return all

def proc_title(title):
    return re.sub(r'[ ,:\-?&\'"]+','',title).upper()
    
def get_review_dates(author):
    try:
        lines = open('RT/%s.txt' % author).read().split('\n')
        lines = map(lambda l:(proc_title(l[0]),l[1:]), map(lambda l:l.split('::'), lines))
        dates = dict(lines)
    except:
        dates = craw_review_date(author)
        lines = map(lambda i:[i[0]]+i[1], dates.items() )
        lines = map(lambda l:'::'.join(l), lines )
        lines.sort()
        open('RT/%s.txt' % author, 'w').write( '\n'.join( lines ) )
        dates = dict( map(lambda (k,v):(proc_title(k),v),dates.items()) )
    return dates

def get_review_date(author, title):
    try:
        dates = review_dates[author]
    except :
        try:
            dates = get_review_dates(author)
            review_dates[author] = dates
        except:
            pass
    try:
        return time.strptime(dates[proc_title(title)][1], '%b %d, %Y')
    except:
        return None

if __name__ == '__main__':
    print get_review_date('Maitland McDonagh', 'Star Wars Episode II - Attack of the Clones')