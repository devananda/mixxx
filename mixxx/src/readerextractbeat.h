/***************************************************************************
                          readerextractbeat.h  -  description
                             -------------------
    begin                : Tue Mar 18 2003
    copyright            : (C) 2003 by Tue & Ken Haste Andersen
    email                : haste@diku.dk
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef READEREXTRACTBEAT_H
#define READEREXTRACTBEAT_H

#include "readerextract.h"
#include "defs.h"
#include <qvaluelist.h>

#ifdef __GNUPLOT__
extern "C" {
    #include "gplot.h"
}
#endif

/** Absolute feature threshold. Currently not used */
const CSAMPLE threshold = 5.;
/** Minimum acceptable BPM */
const CSAMPLE histMinBPM = 60.f;
/** Maximum acceptable BPM */
const CSAMPLE histMaxBPM = 240.f;
/** Down write factor of the histogram, between 0 and 1. */
const CSAMPLE histDownWrite = 0.9999f;
/** Width of gauss/2 used in histogram updates */
const int gaussWidth = 8; 
/** Slack allowed in beat positioning. */
const CSAMPLE beatPrecision = 0.1;

/**
  * Extracts beat information based on peaks in the HFC and a beat probability vector.
  * Based on algorithm in work by Kristoffer Jensen and Tue Haste Andersen. More info
  * comming at http://www.diku.dk/musinf/
  *
  *@author Tue Haste Andersen
  */

class ReaderExtractBeat : public ReaderExtract
{
public: 
    ReaderExtractBeat(ReaderExtract *input, int frameSize, int frameStep, int _histSize);
    ~ReaderExtractBeat();
    /** Reset all buffers */
    void reset();
    /** Reset buffers except histogram (BPM value) */
    void softreset();
    void *getBasePtr();
    CSAMPLE *getBpmPtr();
    int getRate();
    int getChannels();
    int getBufferSize();
    void *processChunk(const int idx, const int start_idx, const int end_idx, bool backwards);
private:
    bool circularValidIndex(int idx, int start, int end, int len);

    /** Buffer indicating if a beat has occoured or not. */
    float *beatBuffer;
    /** Last updated index of beatBuffer containing a beat */
    int beatBufferLastIdx;
    /** Buffer holding bpm values */
    CSAMPLE *bpmBuffer;
    /** Sorted list of peak indexes in HFC */
    typedef QValueList<int> Tpeaks;
    Tpeaks peaks;
    /** Array containing indexes into peaks list. Each element in the array corresponds to the
      * start of a chunk in the peak list. If the index is -1 no peaks are stored in the list
      * for the given chunk */
    Tpeaks::iterator *peakIt;    
    /** Pointer to histogram */
    CSAMPLE *hist;
    /** Pointer to beat interval vector */
    CSAMPLE *beatIntVector;
    /** Size of histogram */
    int histSize;
    /** Histogram interval size, and min and max interval in seconds*/
    CSAMPLE histInterval, histMinInterval, histMaxInterval;
    /** Index of maximum histogram value */
    int histMaxIdx;
    int frameNo;
    int framePerChunk, framePerFrameSize;
    /** Pointer to HFC array */
    CSAMPLE *hfc;

#ifdef __GNUPLOT__
    /** Pointer to gnuplot interface */
    plot_t *gnuplot_hist;
    plot_t *gnuplot_beat;
    plot_t *gnuplot_bpm;
    plot_t *gnuplot_hfc;    
#endif    
};

#endif
