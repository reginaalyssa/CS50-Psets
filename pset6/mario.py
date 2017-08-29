import cs50


def main():
    """Generates a double half-pyramid of certain height."""

    # get valid height of pyramid
    while(True):
        print("Height: ", end="")
        height = cs50.get_int()
        if(height >= 0 and height <= 23):
            break

    # initialize values for first row
    hashes = 1
    spaces = height - 1

    # print pyramid row by row
    for i in range(height):
        # print left pyramid row
        print(" " * spaces, end="")
        print("#" * hashes, end="")

        # print separator of the pyramids
        print(" " * 2, end="")

        # print right pyramid row
        print("#" * hashes, end="")
        print(" " * spaces)

        # update values for next row
        spaces -= 1
        hashes += 1


if __name__ == "__main__":
    main()
