#ifndef PAGE_H
#define PAGE_H

#include <vector>



#define R 0x001
#define W 0x010
#define X 0x100


using namespace std;


class Page{
    uint_8 prem;
    vector<uint_8> page;

    static const int PAGE_SIZE = 4096;
    public:
    Page Page(uint_8 prem = 0x111);
     
    void write(uint_8 addr,int size);

    uint32 read32(uint_8 addr);

};


#endif 