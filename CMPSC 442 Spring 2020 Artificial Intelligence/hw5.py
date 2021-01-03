############################################################
# CMPSC 442: Homework 5
############################################################

student_name = "Hongshuo Wang"

############################################################
# Imports
############################################################

# Include your imports here, if any are used.
import string
import random
import math


############################################################
# Section 1: Markov Models
############################################################

def tokenize(text):
    for token in text:
        if token in string.punctuation:
            text = text.replace(token, " " + token + " ")
    return text.split()


def ngrams(n, tokens):
    if n <= 0:
        return None

    output = list()
    context = tuple()
    tokens = ['<START>'] * (n - 1) + tokens + ['<END>']
    for i in range(n - 1, len(tokens)):
        if n > 1:
            context = tuple(tokens[i - (n - 1):i])
        output.append((context, tokens[i]))
    return output


class NgramModel(object):

    def __init__(self, n):

        self.order = 0
        if n >= 1:
            self.order = n
        self.context_freq = dict()

    def update(self, sentence):
        if sentence:
            n_grams = ngrams(self.order, tokenize(sentence))

            for (c, t) in n_grams:

                if c in self.context_freq:
                    if t in self.context_freq[c]:
                        self.context_freq[c][t] += 1
                    else:
                        self.context_freq[c][t] = 1
                else:
                    self.context_freq[c] = dict()
                    self.context_freq[c][t] = 1

    def prob(self, context, token):

        ft = 0.0
        fc = 0.0
        if context in self.context_freq:
            if token in self.context_freq[context]:
                ft = self.context_freq[context][token]

            fc = sum(self.context_freq[context].values())
        else:
            return 0.0

        return float(ft / fc)

    def random_token(self, context):
        r = random.random()

        if context in self.context_freq:
            freq_context = sum(self.context_freq[context].values())
            sorted_tokens = sorted(self.context_freq[context].items())
            for i, t in enumerate(sorted_tokens):
                freq_above_i = sum(list(sorted_tokens[j][1] for j in range(i)))
                lower_bound = float(freq_above_i / freq_context)
                upper_bound = float((freq_above_i + t[1]) / freq_context)
                if lower_bound <= r < upper_bound:
                    return t[0]
        else:
            return None

    def random_text(self, token_count):
        output = list()
        init_context = tuple()
        if self.order > 1:
            init_context = ("<START>",) * (self.order - 1)

        context = init_context
        for i in range(token_count):
            t = self.random_token(context)
            output.append(t)
            prev = (t,)

            if self.order == 1:
                context = tuple()
            elif t == "<END>":
                context = init_context
            else:
                context = context[1:] + prev
        return " ".join(output)

    def perplexity(self, sentence):
        tokens = tokenize(sentence)
        p_full_joint = 0
        for (c, t) in ngrams(self.order, tokenize(sentence)):
            p_full_joint += math.log(self.prob(c, t))

        m = len(tokens)

        inverse_p = float(1.0 / math.exp(p_full_joint))

        norm_p = inverse_p ** (float(1.0 / (m + 1)))

        return norm_p


def create_ngram_model(n, path):
    ngram_model = NgramModel(n)
    with open(path, "r+") as file:
        for line in file:
            ngram_model.update(line)
    return ngram_model


############################################################
# Section 2: Feedback
############################################################

feedback_question_1 = """
11 hours
"""

feedback_question_2 = """
understanding the instructions and equations
For problem 5,6,7, it's really tricky to understand instruction if we do not look at the lecture note 
"""

feedback_question_3 = """
I like the layout of the project. You can get an realistic model as final result step by step.
I would like to add more test cases.
"""
