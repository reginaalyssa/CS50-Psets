import nltk


class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""

        self.positives = set()
        self.negatives = set()

        with open(positives) as lines:
            for line in lines:
                line = line.strip(" \t")
                if line[0] not in ["\n", ";"]:
                    self.positives.add(line.rstrip("\n"))

        with open(negatives) as lines:
            for line in lines:
                line = line.strip(" \t")
                if line[0] not in ["\n", ";"]:
                    self.negatives.add(line.rstrip("\n"))

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""

        score = 0

        # split the text into tokens
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)

        for token in tokens:
            # check each word if positive, negative, neutral
            token = token.lower()
            if token in self.positives:
                score += 1
            elif token in self.negatives:
                score -= 1

        return score
