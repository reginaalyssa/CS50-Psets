import sys
import cs50


def main():
    """
    Encrypts a message using Vigenere's cipher.

    Usage: vigenere k
    Input p

    where k is an alphabetical string that serves as the key.
    Each character in k corresponds to a number (A and a as 0,
    B and b as 1, ..., and Z and z as 25)
    and
    where p is the string or message to be encrypted.
    """

    # ensure valid arguments
    if len(sys.argv) != 2 or not sys.argv[1].isalpha():
        print("Usage: python vigenere.py k")
        exit(1)

    # key must be all alphabetical
    k = sys.argv[1]

    # get message to encrypt
    print("plaintext: ", end="")
    p = cs50.get_string()

    # number of characters rotated/shifted in message
    shift_count = 0
    print("ciphertext: ", end="")

    # traverse through each character in plaintext
    for c in p:
        # if character is alphabetical, shift it
        if c.isalpha():
            c = rotate(c, k, shift_count)
            shift_count += 1
        print(c, end="")
    print("")


def rotate(c, k, shift_count):
    """
    Returns character c rotated by k[i] positions
    where i is within the range [0, len(k) - 1].
    """

    i = shift_count % len(k)
    msg_f = get_first_letter(c)
    key_f = get_first_letter(k[i])
    c = ord(c)
    key = ord(k[i])

    return chr(((c - msg_f + key - key_f) % 26) + msg_f)


def get_first_letter(c):
    """
    Returns ASCII of the first letter of the
    alphabet depending on case.

    Returns ASCII of "a" if lowercase; "A" if uppercase.
    """

    if c.islower():
        return ord("a")
    else:
        return ord("A")


if __name__ == "__main__":
    main()
