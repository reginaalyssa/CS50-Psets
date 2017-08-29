import cs50


def main():
    """
    Uses Luhn's algorithm to determine if a credit card number
    is a valid American Express, MasterCard, or Visa credit card.
    """

    # get credit card number input
    print("Number: ", end="")
    credit = cs50.get_string()

    # ensure valid input
    while not credit.isdigit():
        print("Retry: ", end="")
        credit = cs50.get_string()

    # compute for the sum based on specs
    credit_sum = 0
    for digit in range(len(credit)):
        if digit % 2 == 1:
            temp = 2 * int(credit[len(credit) - digit - 1])
            while temp > 0:
                credit_sum += temp % 10
                temp //= 10
        else:
            credit_sum += int(credit[len(credit) - digit - 1])

    # determine validity of credit card number
    valid = False
    if credit_sum % 10 == 0:

        # for 16 digit card numbers
        if len(credit) == 16:
            # if first two digits of card number in range [51, 55]
            # classify as MasterCard
            if credit[0] + credit[1] in ["51", "52", "53",
                                         "54", "55"]:
                print("MASTERCARD")
                valid = True

            # if first digit is 4, classify as Visa
            elif credit[0] == "4":
                print("VISA")
                valid = True

        # for 15 digit card numbers
        elif len(credit) == 15:
            # if first two digits of card number is 34 or 37, classify as Amex
            if credit[0] + credit[1] in ["34", "37"]:
                print("AMEX")
                valid = True

        # for 13 digit card numbers
        elif len(credit) == 13:
            # if first digit is 4, classify as Visa
            if credit[0] == "4":
                print("VISA")
                valid = True
    if not valid:
        print("INVALID")


if __name__ == "__main__":
    main()
