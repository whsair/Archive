############################################################
# CMPSC 442: Homework 6
############################################################

student_name = "Hongshuo Wang"

############################################################
# Imports
############################################################

# Include your imports here, if any are used.

from collections import defaultdict
import math
import operator


############################################################
# Section 1: Hidden Markov Models
############################################################

def load_corpus(path):
    output = []

    with open(path, 'r+') as file:
        for line in file:
            l = []
            for word in line.split():
                l.append(tuple(word.split("=")))
            output.append(l)

    return output


def log_unk(smoothing, total_w, total_v):
    return math.log(smoothing / (total_w + smoothing * (total_v + 1)))


def log_w(count_w, smoothing, total_w, total_v):
    return math.log((count_w + smoothing) / (total_w + smoothing * (total_v + 1)))


class Tagger(object):

    def __init__(self, sentences):
        self.tags = ['NOUN', 'VERB', 'ADJ', 'ADV', 'PRON', 'DET', 'ADP', 'NUM', 'CONJ', 'PRT', '.', 'X']

        self.init_probs = defaultdict(float)
        self.trans_probs = defaultdict(lambda: defaultdict(float))
        self.em_probs = defaultdict(lambda: defaultdict(float))

        tag_freq = defaultdict(int)
        initial_tag_freq = defaultdict(int)
        trans_freq = defaultdict(lambda: defaultdict(int))
        em_freq = defaultdict(lambda: defaultdict(int))

        for i in sentences:
            initial_tag_freq[i[0][1]] += 1
            for j in range(len(i)):
                tag_freq[i[j][1]] += 1
                em_freq[i[j][1]][i[j][0]] += 1
                if len(i) - 1 > j:
                    trans_freq[i[j][1]][i[j + 1][1]] += 1

        self.initial_tag_prob(initial_tag_freq, len(sentences))
        self.trans_prob(trans_freq, tag_freq)
        self.emission_prob(tag_freq, em_freq)

    def initial_tag_prob(self, initial_tag_freq, sentences_counter):
        for tag in initial_tag_freq:
            self.init_probs[tag] = math.log(initial_tag_freq[tag] / sentences_counter)

    def trans_prob(self, trans_freq, tag_freq):
        for t_i, t_js in trans_freq.items():
            for t_j in t_js.keys():
                self.trans_probs[t_i][t_j] = math.log(trans_freq[t_i][t_j] / tag_freq[t_i])

    def emission_prob(self, tag_freq, em_freq):
        for tag, ws in em_freq.items():
            for w in ws.keys():
                self.em_probs[tag][w] = log_w(em_freq[tag][w], 1e-10, tag_freq[tag], len(ws.keys()))
            self.em_probs[tag]['<UNK>'] = log_unk(1e-10, tag_freq[tag], len(ws.keys()))

    def most_probable_tags(self, tokens):
        output = []
        for token in tokens:
            max_prob = -math.inf
            max_tag = ""
            for tag, ws in self.em_probs.items():
                if token in ws.keys():
                    if self.em_probs[tag][token] > max_prob:
                        max_prob = self.em_probs[tag][token]
                        max_tag = tag
                else:
                    if self.em_probs[tag]['<UNK>'] > max_prob:
                        max_prob = self.em_probs[tag]['<UNK>']
                        max_tag = tag
            output += [max_tag]

        return output

    def viterbi_tags(self, tokens):

        delta = [{} for i in range(len(tokens))]
        p = [{} for i in range(len(tokens))]

        for tag in self.init_probs.keys():
            if tokens[0] in self.em_probs[tag]:
                delta[0][tag] = self.init_probs[tag] + self.em_probs[tag][tokens[0]]
            else:
                delta[0][tag] = self.init_probs[tag] + self.em_probs[tag]["<UNK>"]

        for j in range(1, len(tokens)):
            for t_j, w in self.em_probs.items():
                best_prob = -math.inf
                best_t_i = ""
                for t_i in delta[j - 1].keys():
                    prob = delta[j - 1][t_i] + self.trans_probs[t_i][t_j]
                    if prob > best_prob:
                        best_prob = prob
                        best_t_i = t_i
                if tokens[j] in w:
                    delta[j][t_j] = best_prob + self.em_probs[t_j][tokens[j]]
                else:
                    delta[j][t_j] = best_prob + self.em_probs[t_j]["<UNK>"]
                p[j][t_j] = best_t_i

        output = [max(delta[len(tokens) - 1].items(), key=operator.itemgetter(1))[0]]

        for i in range(len(p) - 1, 0, -1):
            prev_state = p[i][output[0]]
            output.insert(0, prev_state)

        return output


############################################################
# Section 2: Feedback
############################################################

feedback_question_1 = """
19 hours 
"""

feedback_question_2 = """
The initial parts. It hard to understand the smoothing
Also the last part waste me lost time to get the correct result
"""

feedback_question_3 = """
I like the process of learning hmm but not debugging
more test cases plz
"""
