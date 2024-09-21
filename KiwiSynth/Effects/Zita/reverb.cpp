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


#include <stdio.h>
#include <string.h>
#include <math.h>
#include "reverb.h"


// -----------------------------------------------------------------------


// SRAM UPDATE
float DSY_SDRAM_BSS vdelayLine[2][4800];
float DSY_SDRAM_BSS diff1Line[8][1600];
float DSY_SDRAM_BSS delayLine[8][12000];


// SRAM UPDATE
Diff1::Diff1 (void) :
    _size (0)//,
    //_line (0)
{
}    


Diff1::~Diff1 (void)
{
    fini ();
}


// SRAM UPDATE
//void Diff1::init (int size, float c)
void Diff1::init (int size, float c, int idx)
{
    // SRAM UPDATE
    _idx = idx;
    _size = size;
    // SRAM UPDATE
    //_line = new float [size];
    memset (diff1Line[_idx], 0, size * sizeof (float));
    _i = 0;
    _c = c;
}


void Diff1::fini (void)
{
    // SRAM UPDATE
    //delete[] _line;
    _size = 0;
    // SRAM UPDATE
    //_line = 0;
}


float Diff1::process (float x)
{
    // SRAM UPDATE
	//float z = _line [_i];
	float z = diff1Line[_idx][_i];
	x -= _c * z;
    // SRAM UPDATE
    //_line [_i] = x;
    diff1Line[_idx][_i] = x;
    if (++_i == _size) _i = 0;
	return z + _c * x;
} 




// -----------------------------------------------------------------------


// SRAM UPDATE
Delay::Delay (void) :
    _size (0)//,
    //_line (0)
{
}


Delay::~Delay (void)
{
    fini ();
}


// SRAM UPDATE
//void Delay::init (int size)
void Delay::init (int size, int idx)
{
    // SRAM UPDATE
    _idx = idx;
    _size = size;
    // SRAM UPDATE
    //_line = new float [size];
    //memset (_line, 0, size * sizeof (float));
    memset (delayLine[_idx], 0, size * sizeof (float));
    _i = 0;
}


void Delay::fini (void)
{
    // SRAM UPDATE
    //delete[] _line;
    _size = 0;
    // SRAM UPDATE
    //_line = 0;
}


float Delay::read (void)
{
    // SRAM UPDATE
	//return _line [_i];
	return delayLine[_idx][_i];
}


void Delay::write (float x)
{
    // SRAM UPDATE
	//_line [_i++] = x;
	delayLine[_idx][_i++] = x;
	if (_i == _size) _i = 0;
}


// -----------------------------------------------------------------------


// SRAM UPDATE
Vdelay::Vdelay (void) :
    _size (0)//,
    //_line (0)
{
}


Vdelay::~Vdelay (void)
{
    fini ();
}


// SRAM UPDATE
//void Vdelay::init (int size)
void Vdelay::init (int size, int idx)
{
    // SRAM UPDATE
    _idx = idx;
    _size = size;
    // SRAM UPDATE
    //_line = new float [size];
    //memset (_line, 0, size * sizeof (float));
    memset (vdelayLine[_idx], 0, size * sizeof (float));
    _ir = 0;
    _iw = 0;
}


void Vdelay::fini (void)
{
    // SRAM UPDATE
    //delete[] _line;
    _size = 0;
    // SRAM UPDATE
    //_line = 0;
}


void Vdelay::set_delay (int del)
{
    _ir = _iw - del;
    if (_ir < 0) _ir += _size;
}


float Vdelay::read (void)
{
    // SRAM UPDATE
	//float x = _line [_ir++];
	float x = vdelayLine[_idx][_ir++];
	if (_ir == _size) _ir = 0;
	return x;
}


void Vdelay::write (float x)
{
    // SRAM UPDATE
	//_line [_iw++] = x;
	vdelayLine[_idx][_iw++] = x;
	if (_iw == _size) _iw = 0;
}


// -----------------------------------------------------------------------


void Filt1::set_params (float del, float invtmf, float invtlo, float wlo, float invthi, float chi)
{
    float g, t;

    _gmf = powf (0.001f, del * invtmf);
    _invgmf = 1.0f / _gmf;
    _glo = powf (0.001f, del * invtlo) * _invgmf - 1.0f;
    _wlo = wlo;    
    g = powf (0.001f, del * invthi) * _invgmf;
    t = (1 - g * g) / (2 * g * g * chi);
    _whi = (sqrtf (1 + 4 * t) - 1) / (2 * t); 
} 

 
// -----------------------------------------------------------------------


float Reverb::_tdiff1 [8] = 
{
    20346e-6f,
    24421e-6f,
    31604e-6f,
    27333e-6f,
    22904e-6f,
    29291e-6f,
    13458e-6f,
    19123e-6f
};


float Reverb::_tdelay [8] = 
{
   153129e-6f,
   210389e-6f,
   127837e-6f,
   256891e-6f,
   174713e-6f,
   192303e-6f,
   125000e-6f,
   219991e-6f
};


Reverb::Reverb (void)
{
}


Reverb::~Reverb (void)
{
    fini ();
}

/*
 R_DELAY, &r_delay   0.04);
 R_XOVER, &r_xover  200.0);
 R_RTLOW, &r_rtlow    3.0);
 R_RTMID, &r_rtmid    2.0);
 R_FDAMP, &r_fdamp  6.0e3);

 R_OPMIX, &r_opmix    0.5);
 R_RGXYZ, &r_rgxyz    0.0);


 R_EQ1FR, &r_parfr  160.0);
 R_EQ1GN, &r_pargn    0.0);

 R_EQ2FR, &r_parfr  2.5e3);
 R_EQ2GN, &r_pargn    0.0);
 */


void Reverb::init (float fsamp)
{
    int i, k1, k2;

    _fsamp = fsamp;
    _finvsamp = 1.0f / fsamp;
    _cntA1 = 1;
    _cntA2 = 0;
    _cntB1 = 1;
    _cntB2 = 0;
    _cntC1 = 1;
    _cntC2 = 0;

    _ipdel = 0.04f;
    _xover = 200.0f;
    _rtlow = 3.0f;
    _invrtlow = 1.0f / _rtlow;
    _rtmid = 2.0f;
    _invrtmid = 1.0f / _rtmid;
    _fdamp = 3e3f;
    _opmix = 0.5f;
    _rgxyz = 0.0f;

    _g0 = _d0 = 0;
    _g1 = _d1 = 0;
    _g = sqrtf (0.125f);

    // SRAM UPDATE
    //_vdelay0.init ((int)(0.1f * _fsamp));
    //_vdelay1.init ((int)(0.1f * _fsamp));
    _vdelay0.init ((int)(0.1f * _fsamp), 0);
    _vdelay1.init ((int)(0.1f * _fsamp), 1);
    for (i = 0; i < 8; i++)
    {
	k1 = (int)(floorf (_tdiff1 [i] * _fsamp + 0.5f));
	k2 = (int)(floorf (_tdelay [i] * _fsamp + 0.5f));
        // SRAM UPDATE
        //_diff1 [i].init (k1, (i & 1) ? -0.6f : 0.6f);
        //_delay [i].init (k2 - k1);
        _diff1 [i].init (k1, (i & 1) ? -0.6f : 0.6f, i);
        _delay [i].init (k2 - k1, i);
    }

    _pareq1.setfsamp (fsamp);
    _pareq2.setfsamp (fsamp);
}


void Reverb::fini (void)
{
    for (int i = 0; i < 8; i++) _delay [i].fini ();
}


void Reverb::prepare ()
{
    int    a, b, c, i, k;
    float  t0, t1, wlo, chi;

    a = _cntA1;
    b = _cntB1;
    c = _cntC1;
    _d0 = _d1 = 0;

    if (a != _cntA2)
    {
        k = (int)(floorf ((_ipdel - 0.020f) * _fsamp + 0.5f));
        _vdelay0.set_delay (k);
        _vdelay1.set_delay (k);
        _cntA2 = a;
    }

    if (b != _cntB2)
    {
         //wlo = 6.2832f * _xover / _fsamp;
         wlo = 6.2832f * _xover * _finvsamp;
	 if (_fdamp > 0.49f * _fsamp) chi = 2;
	 //else chi = 1 - cosf (6.2832f * _fdamp / _fsamp);
	 else chi = 1 - cosf (6.2832f * _fdamp * _finvsamp);
         for (i = 0; i < 8; i++)
	 {
             //_filt1 [i].set_params (_tdelay [i], _rtmid, _rtlow, wlo, 0.5f * _rtmid, chi);
             _filt1 [i].set_params (_tdelay [i], _invrtmid, _invrtlow, wlo, 2.0f * _invrtmid, chi);
	 }
         _cntB2 = b;
    }

    if (c != _cntC2)
    {
	    t0 = (1 - _opmix) * (1 + _opmix);
	    t1 = 0.7f * _opmix * (2 - _opmix) * sqrtf (_invrtmid);
        _d0 = (t0 - _g0);
        _d1 = (t1 - _g1);
        _cntC2 = c;
    }

    _pareq1.prepare ();
    _pareq2.prepare ();
}


void Reverb::process (float *inp [], float *out [])
{	
    int   n;
    float *p0, *p1;
    float *q0, *q1;
    float t, x0, x1, x2, x3, x4, x5, x6, x7;

    p0 = inp [0];
    p1 = inp [1];
    q0 = out [0];
    q1 = out [1];

	_vdelay0.write (p0 [0]);
	_vdelay1.write (p1 [0]);

 	t = 0.3f * _vdelay0.read ();
	x0 = _diff1 [0].process (_delay [0].read () + t);
	x1 = _diff1 [1].process (_delay [1].read () + t);
	x2 = _diff1 [2].process (_delay [2].read () - t);
	x3 = _diff1 [3].process (_delay [3].read () - t);
 	t = 0.3f * _vdelay1.read ();
	x4 = _diff1 [4].process (_delay [4].read () + t);
	x5 = _diff1 [5].process (_delay [5].read () + t);
	x6 = _diff1 [6].process (_delay [6].read () - t);
	x7 = _diff1 [7].process (_delay [7].read () - t);

    t = x0 - x1; x0 += x1;  x1 = t;
    t = x2 - x3; x2 += x3;  x3 = t;
    t = x4 - x5; x4 += x5;  x5 = t;
    t = x6 - x7; x6 += x7;  x7 = t;
    t = x0 - x2; x0 += x2;  x2 = t;
    t = x1 - x3; x1 += x3;  x3 = t;
    t = x4 - x6; x4 += x6;  x6 = t;
    t = x5 - x7; x5 += x7;  x7 = t;
    t = x0 - x4; x0 += x4;  x4 = t;
    t = x1 - x5; x1 += x5;  x5 = t;
    t = x2 - x6; x2 += x6;  x6 = t;
    t = x3 - x7; x3 += x7;  x7 = t;

    _g1 += _d1;
    q0 [0] = _g1 * (x1 + x2);
    q1 [0] = _g1 * (x1 - x2);

    _delay [0].write (_filt1 [0].process (_g * x0));
    _delay [1].write (_filt1 [1].process (_g * x1));
    _delay [2].write (_filt1 [2].process (_g * x2));
    _delay [3].write (_filt1 [3].process (_g * x3));
    _delay [4].write (_filt1 [4].process (_g * x4));
    _delay [5].write (_filt1 [5].process (_g * x5));
    _delay [6].write (_filt1 [6].process (_g * x6));
    _delay [7].write (_filt1 [7].process (_g * x7));

    n = 2;
    _pareq1.process (n, out);
    _pareq2.process (n, out);

    _g0 += _d0;
    q0 [0] += _g0 * p0 [0];
    q1 [0] += _g0 * p1 [0];
}


// -----------------------------------------------------------------------
