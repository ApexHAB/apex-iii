/**
 * sdlogger.cpp
 *
 * Part of the Apex III project
 * http://www.apexhab.org/apex-iii/
 *
 * Priyesh Patel
 *
 * (c) Copyright ApexHAB 2011
 * team@apexhab.org
 */

#include "sdlogger.h"

void sdlogger_log(char* sentence)
{
    SdCard card;
    Fat16 file;

    if(!card.init())
    {
        return;
    }

    if(!Fat16::init(&card))
    {
        return;
    }

    if(!file.open(LOG_FILENAME, O_CREAT | O_APPEND | O_WRITE))
    {
        return;
    }

    file.print(sentence);

    file.close();
}
