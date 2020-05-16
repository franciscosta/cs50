from cs50 import get_string

# Get a string from the user
text = get_string("Text: ")

# The length of the string
length = len(text)

# The three core values
letters = 0
words = 1
sentences = 0

for a in range(length):

    # Calculate the letters
    if (((ord(text[a]) >= 48 and ord(text[a]) <= 48)
        or (ord(text[a]) >= 65 and ord(text[a]) <= 90)
        or (ord(text[a]) >= 97 and ord(text[a]) <= 122))):
        letters += 1

    # Calculate the words
    elif ord(text[a]) == 32:
        words += 1

    # Calculate the sentences
    elif ((ord(text[a]) == 46) or (ord(text[a]) == 33)
        or ((ord(text[a]) == 63) and (ord(text[a]) != 32))):
        sentences += 1

# Calculate the averages
L = (float(letters) / float(words)) * 100
S = (float(sentences) / float(words)) * 100

# The formula itself
index = round(0.0588 * L - 0.296 * S - 15.8)

# The results
if index < 1:
    print("Before grade 1")

elif index >= 16:
    print("Grade 16+")

elif index >= 1 and index < 16:
    print(f"Grade {index}")
