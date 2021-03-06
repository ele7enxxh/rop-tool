/************************************************************************/
/* rop-tool - A Return Oriented Programming and binary exploitation     */
/*            tool                                                      */
/*                                                                      */
/* Copyright 2013-2015, -TOSH-                                          */
/* File coded by -TOSH-                                                 */
/*                                                                      */
/* This file is part of rop-tool.                                       */
/*                                                                      */
/* rop-tool is free software: you can redistribute it and/or modify     */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or    */
/* (at your option) any later version.                                  */
/*                                                                      */
/* rop-tool is distributed in the hope that it will be useful,          */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of       */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        */
/* GNU General Public License for more details.                         */
/*                                                                      */
/* You should have received a copy of the GNU General Public License    */
/* along with rop-tool.  If not, see <http://www.gnu.org/licenses/>     */
/************************************************************************/
#ifndef DEF_API_GADGET_H
#define DEF_API_GADGET_H

#include "api/utils.h"
#include "api/binfmt.h"
#include "api/disassemble.h"

typedef struct r_gadget {
  char *gadget;
  int addr_size;
  addr_t addr;
}r_gadget_s;

typedef struct r_gadget_handle {
  r_utils_hash_s *g_hash;
  r_disa_s disa;
  int filter;
  int all;
  int depth;
  r_utils_bytes_s *bad;
}r_gadget_handle_s;

/* gadget.c */
int r_gadget_handle_init(r_gadget_handle_s *g_handle, r_binfmt_arch_e arch, r_disa_flavor_e flavor, int filter, int depth, int all, r_utils_bytes_s *bad);
void r_gadget_handle_close(r_gadget_handle_s *g_handle);
void r_gadget_update(r_gadget_handle_s *g_handle, addr_t addr, u8 *code, u32 code_size);


/* filter.c */
int r_gadget_filter(const char *gadget, r_binfmt_arch_e arch, r_disa_flavor_e flavor);
int r_gadget_filter_strncmp(const char *gadget, const char *filter, int len);

#endif
