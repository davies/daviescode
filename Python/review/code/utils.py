import urllib,urlparse
import os,os.path,re

SubSep = ';'
Sep = '\t'

def cache_file_name(url):
    (_, host, path, query, _) = urlparse.urlsplit( url )
    filename = re.sub(r'[/\\:*?<>|"]','-',"%s-%s" % (path,query))
    if len(filename) > 200:
        filename = hash(filename)
    return 'cache/%s/%s' % (host, filename)

def urlopen(url, data = None):
    if data:
        filename = cache_file_name(url +data)
    else:
        filename = cache_file_name(url)
    try:
        return open( filename ).read()
    except:
        if data:
            page = urllib.urlopen(url, data).read()
        else:
            page = urllib.urlopen(url).read()
        if not os.path.exists( os.path.dirname(filename) ):
            os.makedirs( os.path.dirname(filename) )
        open( filename, 'w').write( page )
        return page

def clear_cache(url):
    filename = cache_file_name(url)
    os.remove(filename)