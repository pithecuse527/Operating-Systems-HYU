import sys
'''
Simple FIFO page replacement algorithm
'''
def FIFO(size, pages):
    #print("##### ##### Going for the FIFO ##### ##### ")
    SIZE = size
    memory = []
    faults = 0
    for page in pages:
        if memory.count(page) == 0 and len(memory) < SIZE:  # still have empty space
            memory.append(page)
            faults += 1
            #print(memory)
        elif memory.count(page) == 0 and len(memory) == SIZE:   # on full
            memory.pop(0)
            memory.append(page)
            faults += 1
            #print(memory)
    #print("##### ##### FIFO Fin. ##### #####\n")
    return faults

'''
LRU algorithm
'''
def LRU(size, pages):
    #print("##### ##### Going for the LRU ##### ##### ")
    SIZE = size
    memory = []
    faults = 0
    for page in pages:
        if memory.count(page) == 0 and len(memory) < SIZE:
            memory.append(page)
            faults += 1
            #print(memory)
        elif memory.count(page) == 0 and len(memory) == SIZE:
            memory.pop(0)
            memory.append(page)
            faults += 1
            #print(memory)
        elif memory.count(page) > 0:
            memory.remove(page)
            memory.append(page)
            #print(memory)
    #print("##### ##### LRU Fin. ##### #####\n")
    return faults


def lookFuture(page, pages):
    i = 0
    while(i < len(pages) and page != pages[i]): i += 1
    return i

'''
Optimal algorithm
'''
def OPT(size,pages):
    print("##### #####  Going for the optimal algo. ##### ##### ")
    SIZE = size
    memory = []
    faults = 0
    for page_i in range(len(pages)):
        if memory.count(pages[page_i]) == 0 and len(memory) < SIZE: # just append when the memory has a space
            memory.append(pages[page_i])
            faults += 1
            print(memory)
        elif memory.count(pages[page_i]) == 0 and len(memory) == SIZE:  # when memory is full,
            to_be_replaced = 0  # location of the memory to be replaced
            max_ = 0     # the maximum location index of the pages array
            for i in range(size):   # find the optimal location to be replaced
                loc_of_page = lookFuture(memory[i], pages[page_i+1:]) # shall we see future?
                if max_ < loc_of_page:
                    max_ = loc_of_page
                    to_be_replaced = i
            memory[to_be_replaced] = pages[page_i]   # replace
            faults += 1
            print(memory)
    print("##### ##### OPT Fin. ##### #####\n")
    return faults

def main(s):
    pages = (7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1)
    size = s
    print('reference string\n', pages)
    print('\nFIFO ->', FIFO(size,pages), 'page faults.\n')
    print('LRU ->', LRU(size,pages), 'page faults.\n')
    print('OPT ->', OPT(size,pages), 'page faults.\n')

if __name__ == "__main__":
    size = int(sys.argv[1])
    if len(sys.argv) != 2:
        print('Usage: python paging.py [number of pages]')
    else:
        main(size)
