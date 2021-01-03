############################################################
# CMPSC 442: Homework 1
############################################################

student_name = "Hongshuo Wang"

############################################################
# Section 1: Python Concepts
############################################################

python_concepts_question_1 = """
Every object has a fixed type, interpreter doesn’t allow things incompatible with that type 
Strong typing: the type of a value doesn't change in the unexpected way and the objects have the types.
Its interpretor keeps track of all variables type.
using function instance(obj, type) / type(obj) to check the type correctness.
For instance:
'cmpsc' + 442 (type error);
1 + 'fffff' (also fail);

Dynamic typing: the data type check on the running time which is opposed to static typing.
python determines the type automatically based on what data is assigned to it previously.
Variables come into existence when first assigned.
type automatically determined by what object assigned
If assigned again, can always refer to object of any type
Functions have no type signatures
For instance:
i = 1
i = 'cmpsc' (the type of i changes from integer to string)
"""

python_concepts_question_2 = """
According to our lecture, the keys in dictionary must be immutable and the value should be mutable;
however, the list is mutable structure.
Sols: we can change the list to the tuple since tuple is immutable

points_to_names ={(0,0):"home",(1,2):"school",(-1,1):"market"}

"""

python_concepts_question_3 = """
The function concatenate2() is significantly faster than the other for large inputs.
According to runtime complexity analysis, memory allocation, 
the second function return a string concatenated with the elements of iterable using python built in function join;
however, the first one returns a string using loop, and that is less efficient than the second function. 
"""


############################################################
# Section 2: Working with Lists
############################################################

def extract_and_apply(l, p, f):
    return list(f(x) for x in l if p(x))


def concatenate(seqs):
    return list(y for x in seqs for y in x)


def transpose(matrix):
    return list(list(matrix[j][i] for j in range(len(matrix))) for i in range(len(matrix[0])))


############################################################
# Section 3: Sequence Slicing
############################################################

def copy(seq):
    return seq[:]


def all_but_last(seq):
    return seq[:] if len(seq) == 0 else seq[:-1]


def every_other(seq):
    return seq[:: 2]


############################################################
# Section 4: Combinatorial Algorithms
############################################################

def prefixes(seq):
    for i in range(len(seq) + 1):
        yield seq[:i]


def suffixes(seq):
    for i in range(len(seq) + 1):
        yield seq[i:len(seq) + 1]


def slices(seq):
    for x in range(len(seq) + 1):
        for y in range(len(seq) + 1):
            if x < y:
                yield seq[x:y]


############################################################
# Section 5: Text Processing
############################################################

def normalize(text):
    return " ".join(text.lower().strip().split())


def no_vowels(text):
    vowels = ('A', 'a', 'E', 'e', 'I', 'i', 'O', 'o', 'U', 'u')

    for x in text:
        if x in vowels:
            text = text.replace(x, '')

    return text


def digits_to_words(text):
    r_text = ''
    hash_mp = {'0': 'zero', '1': 'one', '2': 'two',
               '3': 'three', '4': 'four', '5': 'five',
               '6': 'six', '7': 'seven', '8': 'eight',
               '9': 'nine'
               }

    for x in text:
        if x in hash_mp:
            r_text += ' ' + hash_mp[x]

    if r_text == '':
        return ''
    else:
        return r_text.strip()


def to_mixed_case(name):
    if name.replace('_', '') == '':
        return ''

    t_list = name.lower().split('_')
    t_list = ' '.join(t_list).split()

    r_name = t_list[0]

    for x in t_list[1:]:
        if len(x) != '':
            r_name += x.capitalize()

    return r_name


############################################################
# Section 6: Polynomials
############################################################

class Polynomial(object):

    def __init__(self, polynomial):
        self.polynomial = tuple(polynomial)

    def get_polynomial(self):
        return self.polynomial

    def __neg__(self):
        t = []
        for x in self.polynomial:
            t.append((-x[0], x[1]))
        return Polynomial(t)

    def __add__(self, other):
        a = list(self.polynomial)
        b = list(other.get_polynomial())

        for x in b:
            a.append(x)

        return Polynomial(a)

    def __sub__(self, other):
        a = list(self.polynomial)
        b = list(other.get_polynomial())

        for x in b:
            a.append((-x[0], x[1]))

        return Polynomial(a)

    def __mul__(self, other):
        a = list(self.polynomial)
        b = list(other.get_polynomial())
        c = []
        for x in a:
            for y in b:
                c.append((x[0] * y[0], x[1] + y[1]))

        return Polynomial(c)

    def __call__(self, x):
        return sum([t[0] * x ** t[1] for t in self.polynomial])

    def simplify(self):
        p = list(self.polynomial)
        output = []
        exp = []
        for x in p:

            if x[1] in exp:
                i = exp.index(x[1])
                output[i][0] += x[0]
            else:
                if x != (0, 0):
                    output.append([x[0], x[1]])
                    exp.append(x[1])
        if len(output) != 0:
            output.sort(key=sort_second, reverse=True)

            self.polynomial = tuple(tuple(x) for x in output if x[0] != 0)

        if len(self.polynomial) == 0:
            self.polynomial = tuple([(0, 0)])

    def __str__(self):
        p = self.polynomial
        output = ''

        if len(p) != 0:

            if p[0][0] < 0:
                if p[0][0] != -1:
                    if p[0][1] == 0:
                        output += str(p[0][0])
                    elif p[0][1] == 1:
                        output += str(p[0][0]) + 'x'
                    else:
                        output += str(p[0][0]) + 'x^' + str(p[0][1])
                else:
                    if p[0][1] == 0:
                        output += '-1'
                    elif p[0][1] == 1:
                        output += '-x'
                    else:
                        output += '-x^' + str(p[0][1])

            elif p[0][0] > 0:
                if p[0][0] != 1:
                    if p[0][1] == 0:
                        output += str(p[0][0])
                    elif p[0][1] == 1:
                        output += str(p[0][0]) + 'x'
                    else:
                        output += str(p[0][0]) + 'x^' + str(p[0][1])
                else:
                    if p[0][1] == 0:
                        output += '1'
                    elif p[0][1] == 1:
                        output += 'x'
                    else:
                        output += 'x^' + str(p[0][1])
            else:
                if p[0][1] == 0:
                    output += '0'
                elif p[0][1] == 1:
                    output += '0x'
                else:
                    output += '0x^' + str(p[0][1])

        for x in p[1:]:

            if x[0] == 0:
                if x[1] == 0:
                    output += ' + 0'
                elif x[1] == 1:
                    output += ' + 0x'
                else:
                    output += ' + 0x^' + str(x[1])

            if x[0] < 0:
                if x[0] != -1:
                    if x[1] == 0:
                        output += ' - ' + str(-x[0])
                    elif x[1] == 1:
                        output += ' - ' + str(-x[0]) + 'x'
                    else:
                        output += ' - ' + str(-x[0]) + 'x^' + str(x[1])
                else:
                    if x[1] == 0:
                        output += ' - 1'

                    elif x[1] == 1:
                        output += ' - x'
                    else:
                        output += ' - x^' + str(x[1])

            if x[0] > 0:

                if x[0] != 1:
                    if x[1] == 0:
                        output += ' + ' + str(x[0])
                    elif x[1] == 1:
                        output += ' + ' + str(x[0]) + 'x'
                    else:
                        output += ' + ' + str(x[0]) + 'x^' + str(x[1])

                else:
                    if x[1] == 0:
                        output += ' + 1'
                    elif x[1] == 1:
                        output += ' + x'
                    else:
                        output += ' + x^' + str(x[1])

        return str(output)


def sort_second(x):
    return x[1]


############################################################
# Section 7: Feedback
############################################################

feedback_question_1 = """
12 hours
"""

feedback_question_2 = """
last problem __str__ function. It is tricky to think of every situation
"""

feedback_question_3 = """
The section 2 part. It more efficient that we learn how program our code in one line
If possible, would like to have more coding section like section 2 or 3 instead of concepts
section like section 1 
"""
