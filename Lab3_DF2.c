/*
(C) Copyright Utah State University 2014.  All rights reserved.
No part of this program may be photocopied, reproduced, or translated
to another program language without the prior written consent of
Utah State University.

Time-stamp: " "

(program) template2.c
(status) Under development
(description) Template file for output of samples using the OMAP-L138 Experimenter kit.
(keywords) ECE 5640, Real-Time DSP

(long_d)

This is a template file for use in the lab for ECE 5640.  It is used to provide the
initialization for the OMAP-L138 Experimenter kit that will send samples to the left
and right channels.

This template illustrates the use of interrupt service routines for timing.

(long_d)

(see_also) template2.h L138_aic3106_init.c L138_aic3106_init.h vectors_intr.asm

(bugs)
There are no known bugs at this time
(bugs)

(author) Andrew Pound

(modifications)
Written 04/01/13 by Andrew Pound
Modified: See subversion logs.
(modifications)


  Info: $Id: template2.c 755 2014-02-04 22:08:39Z scott $   USU
  Revision: $Rev: 755 $
 $LastChangedDate: 2014-02-04 15:08:39 -0700 (Tue, 04 Feb 2014) $
 $LastChangedBy: scott $

*/

#include "L138_aic3106_init.h"

// This data structure allows for outputting to both channels.
AIC31_data_type codec_data;

//D_SIZE is the size of the d_k vector.  It is equal to the
//number of stages multiplied by 2 (d(0,k) always equals x(k)),
//so we don't need to carry its value around.
#define D_SIZE (8)

// Function prototype
float dfIIR(float, float*, float*, float*, int);

// The abCoeffs array contains the a and b coefficients for the
// second-order sections in the direct-form structure.  These
// coefficients are in the following order:
// a(2,0), b(2,0), a(1,0), b(1,0), b(0,0), a(2,1), b(2,1), ...
// where a(n, k) is the nth coefficient in the kth stage.
float abCoeffs[] = {0.925474523703572,
                    -1.0000000000000000,
                    -0.845057100107810,
                    0.00000000000000000,
                    1.0000000000000000,
                    0.944936436225674,
                    -1.0000000000000000,
                    -1.465843069884673,
                    0.0000000000000000,
                    1.0000000000000000,
                    0.855163353611161,
                    -1.0000000000000000,
                    -1.265697874274888,
                    0.0000000000000000,
                    1.0000000000000000,
                    0.836282737599746,
                    -1.0000000000000000,
                    -1.010647991647261,
                    0.0000000000000000,
                    1.0000000000000000
};

// When creating this filter as Second-Order Sections, MATLAB
// generates gain coefficients for each stage.  They are
// included here, starting with the gain for stage 0.
float gains[] = {0.194541995843674,
                 0.194541995843674,
                 0.109202904106442,
                 0.109202904106442,
                 0.944060876285923,
};

// The values in d_k are arranged in the following order:
// d_0[n-2], d_0[n-1], d_1[n-2], d_1[n-1], ...
float d_k[D_SIZE] = {0};

interrupt void interrupt4(void)
{
    float x1n, x2n, yn = 0.0;
  
  // Get the next sample of the input.
  x1n = (float)(input_left_sample()); // input from ADC
  x2n = (float)(input_right_sample()); // input from ADC

// Insert processing code here......
  yn = dfIIR(x1n, abCoeffs, d_k, gains, 4);
  
  // output to BOTH right and left channels...
  codec_data.channel[LEFT] = (uint16_t)(yn);
  codec_data.channel[RIGHT] = (uint16_t)(x2n);
  output_sample(codec_data.uint);  // output to L and R DAC

  return;
}

int main(void)
{
// Filter initialization code goes here....

  L138_initialise_intr(FS_48000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB);

  while(1);

}
