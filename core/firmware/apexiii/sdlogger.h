/**
 * sdlogger.h
 *
 * Part of the Apex III project
 * http://www.apexhab.org/apex-iii/
 *
 * Priyesh Patel
 *
 * (c) Copyright ApexHAB 2011
 * team@apexhab.org
 */

#ifndef SDLOGGER_H
#define SDLOGGER_H

#include "WProgram.h"
#include "Fat16.h"
#include "Fat16util.h"

#define LOG_FILENAME "ALPHA.LOG"

void sdlogger_log(char* sentence);

#endif
