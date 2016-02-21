#ifndef __SEGMENTATION_H__
#define __SEGMENTATION_H__

    typedef union {
        struct {
            uint32_t LIMIT_15_0     : 16;
            uint32_t BASE_15_0      : 16;
            uint32_t BASE_23_16     : 8;
            uint32_t TYPE           : 4;
            uint32_t PAD0           : 1;
            uint32_t DPL            : 2;
            uint32_t P              : 1;
            uint32_t LIMIT_19_16    : 4;
            uint32_t AVL            : 1;
            uint32_t O              : 1;
            uint32_t X              : 1;
            uint32_t G              : 1;
            uint32_t BASE_31_24     : 8;
        };
        uint32_t val[2];
    } SegDesc;


#endif
