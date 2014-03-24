#!/usr/bin/python

def get_data():
    prev_char = 0
    upper_ints = []
    lower_ints = []
    with open("UnicodeData.txt") as data:
        for line in data:
            tokens = line.split(";")
            char_int = int(tokens[0], 16)

            upper = tokens[12]
            lower = tokens[13]
            if len(upper):
                upper_int = int(upper, 16)
            else:
                upper_int = char_int
            if len(lower):
                lower_int = int(lower, 16)
            else:
                lower_int = char_int

            while (prev_char < char_int - 1):
                prev_char +=1
                if (prev_char <= 65535):
                    upper_ints.append(prev_char);
                    lower_ints.append(prev_char);
            if char_int > 65535:
                break
            upper_ints.append(upper_int);
            lower_ints.append(lower_int);
            prev_char = char_int

    return [upper_ints, lower_ints]

def print_needy(characters):
    for i in range(0,65536):
        if characters[i] != i:
            print i, 
        

if __name__ == '__main__':
    with open("../../src/core/UpperLower.cpp", 'w') as UpperLower_cpp:
        UpperLower_cpp.write('#include "Xli/Unicode.h"\n')
        upper_ints, lower_ints = get_data()

        UpperLower_cpp.write('Xli::Utf16 Xli::Unicode::ToUpperMap[] = {')
        for i in upper_ints:
            UpperLower_cpp.write("%d," % i)
        UpperLower_cpp.write('};\n')

        UpperLower_cpp.write('Xli::Utf16 Xli::Unicode::ToLowerMap[] = {')
        for i in lower_ints:
            UpperLower_cpp.write("%d," % i)
        UpperLower_cpp.write('};\n')

        print_needy(upper_ints)
        print
        print
        print_needy(lower_ints)
