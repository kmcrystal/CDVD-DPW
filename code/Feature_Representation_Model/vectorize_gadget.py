"""
vectorize_gadget.py -- token embedding for CDVD-DPW (paper Section 3.3.2/3.3.3)

Tokenizes symbolized program slices and turns each slice into a
(SEQ_LEN x EMBED_DIM) matrix of Word2Vec token embeddings:

  - EMBED_DIM = 50   : 50-dimensional token embeddings
  - SEQ_LEN   = 500  : fixed-length token sequences of 500 tokens,
                       with the candidate point positioned at the center
                       (paper Section 3.3.2: "converting to fixed-length
                       token sequences of 500 tokens with candidate points
                       positioned at the center")

Sequences shorter than SEQ_LEN are zero-padded symmetrically; longer ones
are cropped around the candidate-point token position.
"""

import warnings

warnings.filterwarnings("ignore")

import hashlib

import numpy

EMBED_DIM = 50
SEQ_LEN = 500

# Sets for operators
operators3 = {'<<=', '>>='}
operators2 = {
    '->', '++', '--',
    '!~', '<<', '>>', '<=', '>=',
    '==', '!=', '&&', '||', '+=',
    '-=', '*=', '/=', '%=', '&=', '^=', '|='
}
operators1 = {
    '(', ')', '[', ']', '.',
    '+', '-', '*', '&', '/',
    '%', '<', '>', '^', '|',
    '=', ',', '?', ':', ';',
    '{', '}'
}


class GadgetVectorizer:
    """
    Buffers tokenized gadgets, trains Word2Vec (skip-gram, 50-d), then maps
    each gadget to a candidate-point-centered (500 x 50) embedding matrix.
    """

    def __init__(self, vector_length=EMBED_DIM, seq_len=SEQ_LEN, seed=42):
        self.gadgets = []
        self.vector_length = vector_length
        self.seq_len = seq_len
        self.seed = seed

    # ------------------------------------------------------------------ #
    # Tokenization                                                        #
    # ------------------------------------------------------------------ #
    @staticmethod
    def tokenize(line):
        """Break a line of C code into identifiers, keywords and operators."""
        tmp, w = [], []
        i = 0
        while i < len(line):
            if line[i] == ' ':
                tmp.append(''.join(w))
                tmp.append(line[i])
                w = []
                i += 1
            elif line[i:i + 3] in operators3:
                tmp.append(''.join(w))
                tmp.append(line[i:i + 3])
                w = []
                i += 3
            elif line[i:i + 2] in operators2:
                tmp.append(''.join(w))
                tmp.append(line[i:i + 2])
                w = []
                i += 2
            elif line[i] in operators1:
                tmp.append(''.join(w))
                tmp.append(line[i])
                w = []
                i += 1
            else:
                w.append(line[i])
                i += 1
        if w:
            tmp.append(''.join(w))
        res = list(filter(lambda c: c != '', tmp))
        return list(filter(lambda c: c != ' ', res))

    @staticmethod
    def tokenize_gadget(gadget, candidate_pos=0):
        """
        Tokenize all lines of a gadget.
        Returns (tokens, center_index) where center_index is the token
        offset at which the candidate statement begins.
        """
        tokenized = []
        center_index = 0
        for line_no, line in enumerate(gadget):
            if line_no == candidate_pos:
                center_index = len(tokenized)
            tokenized += GadgetVectorizer.tokenize(line)
        center_index = min(center_index, max(len(tokenized) - 1, 0))
        return tokenized, center_index

    # ------------------------------------------------------------------ #
    # Corpus building / model training                                    #
    # ------------------------------------------------------------------ #
    def add_gadget(self, gadget):
        tokenized, _ = GadgetVectorizer.tokenize_gadget(gadget)
        self.gadgets.append(tokenized)

    def train_model(self):
        from gensim.models import Word2Vec
        # min_count=1 prevents out-of-vocabulary errors
        model = Word2Vec(self.gadgets, min_count=1,
                         vector_size=self.vector_length, sg=1,
                         seed=self.seed, workers=1)
        self.embeddings = model.wv
        del model
        del self.gadgets

    # ------------------------------------------------------------------ #
    # Vectorization: candidate-point-centered fixed-length sequence       #
    # ------------------------------------------------------------------ #
    def model_tokens(self, gadget, candidate_pos=0):
        """Return the exact candidate-centered 500-token model layout.

        ``<PAD>`` represents every zero-embedding position.  The resulting
        list is independent of Word2Vec training and is therefore the stable
        key used for exact model-input deduplication.
        """
        tokens, center = GadgetVectorizer.tokenize_gadget(gadget, candidate_pos)
        n = len(tokens)
        half = self.seq_len // 2

        # token window [start, end) around the candidate point
        start = center - half
        end = center + half
        # shift window to stay within the token sequence where possible
        if start < 0:
            end -= start
            start = 0
        if end > n:
            start = max(0, start - (end - n))
            end = n

        layout = ['<PAD>'] * self.seq_len
        offset = half - (center - start)
        offset = max(0, min(offset, self.seq_len - (end - start)))
        layout[offset:offset + (end - start)] = tokens[start:end]
        return layout

    def model_hash(self, gadget, candidate_pos=0):
        layout = self.model_tokens(gadget, candidate_pos)
        return hashlib.sha256('\x1f'.join(layout).encode('utf8')).hexdigest()

    def vectorize(self, gadget, candidate_pos=0):
        layout = self.model_tokens(gadget, candidate_pos)
        vectors = numpy.zeros(shape=(self.seq_len, self.vector_length),
                              dtype=numpy.float32)
        for i, t in enumerate(layout):
            if t in self.embeddings:
                vectors[i] = self.embeddings[t]
        return vectors
