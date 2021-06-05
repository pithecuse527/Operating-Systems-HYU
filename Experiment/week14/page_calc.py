import sys

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("\nusage: page_calc [VIRTUAL ADDRESS NUMBER]\n\n")
        sys.exit(1)

    address = int(sys.argv[1])  # address string to unsigned int
    page_num = address >> 12    # location of page number
    offset = address & 0xfff    # remainedr will be an offset

    print("The address", address, "contains:")
    print("Page Number:", page_num)
    print("Offset:", offset)
