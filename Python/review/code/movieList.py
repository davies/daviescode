import urllib,re,time
import work,Queue

def fetch(page_index):
    try:
        url = 'http://www.rottentomatoes.com/features/stats/index/%d.php' % page_index
        page = urllib.urlopen( url ).read()
        p = re.compile(r'<a href="/m/.*?/">(.*?)</a><br>')
        return p.findall(page)
    except IOError:
        return fetch( page_index )

def fetch_all():
    page_count = 286
    
    wm = work.WorkerManager(min(page_count,50))
    for i in range(page_count):
        wm.add_job( fetch, i+1)

    all = []
    while True:  
        try:  
            ll = wm.get_result(True, timeout = 60)
            all += ll
        except Queue.Empty:  
            break
    
    return all

if __name__ == '__main__':
    all = fetch_all()
    all.sort()
    open('movielist.txt','w').write('\n'.join(all))
