import re,string

def parse_definition( define ):
    info = {}
    lines = define.split('\n')
    for i in range(5):
        info[lines[i*2][:-1]] = lines[i*2+1]
    info['Parameters'] = map(string.strip, info['Parameters'].split(','))
    return info

def parse_example(str):
    str = str.replace(',\n',',')
    str = str.replace('{\n','{')
    str = str.replace('\n}','}')
    examples = filter(None, map(string.strip, re.split(r'\n?\d\)\n', str, re.MULTILINE)))
    print examples
    for i in range(len(examples)):
        s = examples[i]
        examples[i] = s[:s.index('Returns:')-1].split('\n')[1:]
        s = s[s.index('Returns:'):]
        if s.count('\n'):
            examples[i].append(s[len('Returns:'):s.index('\n')])
        else:
            examples[i].append(s[len('Returns:'):].strip())
    print examples
    return examples

def get_one_arg(i, type, arg):
    if type in ['int','double', 'long','string']:
        return '%s Arg%d = %s;' % (type, i, arg)
    if type.startswith('vector'):
        if len(arg) > 2:
            itype = type[type.index('<')+1:-1].strip()
            return '''%s Arr%d[] = %s;%s Arg%d(Arr%d, Arr%d + sizeof(Arr%d)/sizeof(Arr%d[0]));''' % (itype,i,arg, type, i,i,i,i,i)
        else:
            return '%s Arg%d;' % (type, i)

def get_one_test(case, types, args, func):
    s = "void test_case_%d(){" % case
    for i in range(len(types)):
        s += get_one_arg(i, types[i], args[i])
    argv = ', '.join(map(lambda i:'Arg%d'%i, range(len(types)-1)) )
    s += "verify_case<%s>(%d, Arg%d, %s(%s));}\n" % (types[-1], case, len(types)-1, func, argv)
    return s

def get_test_cases(define, examples):
    s = ""
    for i in range(len(examples)):
        s += get_one_test(i, define['Parameters']+[define['Returns']],examples[i], define['Method'])
    return s

def get_run_test(n):
    s = "void run_test(int Case) {"
    for i in range(n):
        s += "if ((Case == -1) || (Case == %d)) test_case_%d();" % (i,i)
    s += "}"
    return s 


def get_test_code(define, examples):
    s = """public:
%s
private:
    template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\\"' << *iter << "\\","; os << " }"; return os.str(); }
    template <typename T> void verify_case(int Case, const T &Expected, const T &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\\tExpected: \\"" << Expected << '\\"' << endl; cerr << "\\tReceived: \\"" << Received << '\\"' << endl; } }
%s
""" % (get_run_test(len(examples)), get_test_cases(define, examples))
    return s

def get_code(define, examples):
    test_code = get_test_code(define, examples)
    return """
#include <vector>
#include <string>
#include <iostream>
using namespace std;

#define SZ(i) ((i).size())
#define FOR(i,a,b) for(int i=(a);i<(b);i++)
#define REP(i,n) FOR(i,0,n)
#define DB double
#define VI vector<int>
#define VS vector<string>

class %s
{
public:
    %s
    {
        %s re;
        return re;
    }
//CUT_BEGIN
%s
//CUT_END
};

//CUT_BEGIN
int main(){
    %s __tester;
    __tester.run_test(-1);
    return 0;
}
//CUT_END
    """ % (define['Class'],define['Method signature'],define['Returns'],
           test_code, define['Class'])
    
def main():
    problem = file('problem.txt').read()
    problem = problem[problem.index('Class:'):problem.index('This problem statement')]
    definition = parse_definition(problem[:problem.index('(be sure your method is public)')].strip())
    examples = parse_example(problem[problem.index('Examples')+len('Examples'):].strip())
    code = get_code(definition, examples)
    file(definition['Class']+'.cpp','w').write(code)

if __name__ == '__main__':
    main()
