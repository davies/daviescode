import re,time,os,string,urlparse,datetime
import RT,bo
import work,Queue
from utils import *

def tidy(s):
    s = re.compile(r'<script .*?>.*?</script>',re.IGNORECASE|re.DOTALL).sub('',s)
    s = re.compile(r'<a .*?>.*?</a>',re.IGNORECASE|re.DOTALL).sub('',s)
    s = re.sub(r'<br>',SubSep,s)
    s = re.compile(r'<.+?>', re.DOTALL).sub('',s)
    s = re.sub(r'[\s|]+', ' ', s)
    s = re.sub(r'&nbsp;', ' ', s)
    return s

def word_count(s):
    p = re.compile(r'[\s.,?;:]')
    words = filter(None,p.split(s))
    return len(words)

Reviewer = {'Austin Chronicle':('%Y-%m-%d','<FONT CLASS="hugesans">(?P<content>.*?)<FONT CLASS="sans">\[(?P<date>[\d-]+)\]</FONT>'),
            'Baltimore Sun':('%B %d, %Y','<div class="date">Originally published (?P<date>.*?)</div>.*?<div class="text">(?P<content>.*?)<div class="story-email">'),
            'Boston Globe':('%m/%d/%Y','Published: <span class="bold">(?P<date>.*?)</span>(?P<content>.*)'),
            'Charlotte Observer':('%A, %B %d, %Y','Published: <STRONG>(?P<date>.*?)</STRONG>.*?<!------- begin full review ------->(?P<content>.*?)<!------- end full review ------->'),
            'Chicago Reader':('%B %d, %Y',''),
            'Chicago Tribune':('','<div id="watermark">(?P<content>.*?)</DIV>'),
            'Chicago Sun-Times':('%B %d, %Y','<b> BY.*?</b>(?P<date>.*?)<p>(?P<content>.*)'),
            'Christian Science Monitor':('%B %d, %Y','<span class="pubDate">from the (?P<date>.*?) edition</span>.*?<h1 class="headline">(?P<content>.*?)<div class="portlet-container">'),
            'Dallas Observer':('',''),
            #'Empire':('','<!-- LATEST REVIEWS: END -->(?P<content>.*?)Your Reviews'),
            'Entertainment Weekly':('%m/%d/%y','<span class="content-section-reg-bodytxt">(?P<content>.*?)</span>.*?Posted:(?P<date>[\d/]+)'),
            'Film Threat':('%Y-%m-%d','<span class="story_date">\((?P<date>.*?)\)</span>.*?<span class="story_body">(?P<content>.*?)</span>'),
            'The Hollywood Reporter':('%B %d, %Y','<p class="story_info">(?P<date>.*?)<br>.*?<div class="story">(?P<content>.*?)</div>'),
            'Los Angeles Weekly':('',''),
            'Los Angeles Times':('%B %d, %Y','<TD class="cTimestamp" width="40%" nowrap> (?P<date>.*?)</TD>.*?<div class="content">(?P<content>.*?)<br clear="all" />'),
            'Miami Herald':('%A, %B %d, %Y','(Published: <STRONG>(?P<date>.*?)</STRONG>)?.*?<!------- begin full review ------->(?P<content>.*?)<!------- end full review ------->'),
            'The New Republic':('',''),
            'New York Daily News':('%B %d, %Y','<span class="bodytext">(?P<content>.*?)<BR><BR><B>Originally published on  (?P<date>.*?)</B></span>'),
            'New York Magazine':('%B %d, %Y','<div id="story">(?P<content>.*?)<p><a href=".*?">(?P<date>.*?) issue</a> of New York Magazine</p>'),
            'New York Post':('%B %d, %Y','<span class=.*?>Rating: <img src="/img/stars/4\.gif"></span><br>(?P<date>.*?)\ --(?P<content>.*?)<script language="JavaScript"'),
            'The New York Times':('%Y/%m/%d','<div class="timestamp">Published:.*?</div>(?P<content>.*)'),
            'The New Yorker':('%Y-%m-%d','Posted (?P<date>[\d-]+).*?<p class="descender">(?P<content>.*?)<div class="emailfriends">'),
            'Newsweek':('%b. %d, %Y','Newsweek.*?(?P<date>[^<>]*?) issue (?P<content>.*?)Print this</a>'),
            'The Onion (A.V. Club)':('%B %d, %Y','<div class="col two article_text">.*?<br/>.*?<b>(?P<date>.*?)</b><br/>(?P<content>.*)'),
            'Philadelphia Inquirer':('%A, %B %d, %Y','Published: <STRONG>(?P<date>.*?)</STRONG>.*?<!------- begin full review ------->(?P<content>.*?)<!------- end full review ------->'),
            'Portland Oregonian':('%A, %B %d, %Y','<div class="byln">(?P<date>.*?)<div>.*?</div>\n</div>(?P<content>.*?)</div>'),
            'Premiere':('%m/%d/%y','posted (?P<date>[\d/]+)(?P<content>.*)'),
            'ReelViews':('','<BODY .*?>(?P<content>.*?)</BODY>'),
            'Rolling Stone':('%b %d, %Y','<span class="content">(?P<content>.*?)</span>.*?<p class="post">\(Posted: (?P<date>.*?)\)</p>'),
            'Salon.com':('%Y/%m/%d','article_body(?P<content>.*)'),
            'San Francisco Chronicle':('%B %d, %Y','<p class="date"><font size="1">\w+, (?P<date>.+?)<!-- END WRITER CREDIT-->(?P<content>.*?)<!-- END STORY -->'),
            'Seattle Post-Intelligencer':('%B %d, %Y','<p class="rddateline">\w+, (?P<date>.*?)</p>.*?<p class="rdbyline">(?P<content>.*?)<script'),
            'Slate':('%A, %b. %d, %Y','<span class="dateline">(Posted|Updated)(?P<date>.*?), at.*?</span></div><div class="article_body">(?P<content>.*?)<div class="back_to_top">'),
            'Time':('%b. %d, %Y','<span class="smallRedtext">Posted \w+, (?P<date>.*?)</span><br />(?P<content>.*?)<noindex>'),
            'The Globe and Mail (Toronto)':('%A, %B %d, %Y','<p class="date">(?P<date>.*?), Page.*?</p>.*?<em class="wordlength">(?P<length>[\d]+) words</em>'),
            'TV Guide':('','<tr class="reviewText">(?P<content>.*?)</td>'),
            'USA Today':('%m/%d/%Y','(Posted|Updated) (?P<date>[\w/]+).*?<div class="inside-copy">(?P<content>.*)'),
            'Variety':('%a., %b. %d, %Y','<h3>Posted:(?P<date>.*?)</h3>(?P<content>.*)'),
            'Village Voice':('%B %d, %Y','<span class="head">.*?</b><br />(?P<date>.*?)</div>.*?<div class="bodytext">(?P<content>.*?)<br /><br />'),
            #'Wall Street Journal':('',''), # need login
            'Washington Post':('%A, %B %d, %Y','Washington Post Staff Writer.*>(?P<date>.*?); +Page C01(?P<content>.*?)&copy;.*?The Washington Post Company'),
            }

bad_hosts = ['www.dallasobserver.com',
             'listings.dallasobserver.com',
             'dallasobserver.com',
             'www.laweekly.com',
             ]

PGArchiver = {'Baltimore Sun':'baltsun',
             'Chicago Tribune':'chicagotribune',
             'Los Angeles Times':'latimes',
             'New York Daily News':'nydailynews',
             'New York Post':'nypost',
             'The New York Times':'nytimes',
#             'Rolling Stone':'rollingstone',
             'Wall Street Journal':'wsj',
             'USA Today':'USAToday',
             'Washington Post':'washingtonpost',
              }

def get_movies(title):
    title = re.sub(r'[!?]','',title)
    words = '+'.join(filter(None,title.replace(' - ',' ').split(' ')))
    url = 'http://www.metacritic.com/search/process?tfs=all_titles&x=30&y=12&sb=0&ty=1&ts=' + words
    page = urlopen( url )
    p = re.compile(r'<A href="(http://www.metacritic.com/\w+/titles/.*?)">.*?</A> \((\d+)\) <SPAN CLASS="\w+">(\d+)</SPAN>',
                   re.DOTALL)
    m = p.search(page)
    if m:
        return m.groups()
    else:
        print url
        return None,None,None

def review_info(url):
    page = urlopen( url )
    release = re.search(r'Theatrical: <b>(.*?)</b>', page).group(1)
    p = re.compile(r'''<DIV\ CLASS="criticscore">(\d+)</DIV>.*?
                        <SPAN\ CLASS="publication">(.*?)</SPAN>.*?
                        <SPAN\ CLASS="criticname">(.*?)</SPAN>.*?
                        <DIV\ CLASS="quote">(.*?)(<BR><A\ HREF="(.*?)"\ TARGET="_blank"><IMG.*?></A>)?</DIV>''',
                   re.DOTALL|re.VERBOSE)
    reviews = p.findall(page)
    return release,reviews    
  

def parse_date_length(pub, url, pattern, date_format, code = None, year = None):
    pubdate,length = None,None
    try:
        if pub in ['Salon.com','The New York Times']:
            str = re.search(r'/(\d+/\d+/\d+)/',url).group(1)
            pubdate = time.strptime(str, '%Y/%m/%d')
        if pub == 'The New Republic':
            str = re.search(r'i=(\d+)',url).group(1)
            pubdate = time.strptime(str, '%Y%m%d')
        if pub == 'Dallas Observer':
            str = re.search(r'/([\d-]+)/',url).group(1)
            pubdate = time.strptime(str, '%Y-%m-%d')
    except:
        pass
    
    page = urlopen(url)
    p = re.compile(pattern, re.DOTALL)
    if code :
        page = page.decode(code)
    m = p.search( page )
    if m :
        try:
            str = m.group('date').strip()
            if pub == 'Newsweek' and str.find(',') == -1:
                str += ', '+year
            if pub == 'Variety':
                str = str.replace('May','May.')
            str = re.sub(r'(\d+)(st|nd|rd|th)', r'\1', str )
            str = str.replace('Sept.','Sep.')
            str = str.replace('Thurs.','Thu.')
            if date_format.find('-') == -1:
                str = re.sub(r' *-[\w\d ]+,', ',', str )
            str = re.sub(r',? *\d+:\d+.*', '', str )
            pubdate = time.strptime(str.strip(), date_format)
        except ValueError:
            if pub == 'Slate':
                pubdate = time.strptime(str.strip(), '%A, %B %d, %Y')
            elif pub == 'Newsweek':
                pubdate = time.strptime(str, '%B %d, %Y')
            elif pub == 'Time' or pub == 'The Hollywood Reporter':
                pubdate = time.strptime(str, '%b %d, %Y')
            elif pub == 'USA Today':
                pass
            else:
                raise
        except IndexError:
            pass
        except:
            pass
            #raise
        try:
            length = int( m.group('length') )
        except IndexError :
            try:
                content = m.group('content')
                length = word_count( tidy( content) )
            except IndexError :
                pass
    else:
        pass
    return pubdate,length

def clean(quote):
    quote = re.sub(r'([-\',.:&?!" \(\)]+)',' ',quote).strip()
    words = filter(lambda w:len(w)>1, quote.split(' '))
    if len(words)>10: words = words[:10]
    return ' '.join(words)

def choose(reviews, release):
    release_time = time.mktime(release)
    dates = reviews.keys()
    diffs = map(lambda d: abs(time.mktime(d)-release_time), dates )
    m = min(diffs)
    date = dates[diffs.index(m)]
    return date,reviews[date]

def review_date_length_from_archive(pub, title, author, release, quote):
    url = None
    date_format,pattern = Reviewer[pub]
    year = time.strftime("%Y",release)
    start_date = (datetime.datetime( release[0], release[1], release[2]) + datetime.timedelta(-30)).timetuple()
    end_date = (datetime.datetime( release[0], release[1], release[2]) + datetime.timedelta(30)).timetuple()
    if pub in PGArchiver.keys():
        dir = PGArchiver[pub]
        search = 'http://pqasb.pqarchiver.com/%s/' % dir
        search += 'results.html?st=advanced&type=current&sortby=RELEVANCE&at=ALL&Sect=ALL&datetype=6&frommonth=01&fromday=01&fromyear=2002&tomonth=11&today=30&toyear=2006'
        search += '&QryTxt=' + clean(title)
        search = search.replace(' ','+')
        page = urlopen( search )
        page = page[ page.index('Start of search results'):]
        try: page = page[ :page.index('End of search results')]
        except: pass
        res = re.findall(r'<tr>.*?</tr>', page, re.DOTALL)
        if len(res):
            r = []
            for t in res:
                m = re.search(r'href="(/%s/.*?)".*?Preview.*?; +(\w+ \d+, 20\d\d)' % dir, t, re.DOTALL)
                if m: r.append( m.groups() )
                #else:print t
            if len(r):
                res = dict( map(lambda (url,d):(time.strptime(d,'%b %d, %Y'),url), r) )
                date, url = choose(res, release)
                url = 'http://pqasb.pqarchiver.com' + url
                pattern = r'<td>Date:</td>.*?<td>(?P<date>.*?)</td>.*?<td>Text Word Count:</td>.*?<td>(?P<length>\d+)</td>'
                date_format = r'%b %d, %Y'
            else:
                print 'no search result:', search
    elif pub == 'Rolling Stone':
        search = 'http://pqasb.pqarchiver.com/rollingstone/main/'
        search += 'results.html?st=advanced&type=current&sortby=RELEVANCE&at=ALL&Sect=ALL&datetype=6&frommonth=01&fromday=01&fromyear=2002&tomonth=11&today=30&toyear=2006'
        search += '&QryTxt=' + clean(title)
        search = search.replace(' ','+')
        page = urlopen( search )
        page = page[ page.index('Start of search results'):]
        page = page[ :page.index('End of search results')]
        res = re.findall(r'<table>.*?</table>', page, re.DOTALL)
        if len(res):
            r = []
            for t in res:
                m = re.search(r'href="(http://pqasb.pqarchiver.com/rollingstone/.*?)".*?Preview.*?; +(\w+ \d+, 20\d\d)', t, re.DOTALL)
                if m: r.append( m.groups() )
            if len(r):
                res = dict( map(lambda (url,d):(time.strptime(d,'%b %d, %Y'),url), r) )
                date, url = choose(res, release)
                pattern = r'<td>Date:</td>.*?<td>(?P<date>.*?)</td>.*?<td>Text Word Count:</td>.*?<td>(?P<length>\d+)</td>'
                date_format = r'%b %d, %Y'
            else:
                print 'no search result:', search
    elif pub == 'The New York Times':
        url = 'http://query.nytimes.com/search/query?frow=0&n=5&srcht=s&srchst=nyt&submit.x=22&submit.y=12&submit=sub&sort=closest'
        url += '&daterange=period&mon1=%d&day1=%d&year1=%d&mon2=%d&day2=%d&year2=%d' % (start_date[1],start_date[2],start_date[0],end_date[1],end_date[2],end_date[0])
        url += '&query=%s %s' % (clean(title), clean(quote) )
        url = url.replace(' ','+')
        pattern = r'<div class="details">(?P<date>.*?)-.*?<span class="words">(?P<length>\d+) words</span>'
        date_format = r'%B %d, %Y'
    elif pub == 'Portland Oregonian':
        url = 'http://nl.newsbank.com/nl-search/we/Archives?p_product=OR&p_theme=or&p_action=search&p_maxdocs=100'
        url += '&p_text_search-0=%s' % urllib.quote('"%s"' % clean(title))
        url += '&p_field_date-0=YMD_date&p_params_date-0=date:B,E&p_text_date-0=%s' % urllib.quote('%s to %s' % (time.strftime("%m/%d/%Y",start_date),time.strftime("%m/%d/%Y",end_date)))
        url += '&s_dispstring=&p_perpage=1&p_sort=YMD_date:D&xcal_useweights=no' 
        pattern = r'Article \d+ of \d+<br>(?P<date>.*?)<br>.*?Full article: (?P<length>\d+) words'
        date_format = r'%B %d, %Y'
    elif pub == 'The Onion (A.V. Club)':
        search = 'http://www.avclub.com/content/search/av/advanced2?restrict=.site:avclub'
        search += '&search=' + urllib.quote(clean(quote))
        page = urlopen( search )
        m = re.search(r'<a href="(http://www.avclub.com/content/node/\d+)" class="landing_title">', page)
        if m:
            url = m.group(1)
        else:
            print 'no search result:',search
    elif pub == 'Boston Globe':
        search = 'http://www.boston.com/movies/search/results?action=search&search=movie'
        search += '&movie_name=%s' % urllib.quote(title)
        page = urlopen( search )
        m = re.search(r'<a href="(/movies/display\?display=movie&id=\d+)"><strong>.*?</strong></a><BR>', page)
        if m:
            url = 'http://www.boston.com' + m.group(1)
        else:
            print 'no search result: ',search
##    elif pub == 'Los Angeles Times':
##        search = 'http://pqasb.pqarchiver.com/latimes/results.html?st=advanced&type=current&sortby=RELEVANCE&datetype=0&at=ALL&Sect=ALL'
##        search += '&By=%s&QryTxt=%s' % (author,clean(quote) )
##        search = search.replace(' ','+')
##        page = urlopen( search )
##        m = re.search(r'href="(/latimes/access/.*?)"', page)
##        if m:
##            url = 'http://pqasb.pqarchiver.com' + m.group(1)
##            pattern = r'<td>Date:</td>.*?<td>(?P<date>.*?)</td>.*?<td>Text Word Count:</td>.*?<td>(?P<length>\d+)</td>'
##            date_format = r'%B %d, %Y'
##        else:
##            print 'no search result:', search

    elif pub == 'ReelViews':
        page = urlopen('http://www.reelviews.net/archives.html')
        id = re.search(r'<INPUT TYPE="HIDDEN" NAME="id" SIZE="-1" VALUE="(\d+)">', page).group(1)
        search = 'http://search.freefind.com/find.html?id=%s&pid=r&mode=ALL&query=%s' % (id, clean(quote).replace(' ','+') )
        page = urlopen( search )
        m = re.search(r'<a  href="(http://www.reelviews.net/movies/[^"]+\.html)" >',page)
        if m:
            url = m.group(1)
        else:
            print 'no search result:', search
    elif pub == 'Variety':
        search = 'http://www.variety.com/search/SiteReviews?q=%s+%s' % (author, clean(quote) )
        search = search.replace(' ','+')
        page = urlopen( search )
        m = re.search(r'<p id="sortby">.*?<a href="http://www.variety.com/.*?(VE\d+).*?" onmousedown=".*?">',page,re.DOTALL)
        if m:
            url = 'http://www.variety.com/review/%s.html' % m.group(1)
        else:
            print 'no search result:', search
    elif pub == 'The New Yorker':
        search = 'http://www.newyorker.com/search/results?page=1&query=' + urllib.quote(title)
        page = urlopen( search )
        m = re.search(r'<a class="articleTitle" href="(.*?)">.*?class="author">%s</span>.*?<span class="issue">Issue of ([\d-]+)</span>' % author , page, re.DOTALL)
        if m:
            url = 'http://www.newyorker.com' + m.group(1)
            pattern = r'<div class="content">(?P<content>.*?)<div class="author">'
            date,length = parse_date_length(pub, url, pattern, date_format)
            date_format = '%Y-%m-%d'
            date = time.strptime(m.group(2), date_format)
            return date,length
        else:
            print 'no search result:', search
    elif pub == 'Austin Chronicle':
        search = 'http://www.austinchronicle.com/gyrobase/Calendar/Film?Search=' + urllib.quote(title)
        page = urlopen( search )
        m = re.search(r'<A HREF="(/gyrobase/Calendar/Film\?Film=oid%3A\d+)">', page)
        if m:
            url = 'http://www.austinchronicle.com' + m.group(1)
        else:
            print 'no search result:', search
    elif pub == 'ReelViews':
        page = urlopen('http://www.reelviews.net/archives.html')
        m = re.search(r'<INPUT TYPE="HIDDEN" NAME="id" SIZE="-1" VALUE="(\d+)">', page)
        if m:
            id = m.group(1)
            search = 'http://search.freefind.com/find.html\?id=%s&pid=r&mode=ALL&query=%s' % (id,clean(quote).replace(' ','+') )
            page = urlopen( search )
            m = re.search(r'<a  href="(http://www\.reelviews\.net/movies/.*\.html)" >',page)
            if m:
                url = m.group(1)
            else:
                print 'no search result:', search
    elif pub == 'Time':
        url = 'http://www.time.com/time/searchresults?N=55&Ntk=NoBody&Nty=1&Ntx=mode%2Bmatchallpartial&x=18&y=15'
        url += '&from_month=%d&from_day=%d&from_year=%d&to_month=%d&to_day=%d&to_year=%d' % (start_date[1],start_date[2],start_date[0],end_date[1],end_date[2],end_date[0])
        url += '&Ntt=' + clean(quote).replace(' ','+')
        pattern = r'<h3><a href="(/time/magazine/article/[\d,]+\.html)">.*?-->(?P<date>.*?)\|.*?(?P<length>\d+) words'
        date_format = '%b. %d, %Y'
    elif pub == 'TV Guide':
        search = 'http://www.tvguide.com/search/index.aspx?keyword=' + urllib.quote(title)
        page = urlopen( search )
        if page.find('Search Results') != -1:
            m = re.search(r'<a href="(/detail/movie.aspx\?tvobjectid=\d+)">[^?]+'+str(start_date[0]), page)
            if m:
                link = m.group(1)
                page = urlopen( 'http://www.tvguide.com' + link )
        m = re.search(r'href="(/detail/movie.aspx\?tvobjectid=\d+&more=ucmoviereview)"',page)
        if m:
            url = 'http://www.tvguide.com' + m.group(1)
        else:
            print 'no search result:', search
    elif pub == 'The Globe and Mail (Toronto)':
        search = 'http://www.theglobeandmail.com/servlet/Page/document/v5/templates/hub'
        data = {'searchText':quote,
                'searchDateType':'searchDatePreset',
                'searchDatePreset':'all',
                'sort':'Score,sortdate,sorttime',
                'hub':'Search',
                'searchType':'Advanced',
                'from_date':'ValidateDateFrom',
                'to_date':'ValidateDateTo',
                'start_row':1,
                'current_row':1,
                'start_row_offset1':0
                }
        page = urlopen(search, urllib.urlencode(data) )
        m = re.search(r'<h3 class="storyLink"><a href="(/servlet/story/.*?)">.*?</a></h3>', page)
        if m:
            url = 'http://www.theglobeandmail.com' + m.group(1)
            pattern = r'<p class="date">(?P<date>.*?), Page.*?<em class="wordlength">(?P<length>\d+) words</em>'
            date_format = '%A, %B  %d, %Y'
        else:    
            print 'no search result:', search
##    elif pub == 'Empire':
##        search = 'http://www.empireonline.com/search/default.asp?search=%s&searchtype=1' % title
##        search = search.replace(' ','+')
##        page = urlopen( search )
##        m = re.search(r'Popular Matches.*?href="(/reviews/review.asp.*?)"', page, re.DOTALL)
##        if m:
##            url = 'http://www.empireonline.com' + m.group(1)
##            url = url.replace('review.asp','reviewcomplete.asp')
##        else:
##            print 'no search result:', search
    elif pub == 'Chicago Sun-Times':
        search = 'http://rogerebert.suntimes.com/apps/pbcs.dll/classifieds?category=search1'
        data = {'category':'search',
                'Start':1,
                'SearchType':2,
                'Type':'REVIEWS',
                'FromDate':time.strftime("%Y%m%d",start_date),
                'ToDate':time.strftime("%Y%m%d",end_date),
                'SortOrder':'AltTitle',
                'qrender':clean(title).replace(' ',','),
                }
        page = urlopen(search, urllib.urlencode(data) )
        m = re.search(r'name=MyForm.*?href="(/apps/pbcs\.dll/article.*?)"', page, re.DOTALL)
        if m:
            url = 'http://rogerebert.suntimes.com' + m.group(1)
        else:    
            print 'no search result:', search
    elif pub == 'Chicago Reader':
        search = 'https://securesite.chireader.com/cgi-bin/Archive/startsearch2.bat?SelectYear=All'
        search += '&SearchString=' + urllib.quote(clean(title))
        page = urlopen(search)
        res = re.findall(r'Date: (.*?); Word Count: (\d+).', page)
        res = dict( map(lambda (d,l):(time.strptime(d,'%B %d, %Y'),l), res) )
        if len(res):
            return choose( res, release )
        else:
            print 'no search result:', search
    else:
        pass
    if url:
        return parse_date_length(pub, url, pattern, date_format)
    else:
        return None,None



def review_date_length(pub, url, year):
    pubdate,length = None,None
    if not len(url) or not url.startswith('http') or pub in ['Wall Street Journal','Chicago Reader']:
        return None,None
    try:
        if pub in ['Salon.com','The New York Times']:
            str = re.search(r'/(\d+/\d+/\d+)/',url).group(1)
            pubdate = time.strptime(str, '%Y/%m/%d')
        if pub == 'The New Republic':
            str = re.search(r'i=(\d+)',url).group(1)
            pubdate = time.strptime(str, '%Y%m%d')
        if pub == 'Dallas Observer':
            str = re.search(r'/([\d-]+)/',url).group(1)
            pubdate = time.strptime(str, '%Y-%m-%d')
    except:
        pass

    (_, host, path, query, _) = urlparse.urlsplit( url )
    if host in bad_hosts:
        return pubdate,length
    
    if pub == 'The Hollywood Reporter':
        id = re.search(r'id=(\d+)',url).group(1)
        url = 'http://www.hollywoodreporter.com/hr/search/article_display.jsp?vnu_content_id=' + id
    if pub == 'Film Threat':
        id = re.search(r'Id=(\d+)', url).group(1)
        url = 'http://www.filmthreat.com/index.php?section=reviews&Id=%s' % id
    if pub == 'Rolling Stone':
        m = re.search(r'id=(\d+)', url)
        if m:
            url = 'http://www.rollingstone.com/reviews/movie/%s/' % m.group(1)
    if pub == 'Washington Post':
        page = urlopen( url )
        m = re.search(r'<a href=\'(.*?)\'>Read Full Review</a>', page)
        if m: url = m.group(1)
    if pub == 'Boston Globe':
        url = url.replace('ae.boston.com','www.boston.com')

    date_format,pattern = Reviewer[pub]
    code = None
    if pub == 'USA Today':
        if url.find('/reviews/') == -1 :
            pattern = r'<span class="datestamp">(?P<date>[\d/]+).*?<p class="inside-head">(?P<content>.*)'
    if pub == 'Slate':
        code = 'UTF8'
    return parse_date_length(pub, url, pattern, date_format, code, year)    

def date_words(title, pub, score, url, author, release, quote):
    year = time.strftime("%Y",release)
    date,length = review_date_length(pub, url, year)
    if not date or not length:
        date1,length1 = review_date_length_from_archive(pub, title, author, release, quote)
        date = date or date1
        length = length or length1
    if not date:
        if author == 'Michael Phillips' and pub == 'Chicago Tribune':
            author += ' ' + pub
        date = RT.get_review_date( author, title )
    return (pub, score, author, date, length, url)

authors = { 'J. R. Jones':'Jonathan Rosenbaum',
    }
def proc_author( author ):
    author = author.replace('&#039;',"'")
    author = author.replace('.',". ")
    author = author.replace('  '," ")
    if authors.has_key( author ):
        return authors[author]
    return author

def review(title, info = None):
    if not info: info = {}

    page_url, year, score = get_movies( title )
    if not page_url:
        print title, 'not found in metacritic.com'
        return title,{}

    release,info_list = review_info(page_url)
    info['release'], info['score'],info['reviews'],info['page'] = release, score, str(len(info_list)), page_url
    
    wm = work.WorkerManager(min(len(info_list),50))
    for score, pub, author, quote, _, url in info_list:
        if url.find('http://') >= 0 : url = url[url.find('http://'):]
        url = url.replace('&amp;','&')
        author = proc_author( author )
        if pub == 'LA Weekly':
            pub = 'Los Angeles Weekly';
        if not Reviewer.has_key(pub):
            continue
        if not info.has_key(pub):
            wm.add_job( date_words, title, pub, score, url, author, time.strptime(release,"%B %d, %Y"), quote)

    wm.wait_for_complete()
    while True:  
        try:  
            pub, score, author, date, length, url = wm.get_result(False)
            info[pub] = (score, author, date, length, url)
        except Queue.Empty:  
            break            

    return title,info

def craw(movielist, infos ):
    wm = work.WorkerManager(min(len(movielist),20))
    for movie in movielist:
        try:
            old = infos[movie]
        except:
            old = {}
        if len(old)<len(Reviewer)+4:
            wm.add_job(review, movie, old)

    print '%d/%d' % (len(infos),len(movielist))

    try:
        wm.wait_for_complete()
    except:
        pass
    while True:  
        try:  
            title, info = wm.get_result(True, timeout = 1)
            #print '[%d/%d]' % (len(infos),len(movielist)),title
            if len(info):
                infos[title] = info
        except Queue.Empty:
            break
    
    print 'Got %d/%d ' % (len(infos),len(movielist))
    return infos    

def save_with_url(filename, infos):
    lines = []
    pubs = Reviewer.keys()
    pubs.sort()
    for k,v in infos.items():
        line = [k,v['release'],v['score'],v['reviews'],v['page']]
        for pub in pubs:
            if v.has_key(pub):
                score, author, pubdate, words, url = v[pub]
                try:
                    pubdate = time.strftime('%B %d, %Y', pubdate)
                except:
                    pubdate = 'N/A'
                if words:
                    words = str(words)
                else:
                    words = 'N/A'
                line.extend( [score, author, pubdate, words, url] )
            else:
                line.extend( [''] * 5 )
        lines.append( line )
    lines = map(lambda l:Sep.join(l), lines)
    lines.sort()
    open(filename,'w').write( '\n'.join(lines) )

def save(filename, infos):
    lines = []
    pubs = Reviewer.keys()
    pubs.sort()
    for k,v in infos.items():
        line = [k,v['release'], v['score'],v['reviews']]
        for pub in pubs:
            if v.has_key(pub):
                score, author, pubdate, words, url = v[pub]
                try:
                    pubdate = time.strftime('%B %d, %Y', pubdate)
                except:
                    pubdate = 'N/A'
                if words:
                    words = str(words)
                else:
                    words = 'N/A'
                line.extend( [score, author, pubdate, words] )
            else:
                line.extend( [''] * 4 )
        lines.append( line )
    lines = map(lambda l:Sep.join(l), lines)
    lines.sort()
    open(filename,'w').write( '\n'.join(lines) )

def load(filename):
    lines = filter(None,open(filename).read().split('\n'))
    lines = map(lambda l:l.split(Sep), lines)
    infos = {}
    for line in lines:
        info = {}
        title,info['release'], info['score'],info['reviews'],info['page'] = tuple( line[:5] )
        pubs = Reviewer.keys()
        pubs.sort()
        for i in range(len(pubs)):
            score, author, pubdate, words, url = tuple( line[i*5+5:i*5+10] )
            if len(score.strip()):
                try:
                    pubdate = time.strptime(pubdate,'%B %d, %Y')
                except:
                    try:
                        pubdate = time.strptime(pubdate,'%d-%b-%y')
                    except:
                        pubdate = None
                try:
                    words = int(words)
                except:
                    words = None
                info[pubs[i]] = (score, author, pubdate, words, url)
        infos[title] = info
    return infos

def do_craw(filename):
    import socket
    socket.setdefaulttimeout(30)    

    movielist = open(filename).read().split('\n')
    movielist = map(string.strip, movielist)
    movielist = filter(lambda name:name and len(name)>1, movielist)

    try:
        infos = load('review1_'+filename)
    except:
        infos = {}

    #movielist = [m for m in movielist if not infos.has_key(m) ]
    stat( infos )
    infos = craw(movielist, infos)
    stat( infos )
    
    save('review_'+filename, infos)
    save_with_url('review1_'+filename, infos)

def stat(infos):
    date,length,all = 0,0,0
    pubs = Reviewer.keys()
    pubs.sort()
    for k,v in infos.items():
        for pub in pubs:
            if v.has_key(pub):
                score, author, pubdate, words, url = v[pub]
                if pubdate: date += 1
                if words: length += 1
                all += 1
    if all>0: print date, length, all, date*1.0/all, length*1.0/all, 

def pubs():
    pubs = Reviewer.keys()
    pubs.sort()
    print len(pubs)
    print '\n'.join( pubs )

def valid_movie(filename):
    infos = load('review_'+filename)
    titles = infos.keys()
    titles.sort()
    file(filename, 'w').write( '\n'.join(titles) )

if __name__ == "__main__":
    pubs()
    import socket
    socket.setdefaulttimeout(30)    
##    print review('Finding Nemo')
##    print review('Spider-Man')
##    print review('Shrek 2')
##    print review('The Lord of the Rings: The Return of the King')
##    print review('Harry Potter and the Chamber of Secrets')
##    print review('The Matrix Reloaded')
    do_craw('movie_all.txt')
    