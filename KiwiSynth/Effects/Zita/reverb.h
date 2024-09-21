// ----------------------------------------------------------------------------
//
//  Copyright (C) 2003-2017 Fons Adriaensen <fons@linuxaudio.org>
//    
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// ----------------------------------------------------------------------------

/* Undocumented MAX VALUES
 * rtlow and rtmid: 8.0f
 * delay: 0.1f
 */



#ifndef __REVERB_H
#define __REVERB_H

#include "pareq.h"
#include "daisy_seed.h"

// -----------------------------------------------------------------------


class Diff1
{
private:

    friend class Reverb;
    
    Diff1 (void);
    ~Diff1 (void);

    // SRAM UPDATE
    //void  init (int size, float c);
    void  init (int size, float c, int idx);
    void  fini (void);
    float process (float x);

    // SRAM UPDATE
    int     _idx;
    int     _i;
    float   _c;
    int     _size;
    // SRAM UPDATE
    //float  *_line;
};


// -----------------------------------------------------------------------


class Filt1
{
private:

    friend class Reverb;
    
    Filt1 (void) : _slo (0), _shi (0) {}
    ~Filt1 (void) {}

    void  set_params (float del, float tmf, float tlo, float wlo, float thi, float chi);

    float process (float x)
    {
        _slo += _wlo * (x - _slo) + 1e-10f;
        x += _glo * _slo;
        _shi += _whi * (x - _shi);
	return _gmf * _shi;
    }
    float   _gmf;
    float   _invgmf;
    float   _glo;
    float   _wlo;
    float   _whi;
    float   _slo;
    float   _shi;    
};


// -----------------------------------------------------------------------


class Delay
{
private:

    friend class Reverb;
    
    Delay (void);
    ~Delay (void);

    // SRAM UPDATE
    //void  init (int size);
    void  init (int size, int idx);
    void  fini (void);
    float read (void);
    void write (float x);

    // SRAM UPDATE
    int     _idx;
    int     _i;
    int     _size;
    // SRAM UPDATE
    //float  *_line;
};


// -----------------------------------------------------------------------


class Vdelay
{
private:

    friend class Reverb;
    
    Vdelay (void);
    ~Vdelay (void);

    // SRAM UPDATE
    //void  init (int size);
    void  init (int size, int idx);
    void  fini (void);
    void  set_delay (int del);
    float read (void);
    void write (float x);

    // SRAM UPDATE
    int     _idx;
    int     _ir;
    int     _iw;
    int     _size;
    // SRAM UPDATE
    //float  *_line;
};


// -----------------------------------------------------------------------


class Reverb
{
public:

    Reverb (void);
    ~Reverb (void);

    void init (float fsamp);
    void fini (void);

    void prepare ();
    void process (float *inp [], float *out []);

    void set_delay (float v) { _ipdel = v; _cntA1++; }
    void set_xover (float v) { _xover = v; _cntB1++; }
    void set_rtlow (float v) { _rtlow = v; _invrtlow = 1.0f / v; _cntB1++; }
    void set_rtmid (float v) { _rtmid = v; _invrtmid = 1.0f / v; _cntB1++; _cntC1++; }
    void set_fdamp (float v) { _fdamp = v; _cntB1++; }
    void set_opmix (float v) { _opmix = v; _cntC1++; }
    void set_rgxyz (float v) { _rgxyz = v; _cntC1++; }
    void set_eq1 (float f, float g) { _pareq1.setparam (f, g); }
    void set_eq2 (float f, float g) { _pareq2.setparam (f, g); }

private:


    float   _fsamp;
    float   _finvsamp;
    float   _g;

    Vdelay  _vdelay0;
    Vdelay  _vdelay1;
    Diff1   _diff1 [8];
    Filt1   _filt1 [8];
    Delay   _delay [8];
    
    volatile int _cntA1;
    volatile int _cntB1;
    volatile int _cntC1;
    int     _cntA2;
    int     _cntB2;
    int     _cntC2;

    float   _ipdel;
    float   _xover;
    float   _rtlow;
    float   _invrtlow;
    float   _rtmid;
    float   _invrtmid;
    float   _fdamp;
    float   _opmix;
    float   _rgxyz;

    float   _g0, _d0;
    float   _g1, _d1;

    Pareq   _pareq1;
    Pareq   _pareq2;

    static float _tdiff1 [8];
    static float _tdelay [8];
};


// -----------------------------------------------------------------------


#endif