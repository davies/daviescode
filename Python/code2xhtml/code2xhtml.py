#!/usr/bin/python

# source2xhtml
# by davies.liu@gmail
#  2005/5/20

import re,string
import keyword

def escape( code ):
    code = string.replace(code,'&','&amp;')
    code = string.replace(code,'<','&lt;')
    code = string.replace(code,'>','&gt;')
    return code

def get_class_names( code ):
    class_names = []
    p = re.compile( r"^(\s)*?class\s+"   # before class name
                    r"(?P<classname>[a-zA-Z]\w*)" # class name
                    r"(\s*\()" ,re.MULTILINE )    #after class name
    for m in p.finditer( code ):
        class_names.append( m.group( 'classname' ) )
    return class_names

def get_func_names( code ):
    func_names = []
    p = re.compile( r"^(\s)*?def\s+"   # before func name
                    r"(?P<funcname>[a-zA-Z]\w*)" # func name
                    r"(\s*\()" ,re.MULTILINE )    #after func name
    for m in p.finditer( code ):
        func_names.append( m.group( 'funcname' ) )
    return func_names

def translate2xhtml( code ):

    regex = {
        'comment' :r'(?P<comment>\#.*?$)', # comments
        'dblquot'    :r'''(?P<dblquot>
                       r?(?P<tri>\'\'\'|""")
                           (\\.|[^\\])*?
                         (?P=tri) |     # trible quotation
                       r?(?P<single>[\'"])
                           (\\.|[^\\])*?
                         (?P=single)     # single quotation
                   )''', 
        'keyword' :r'\W(?P<keyword>' + string.join(keyword.kwlist,'|')+r')[\s:(]', # keyword
        'numeric' :r'''
                     (?P<numeric>( # numeric re
                        \d+[.]?|[.]\d)\d*     #  Decimal form
                            (    
                                [lL]|            # long number
                                [eE][+-]?\d+|    # in exp format
                                [jJ]|[+-](\d+[.]?|[.]\d)\d*[jJ] # complex form
                            )?|
                        0[0-7]+|             # Octal form
                        0[xX][\da-fA-F]+     # Hexadecimal form
                    )[^\w.]
                    ''',
    }

    #get class name and func name
    class_names = get_class_names( code )
    if len(class_names) :
        regex['class'] = r'(?P<class>' + string.join(class_names,'|')+r')[\s:(]' # class
    func_names = get_func_names( code )
    if len(func_names) :
        regex['function'] = r'(?P<function>' + string.join(func_names,'|')+r')[\s:(]' # function

    # tranlate code into xhtml format
    p = re.compile( string.join( regex.values(), '|' ) ,re.M|re.S|re.VERBOSE) 
    offset = 0
    new_code = ""
    for m in p.finditer( code ):
        groupname = ""
        for groupname in regex.keys():
            if m.group(groupname ):
                break
        new_code = new_code + code[offset:m.start(groupname)]\
            + '<font class="%s">%s</font>'% (groupname,m.group(groupname))
        offset = m.end(groupname)
    new_code = new_code + code[offset:]

    return new_code

if __name__ == "__main__":
    test_file = 'f:/work/code2xhtml/code2xhtml.py'
    code = open(test_file).read()
    
    code = escape( code )
    xhtml_body = translate2xhtml( code )
    xhtml_header = r"""
<html>
<head>
    <title>%s</title>
    <link rel=stylesheet type="text/css" href="winter.css">
</head>
<body>
    <div class="webcpp">
    <pre>
""" % test_file
    xhtml_footer = """
    </pre>
    </div>
</body>
</html>"""
    output = open(test_file + ".html", 'w')
    output.write( xhtml_header + xhtml_body + xhtml_footer)
    output.close()
    