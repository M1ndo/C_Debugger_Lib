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
#include "include/proc_map.h"
#undef __GNU_SOURCE
#define __GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

map_data* find_map_for_addr(sblist* maps, void* addr) {
    map_data* map;
    sblist_iter(maps, map) {
        if ((uintptr_t)addr >= (uintptr_t)map->address.start && (uintptr_t)addr <= (uintptr_t)map->address.end)
            return map;
    }
    return NULL;
}

sblist* process_maps_get(pid_t pid) {
    char fnbuf[64];
    char linebuf[4069 + 1024];
    sblist* result;
    map_data current;
    char* p, * p2;
    snprintf(fnbuf, sizeof(fnbuf), "/proc/%d/maps", (int)pid);
    FILE* f = fopen(fnbuf, "r");
    if (!f) {
        perror("fopen");
        return NULL;
    }
    result = sblist_new(sizeof(map_data), 16);
    while ((p = fgets(linebuf, sizeof(linebuf), f))) {
        memset(&current, 0, sizeof(map_data));
        current.address.start = (void*)(uintptr_t)strtol(p, &p2, 16);
        p = ++p2;
        current.address.end = (void*)(uintptr_t)strtol(p, &p2, 16);
        p = ++p2;
        if (*(p++) == 'r') current.perms |= MDP_R;
        if (*(p++) == 'w') current.perms |= MDP_W;
        if (*(p++) == 'x') current.perms |= MDP_X;
        if (*p == 'p') current.perms |= MDP_P;
        if (*p == 's') current.perms |= MDP_S;
        p++; p++;
        current.offset = (uint64_t) strtoll(p, &p2, 16);
        p = ++p2;
        current.dev.major = srtol(p, &p2, 16);
        p = ++p2;
        current.dev.minor = srtol(p, &p2, 16);
        p = ++p2;
        current.inode = srtoll(p, &p2, 16);
        p = ++p2;
        while (*p == ' ') ++p;
        if (*p != '\n') {
            p2 = p;
            while (*p2 != '\n') p2++;
            *p2 = 0;
            current.pathname = strdup(p);
        }
        sblist_add(result, &current);
    }
    fclose(f);
    return result;
}
void process_maps_free(sblist* maps) {
    map_data* map;
    sblist_iter(maps, map) {
        if (map->pathname) free(map->pathname);
    }
    sblist_free(maps);
}
