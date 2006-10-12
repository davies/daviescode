#python
import re,urllib,os
import worker, Queue

def link2filename(link):
    if link.count('.asp?'):
        return link.replace('?','_')+'.html'
    else:
        return link

def find_links( filename ):
    base = ''
    if filename.find('/') > 0: base = filename[:filename.index('/')+1]
    content = open(filename).read()
    href = re.findall(r'href="(.*?)"', content)
    src = re.findall(r'src="(.*?)"', content)
    urls = href + src
    urls = filter(lambda s:not s.startswith('http'), urls)
    urls = filter(lambda s:not s.startswith('mailto'), urls)
    urls = filter(lambda s:not s.startswith('javascript'), urls)
    res = []
    for url in urls:
        if url.startswith('../'):
            res.append(url[3:])
        else:
            res.append(base+url)
    return res


def download(link):
    urllib.urlretrieve('http://219.239.7.49/braveheart/'+link, link2filename(link))
    return link

def mirror(url):
    links = open('urls.log').read().split('\n')
    os.chdir('braveheart')
    worker.Worker.timeout = None
    wm = worker.WorkerManager(50)
    wm.add_job(download, url)
    while True:
        try:
            link = wm.get_result(True, timeout = 90)
            print link
            links.append(link)
            filename = link2filename(link)
            if filename.endswith('html') > 0:
                new_links = find_links( filename )
                new_links = [l for l in new_links if l not in links]
                for l in new_links:
                    wm.add_job(download, l)
        except Queue.Empty:
            break
    open('../urls.log','w').write( '\n'.join(links) )

def test(filename):
    new_links = find_links( filename )
    print '\n'.join(new_links)
if __name__ == '__main__':
    mirror('showcatalog3.asp?CatalogID=1')
    #test('showcatalog3.asp_CatalogID=1.html')