This is a project that was done for my C/C++ course in TalTech.

It consists of two solutions - one in C, one in C++. Since they're both part of the same project, the C version's main function is renamed to avoid conflict.
It must be renamed to be used.

The data structure itself and a few standard functions related to them have been provided as part of the course by the lecturer.

The data structure itself is fairly simple. It is made up of multiple nested linked lists, and stores two-word color names along with some metadata.
It can be represented in the following way:

The first layer of linked lists is a linked list of struct HEADER_B instances. Each HEADER_B contains a cBegin value, which signifies the first letter of the
first word of the color names stored in it. For example, a HEADER_B instance with a cBegin value of 'A' can only contain color names, the first word of which
starts with an A. Every HEADER_B instance also contains a pointer to a HEADER_A linked list.

The second layer of linked lists is a linked list of struct HEADER_A instances. Each HEADER_A also contains a cBegin value, but this signifies the first letter
of the SECOND word of the color names stored in it instead. For example, a HEADER_A instance with a cBegin value of 'A' can only contain color names, the second
word of which starts with an A. Every HEADER_A instance also contains a pointer to a pItems linked list.

The third layer of linked lists is a linked list of struct ITEM9 instances. These are the actual "items" stored in the data structure, and have attributes containing
the data.

This might be a better visualization:

```
HEADER_B[D]
    HEADER_A[K]
        ITEM9[Dark Khaki]
    HEADER_A[L]
        ITEM9[Dark Lava]
        ITEM9[Dark Liver]
HEADER_B[F]
    HEADER_A[R]
        ITEM9[Fuchsia Rose]
        ITEM9[French Rose]
    HEADER_A[W]
        ITEM9[Floral White]
HEADER_B[B]
    HEADER_A[U]
        ITEM9[Burnt Umber]
    HEADER_A[P]
        ITEM9[Blue Pantone]
```
