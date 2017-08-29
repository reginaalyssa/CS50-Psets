import sys
import cs50


def main():
    """
    Encrypts a message using Caesar's cipher.

    Usage: caesar k
    Input p

    where k is an integer that acts as key for
    the number of shifts/rotations per alphabetical
    character in the message.
    and
    where p is the string or message to be encrypted.
    """

    # ensure valid arguments
    if len(sys.argv) != 2 or not sys.argv[1].isdigit():
        print("Usage: python caesar.py k")
        exit(1)

    # assign only if key is a nonnegative integer
    k = int(sys.argv[1])

    # get message to encrypt
    print("plaintext: ", end="")
    p = cs50.get_string()

    print("ciphertext: ", end="")
    for c in p:
        # rotate only if alphabetical
        if c.isalpha():
            c = rotate(c, k)
        print(c, end="")
    print("")


def rotate(c, k):
    """Returns character c rotated by k positions."""

    first_letter = get_first_letter(c)
    c = ord(c)

    return chr(((c - first_letter + k) % 26) + first_letter)


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
