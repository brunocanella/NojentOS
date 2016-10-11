/*
 * NojentOS - Nojo Embedded Task Operating System
 * by Alunos 2016-2 - Sistemas Operacionais Embarcados
 */

#ifndef NOJENTOS_H
#define	NOJENTOS_H

#include "kernel/defines.h"
#include "kernel/types.h"

#include "kernel/timers.h"

#include "kernel/dispatcher.h"

void nojo_init();

void nojo_start();

#endif	/* NOJENTOS_H */

