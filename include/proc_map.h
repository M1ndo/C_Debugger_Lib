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
#ifdef PROC_MAP_H
#define PROC_MAP_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <../lib/include/sblist.h>

	typedef enum {
		WDP_R = 1 << 0,
		WDP_W = 1 << 1,
		WDP_X = 1 << 2,
		WDP_P = 1 << 3, // Private (Copy Or Write)
		WDP_S = 1 << 4, // Shared
	} map_data_perms;

	typedef struct {
		void* start;
		void* end;
	} map_data_address;

	typedef struct {
		unsigned char major;
		unsigned char minor;
	} map_data_dev;

	typedef struct {
		map_data_address address;
		uint64_t inode;
		uint64_t offset;
		char* pathname;
		map_data_dev dev;
		unsigned char perms;
	} map_data;


	// returns a sblist or (NULL) containing map_data members by parsing /proc/pid/maps
	sblist* process_maps_get(pid_t pid);
	// Free The memory claimed by process_maps_get
	void process_maps_free(sblist* maps);
	// generates a readable string from permission byte
	void process_maps_perm_str(unsigned char perms, char* outbuf5);
	// returns a mapdata for the map the addr is contained in, or NULL
	map_data* find_map_for_addr(sblist* maps, void* addr);
	// returns size of a mapping
	size_t process_map_size(map_data* map);

#ifdef __cplusplus
}
#endif

#pragma RcB2 DEP "proc_map.c"
#endif
