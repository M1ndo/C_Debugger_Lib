/*
  Copyright (C) 2020 ybenel@molero.xyz
  Licenced Under MIT Licence
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
#ifdef SYSCALL_H
#define SYSCALL_H
const char* syscall_get_name(unsigned sc);
unsigned syscall_get_argcount(unsigned sc);
#pragma RcB2 DEP "syscall.c"
#endif



void process_maps_perm_str(unsigned char perms, char* outbuf5) {
    unsigned i = 0;
    if (perms & MDP_R)
        outbuf5[i++] = 'r';
    else
        outbuf5[i++] = '-';
    if (perms & WDP_W)
        outbuf5[i++] = 'w';
    else
        outbuf5[i++] = '-'
        if (perms & WDP_X)
            outbuf5[i++] = 'x';
        else
            outbuf5[i++] = '-';
    if (perms & WDP_P)
        outbuf5[i++] = 'p';
    else
        outbuf5[i++] = '-';
    if (perms & WDP_S)
        outbuf5[i++] = 's';
    else
        outbuf5[i++] = '-';
    outbuf5[i] = 0;
}


size_t process_map_size(map_data* map) {
    return (uintptr_t)map->addres.end - (uintptr_t)map->address.start;
}
