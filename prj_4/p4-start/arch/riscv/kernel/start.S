/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2012 Regents of the University of California
 */ 

#include <asm.h>
#include <csr.h>
#include <asm/sbidef.h>
#include <asm/sbiasm.h>

.section ".entry_function","ax"
ENTRY(_start)
  /* TODO: this is ther kernel's entry point.
   * Same as head.S in Project 3                     
   */ 
  /* Mask all interrupts */
  csrw CSR_SIE, zero
  csrw CSR_SIP, zero

  /*
   * Disable FPU to detect illegal usage of
   * floating point in kernel space
   */
  li t0, SR_FS
  csrc CSR_SSTATUS, t0
  /* set user sum */
  li t0, SR_SUM
  csrs CSR_SSTATUS, t0


  /* Load the global pointer */
  .option push
  .option norelax
  la gp, __global_pointer$
  .option pop

  csrr a0, CSR_MHARTID
  bnez a0, slave_core
  
  /* Clear BSS for flat non-ELF images */
  la a3, __bss_start
  la a4, __BSS_END__
  ble a4, a3, clear_bss_done
clear_bss:
  sd zero, (a3)
  add a3, a3, RISCV_SZPTR
  blt a3, a4, clear_bss
clear_bss_done:

  /* setup C environment */
  la tp, pid0_pcb_m
  ld sp, pid0_stack_m
  csrw CSR_SSCRATCH, x0
  j all
slave_core:                     
  la tp, pid0_pcb_s
  ld sp, pid0_stack_s
  csrw CSR_SSCRATCH, x0

all:
  call main

loop:
  wfi
  j loop

END(_start)
