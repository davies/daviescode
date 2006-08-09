#
import re,os
import urllib


def downpdf(note):
    url = "http://www.atmel.com" + note[0]
    filename = note[1].replace(":","")[:-1] + ".pdf"
    filename = filename.replace("/","")
    filename = filename.replace("\\","")
    if not os.access(filename,os.F_OK):
        try:
            urllib.urlretrieve(url,filename)
        except:
            print url,filename
            filename = url[url.rfind("/"):]
            urllib.urlretrieve(url,filename)

def downfile(url):
    url = "http://www.atmel.com" + url
    filename = url[-10:]
    if not os.access(filename,os.F_OK):
        urllib.urlretrieve(url,filename)
    
page = file('f:\\notes.htm').read()
p = re.compile(r'''
            (?P<pdf>/dyn/resources/prod_documents/.+?\.pdf).*?
             "bodytext_bold">(?P<title>AVR[0-9][0-9][0-9]: .*?) \<
            ''',re.VERBOSE|re.DOTALL|re.IGNORECASE)
map(downpdf,re.findall(p,page))

#page = page[10000:20000]
p = re.compile(r'''
            (?P<zip>/dyn/resources/prod_documents/AVR.+?\.zip)
            ''',re.VERBOSE|re.DOTALL|re.IGNORECASE)
map(downfile,re.findall(p,page))

