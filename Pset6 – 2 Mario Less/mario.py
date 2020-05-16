from cs50 import get_int

def main():

    height = get_integer()

    spaces = height - 1
    hashes = 1

    for i in range(height):
        print(" " * spaces, end="")
        print("#" * hashes, end="")
        print("")
        spaces -= 1
        hashes += 1


def get_integer():

    height = 0

    while height < 1 or height > 8:
        # height = int(input("Height: "))
        height = get_int("Height: ")

    return height


main()