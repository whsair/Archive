############################################################
# CMPSC442: Homework 4
############################################################

student_name = "Hongshuo Wang"

############################################################
# Imports
import collections
import email
import math
import os


############################################################

# Include your imports here, if any are used.

############################################################
# Section 1: Spam Filter
############################################################

def load_tokens(email_path):
    with open(email_path, encoding="utf8") as file:
        message = email.message_from_file(file)
    return [t for i in email.iterators.body_line_iterator(message) for t in i.split()]


def log_unk(smoothing, total_w, total_v):
    return math.log(smoothing / (total_w + smoothing * (total_v + 1)))


def log_w(count_w, smoothing, total_w, total_v):
    return math.log((count_w + smoothing) / (total_w + smoothing * (total_v + 1)))


def log_probs(email_paths, smoothing):
    if smoothing > 1 or smoothing <= 0:
        return None

    w = [t for p in email_paths for t in load_tokens(p)]
    voc = collections.Counter(w)

    output = dict((token, log_w(count_w, smoothing, len(w), len(voc))) for token, count_w in voc.items())
    output['<UNK>'] = log_unk(smoothing, len(w), len(voc))
    return output


class SpamFilter(object):

    def __init__(self, spam_dir, ham_dir, smoothing):
        self.spam_tokens = os.listdir(spam_dir)
        self.ham_tokens = os.listdir(ham_dir)
        self.spam_dict = log_probs(list(spam_dir + "/" + t for t in self.spam_tokens), smoothing)
        self.ham_dict = log_probs(list(ham_dir + "/" + t for t in self.ham_tokens), smoothing)
        self.p_spam = float(len(self.spam_tokens)) / float(len(self.spam_tokens) + len(self.ham_tokens))
        self.p_not_spam = 1 - self.p_spam

    def is_spam(self, email_path):
        log_p_spam = float(math.log(self.p_spam))
        log_p_not_spam = float(math.log(self.p_not_spam))
        counter = collections.Counter(load_tokens(email_path))
        for key, value in counter.items():
            if key in self.spam_dict.keys():
                log_p_spam += self.spam_dict[key] * value
            else:
                log_p_spam += self.spam_dict["<UNK>"] * value

            if key in self.ham_dict.keys():
                log_p_not_spam += self.ham_dict[key] * value
            else:
                log_p_not_spam += self.ham_dict["<UNK>"] * value

        return log_p_not_spam < log_p_spam

    def most_indicative_spam(self, n):
        output = dict()
        common_tokens = set(self.ham_dict.keys()).intersection(set(self.spam_dict.keys()))
        for key in common_tokens:
            p_w = math.exp(self.spam_dict[key]) * self.p_spam + math.exp(self.ham_dict[key]) * self.p_not_spam
            output[key] = self.spam_dict[key] - math.log(p_w)

        return sorted(output, reverse=True, key=output.__getitem__)[:n]

    def most_indicative_ham(self, n):
        output = dict()
        common_tokens = set(self.ham_dict.keys()).intersection(set(self.spam_dict.keys()))
        for key in common_tokens:
            p_w = math.exp(self.spam_dict[key]) * self.p_spam + math.exp(self.ham_dict[key]) * self.p_not_spam
            output[key] = self.ham_dict[key] - math.log(p_w)

        return sorted(output, reverse=True, key=output.__getitem__)[:n]


############################################################
# Section 2: Feedback
############################################################

feedback_question_1 = """
7 hours
"""

feedback_question_2 = """
understand the equation. In last problem, I stuck for a long time because I did't realize that p(x) is changed 
"""

feedback_question_3 = """
The instruction which guide you step by step.
I don't think this assignment need to change anything. Probably, more test case I guess.
"""
