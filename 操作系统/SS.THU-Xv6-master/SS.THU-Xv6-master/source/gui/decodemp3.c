#include "types.h"
#include "stat.h"
#include "user.h"
#include "math.h"
#include "common.h"
#include "decodemp3.h"
#include "huffman.h"

void decode_info(Bit_stream_struc *bs, struct frame_params *fr_ps)
{
    layer *hdr = fr_ps->header;

    hdr->version = get1bit(bs);
    hdr->lay = 4-getbits(bs,2);
    hdr->error_protection = !get1bit(bs); /* ���󱣻�. TRUE/FALSE */
    hdr->bitrate_index = getbits(bs,4);
    hdr->sampling_frequency = getbits(bs,2);
    hdr->padding = get1bit(bs);
    hdr->extension = get1bit(bs);
    hdr->mode = getbits(bs,2);
    hdr->mode_ext = getbits(bs,2);
    hdr->copyright = get1bit(bs);
    hdr->original = get1bit(bs);
    hdr->emphasis = getbits(bs,2);
}

void III_get_side_info(Bit_stream_struc *bs, struct III_side_info_t *si, struct frame_params *fr_ps)
{
	int ch, gr, i;
	int stereo = fr_ps->stereo;

	si->main_data_begin = getbits(bs, 9);
	if (stereo == 1)
		si->private_bits = getbits(bs,5);
	else
		si->private_bits = getbits(bs,3);

	for (ch=0; ch<stereo; ch++)
		for (i=0; i<4; i++)
			si->ch[ch].scfsi[i] = get1bit(bs);

	for (gr=0; gr<2; gr++) {
		for (ch=0; ch<stereo; ch++) {
			si->ch[ch].gr[gr].part2_3_length = getbits(bs, 12);
			si->ch[ch].gr[gr].big_values = getbits(bs, 9);
			si->ch[ch].gr[gr].global_gain = getbits(bs, 8);
			si->ch[ch].gr[gr].scalefac_compress = getbits(bs, 4);
			si->ch[ch].gr[gr].window_switching_flag = get1bit(bs);
			if (si->ch[ch].gr[gr].window_switching_flag) {
				si->ch[ch].gr[gr].block_type = getbits(bs, 2);
				si->ch[ch].gr[gr].mixed_block_flag = get1bit(bs);
				for (i=0; i<2; i++)
					si->ch[ch].gr[gr].table_select[i] = getbits(bs, 5);
				for (i=0; i<3; i++)
					si->ch[ch].gr[gr].subblock_gain[i] = getbits(bs, 3);

				/* Set region_count parameters since they are implicit in this case. */

				if (si->ch[ch].gr[gr].block_type == 0) {
					//printf("Side info bad: block_type == 0 in split block.\n");
					exit();
				}
				else if (si->ch[ch].gr[gr].block_type == 2
						&& si->ch[ch].gr[gr].mixed_block_flag == 0)
					si->ch[ch].gr[gr].region0_count = 8; /* MI 9; */
				else si->ch[ch].gr[gr].region0_count = 7; /* MI 8; */
					si->ch[ch].gr[gr].region1_count = 20 - si->ch[ch].gr[gr].region0_count;
			}
			else {
				for (i=0; i<3; i++)
					si->ch[ch].gr[gr].table_select[i] = getbits(bs, 5);
				si->ch[ch].gr[gr].region0_count = getbits(bs, 4);
				si->ch[ch].gr[gr].region1_count = getbits(bs, 3);
				si->ch[ch].gr[gr].block_type = 0;
			}
			si->ch[ch].gr[gr].preflag = get1bit(bs);
			si->ch[ch].gr[gr].scalefac_scale = get1bit(bs);
			si->ch[ch].gr[gr].count1table_select = get1bit(bs);
         }
	}
}


struct {
	int l[5];
	int s[3];
} sfbtable = {
	{0, 6, 11, 16, 21},
	{0, 6, 12}
};
int slen[2][16]={
	{0, 0, 0, 0, 3, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4},
	{0, 1, 2, 3, 0, 1, 2, 3, 1, 2, 3, 1, 2, 3, 2, 3}
};
struct {
	int l[23];
	int s[14];
} sfBandIndex[3]= {
	{{0,4,8,12,16,20,24,30,36,44,52,62,74,90,110,134,162,196,238,288,342,418,576},
		{0,4,8,12,16,22,30,40,52,66,84,106,136,192}},
	{{0,4,8,12,16,20,24,30,36,42,50,60,72,88,106,128,156,190,230,276,330,384,576},
		{0,4,8,12,16,22,28,38,50,64,80,100,126,192}},
	{{0,4,8,12,16,20,24,30,36,44,54,66,82,102,126,156,194,240,296,364,448,550,576},
		{0,4,8,12,16,22,30,42,58,78,104,138,180,192}}
};

void III_get_scale_factors(III_scalefac_t *scalefac, struct III_side_info_t *si, int gr, int ch, struct frame_params *fr_ps)
{
	int sfb, i, window;
	struct gr_info_s *gr_info = &(si->ch[ch].gr[gr]);

	if (gr_info->window_switching_flag && (gr_info->block_type == 2)) {
		if (gr_info->mixed_block_flag) { /* MIXED */ /* NEW - ag 11/25 */
			for (sfb = 0; sfb < 8; sfb++)
				(*scalefac)[ch].l[sfb] = hgetbits(
					slen[0][gr_info->scalefac_compress]);
			for (sfb = 3; sfb < 6; sfb++)
				for (window=0; window<3; window++)
					(*scalefac)[ch].s[window][sfb] = hgetbits(
						slen[0][gr_info->scalefac_compress]);
			for (sfb = 6; sfb < 12; sfb++)
				for (window=0; window<3; window++)
					(*scalefac)[ch].s[window][sfb] = hgetbits(
						slen[1][gr_info->scalefac_compress]);
			for (sfb=12,window=0; window<3; window++)
				(*scalefac)[ch].s[window][sfb] = 0;
		}
		else {  /* SHORT*/
			for (i=0; i<2; i++)
				for (sfb = sfbtable.s[i]; sfb < sfbtable.s[i+1]; sfb++)
					for (window=0; window<3; window++)
						(*scalefac)[ch].s[window][sfb] = hgetbits(
							slen[i][gr_info->scalefac_compress]);
				for (sfb=12,window=0; window<3; window++)
					(*scalefac)[ch].s[window][sfb] = 0;
		}
	}
	else {   /* LONG types 0,1,3 */
		for (i=0; i<4; i++) {
			if ((si->ch[ch].scfsi[i] == 0) || (gr == 0))
				for (sfb = sfbtable.l[i]; sfb < sfbtable.l[i+1]; sfb++)
					(*scalefac)[ch].l[sfb] = hgetbits(
						slen[(i<2)?0:1][gr_info->scalefac_compress]);
		}
		(*scalefac)[ch].l[22] = 0;
	}
}

/* �Ѿ��� huffman.c������
struct huffcodetab ht[HTN];
*/
int huffman_initialized = FALSE;

void initialize_huffman()
{
	if (huffman_initialized) return;
	read_decoder_table();
	huffman_initialized = TRUE;
}


void III_hufman_decode(long int is[SBLIMIT][SSLIMIT], struct III_side_info_t *si, int ch, int gr, int part2_start, struct frame_params *fr_ps)
{
   int i, x, y;
   int v, w;
   struct huffcodetab *h;
   int region1Start;
   int region2Start;
   //int bt = (*si).ch[ch].gr[gr].window_switching_flag && ((*si).ch[ch].gr[gr].block_type == 2);

   initialize_huffman();

   /* ��������߽� */

   if ( ((*si).ch[ch].gr[gr].window_switching_flag) &&
        ((*si).ch[ch].gr[gr].block_type == 2) ) {

      /* Region2. */

      region1Start = 36;  /* sfb[9/3]*3=36 */
      region2Start = 576; /* No Region2 for short block case. */
   }


   else {          /* ���ҳ�������µ�����߽�. */

      region1Start = sfBandIndex[fr_ps->header->sampling_frequency]
                           .l[(*si).ch[ch].gr[gr].region0_count + 1]; /* MI */
      region2Start = sfBandIndex[fr_ps->header->sampling_frequency]
                              .l[(*si).ch[ch].gr[gr].region0_count +
                              (*si).ch[ch].gr[gr].region1_count + 2]; /* MI */
      }


   /* ��ȡ��ֵ����Read bigvalues area. */
   for (i=0; i<(*si).ch[ch].gr[gr].big_values*2; i+=2) {
      if      (i<region1Start) h = &ht[(*si).ch[ch].gr[gr].table_select[0]];
      else if (i<region2Start) h = &ht[(*si).ch[ch].gr[gr].table_select[1]];
           else                h = &ht[(*si).ch[ch].gr[gr].table_select[2]];
      huffman_decoder(h, &x, &y, &v, &w);
      is[i/SSLIMIT][i%SSLIMIT] = x;
      is[(i+1)/SSLIMIT][(i+1)%SSLIMIT] = y;
      }

   /* Read count1 area. */
   h = &ht[(*si).ch[ch].gr[gr].count1table_select+32];
   while ((hsstell() < part2_start + (*si).ch[ch].gr[gr].part2_3_length ) &&
     ( i < SSLIMIT*SBLIMIT )) {
      huffman_decoder(h, &x, &y, &v, &w);
      is[i/SSLIMIT][i%SSLIMIT] = v;
      is[(i+1)/SSLIMIT][(i+1)%SSLIMIT] = w;
      is[(i+2)/SSLIMIT][(i+2)%SSLIMIT] = x;
      is[(i+3)/SSLIMIT][(i+3)%SSLIMIT] = y;
      i += 4;
      }

   if (hsstell() > part2_start + (*si).ch[ch].gr[gr].part2_3_length)
   {  i -=4;
      rewindNbits(hsstell()-part2_start - (*si).ch[ch].gr[gr].part2_3_length);
   }

   /* Dismiss stuffing Bits */
   if ( hsstell() < part2_start + (*si).ch[ch].gr[gr].part2_3_length )
      hgetbits( part2_start + (*si).ch[ch].gr[gr].part2_3_length - hsstell());

   /* Zero out rest. */
   for (; i<SSLIMIT*SBLIMIT; i++)
      is[i/SSLIMIT][i%SSLIMIT] = 0;
}


int pretab[22] = {0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,2,2,3,3,3,2,0};

void III_dequantize_sample(long int is[SBLIMIT][SSLIMIT], double xr[SBLIMIT][SSLIMIT], III_scalefac_t *scalefac, struct gr_info_s *gr_info, int ch, struct frame_params *fr_ps)
{
	int ss,sb,cb=0,sfreq=fr_ps->header->sampling_frequency;
	//int stereo = fr_ps->stereo;
	int next_cb_boundary, cb_begin, cb_width = 0, sign;

	/* choose correct scalefactor band per block type, initalize boundary */

	if (gr_info->window_switching_flag && (gr_info->block_type == 2) )
		if (gr_info->mixed_block_flag)
			next_cb_boundary=sfBandIndex[sfreq].l[1];  /* LONG blocks: 0,1,3 */
		else {
			next_cb_boundary=sfBandIndex[sfreq].s[1]*3; /* pure SHORT block */
			cb_width = sfBandIndex[sfreq].s[1];
			cb_begin = 0;
		}
	else
		next_cb_boundary=sfBandIndex[sfreq].l[1];  /* LONG blocks: 0,1,3 */

	/* apply formula per block type */
	for (sb=0 ; sb < SBLIMIT ; sb++) {
		for (ss=0 ; ss < SSLIMIT ; ss++) {
			if ( (sb*18)+ss == next_cb_boundary) { /* Adjust critical band boundary */
				if (gr_info->window_switching_flag && (gr_info->block_type == 2)) {
					if (gr_info->mixed_block_flag) {
						if (((sb*18)+ss) == sfBandIndex[sfreq].l[8])  {
							next_cb_boundary=sfBandIndex[sfreq].s[4]*3;
							cb = 3;
							cb_width = sfBandIndex[sfreq].s[cb+1] -
										sfBandIndex[sfreq].s[cb];
							cb_begin = sfBandIndex[sfreq].s[cb]*3;
						}
						else if (((sb*18)+ss) < sfBandIndex[sfreq].l[8])
							next_cb_boundary = sfBandIndex[sfreq].l[(++cb)+1];
						else {
							next_cb_boundary = sfBandIndex[sfreq].s[(++cb)+1]*3;
							cb_width = sfBandIndex[sfreq].s[cb+1] -
										sfBandIndex[sfreq].s[cb];
							cb_begin = sfBandIndex[sfreq].s[cb]*3;
						}
					}
					else {
						next_cb_boundary = sfBandIndex[sfreq].s[(++cb)+1]*3;
						cb_width = sfBandIndex[sfreq].s[cb+1] -
									sfBandIndex[sfreq].s[cb];
						cb_begin = sfBandIndex[sfreq].s[cb]*3;
					}
				}
	            else /* long blocks */
		           next_cb_boundary = sfBandIndex[sfreq].l[(++cb)+1];
			}

			/* Compute overall (global) scaling. */
			xr[sb][ss] = pow( 2.0 , (0.25 * (gr_info->global_gain - 210.0)));

			/* Do long/short dependent scaling operations. */

			if (gr_info->window_switching_flag && (
				((gr_info->block_type == 2) && (gr_info->mixed_block_flag == 0)) ||
				((gr_info->block_type == 2) && gr_info->mixed_block_flag && (sb >= 2)) )) {

				xr[sb][ss] *= pow(2.0, 0.25 * -8.0 *
						gr_info->subblock_gain[(((sb*18)+ss) - cb_begin)/cb_width]);
				xr[sb][ss] *= pow(2.0, 0.25 * -2.0 * (1.0+gr_info->scalefac_scale)
						* (*scalefac)[ch].s[(((sb*18)+ss) - cb_begin)/cb_width][cb]);
			}
			else {   /* LONG block types 0,1,3 & 1st 2 subbands of switched blocks */
				xr[sb][ss] *= pow(2.0, -0.5 * (1.0+gr_info->scalefac_scale)
								* ((*scalefac)[ch].l[cb]
								+ gr_info->preflag * pretab[cb]));
			}

			/* Scale quantized value. */

			sign = (is[sb][ss]<0) ? 1 : 0;
			xr[sb][ss] *= pow( (double) abs(is[sb][ss]), ((double)4.0/3.0) );
			if (sign) xr[sb][ss] = -xr[sb][ss];
		}
	}
}


void III_reorder(double xr[SBLIMIT][SSLIMIT], double ro[SBLIMIT][SSLIMIT], struct gr_info_s *gr_info, struct frame_params *fr_ps)
{
   int sfreq=fr_ps->header->sampling_frequency;
   int sfb, sfb_start, sfb_lines;
   int sb, ss, window, freq, src_line, des_line;

   for(sb=0;sb<SBLIMIT;sb++)
      for(ss=0;ss<SSLIMIT;ss++)
         ro[sb][ss] = 0;

   if (gr_info->window_switching_flag && (gr_info->block_type == 2)) {
      if (gr_info->mixed_block_flag) {
         for (sb=0 ; sb < 2 ; sb++)
            for (ss=0 ; ss < SSLIMIT ; ss++) {
               ro[sb][ss] = xr[sb][ss];
            }
         for(sfb=3, sfb_start=sfBandIndex[sfreq].s[3]; sfb < 13; sfb++, sfb_start=sfBandIndex[sfreq].s[sfb]) {
               sfb_lines=sfBandIndex[sfreq].s[sfb+1] - sfb_start;
               for(window=0; window<3; window++)
                  for(freq=0;freq<sfb_lines;freq++) {
                     src_line = sfb_start*3 + window*sfb_lines + freq;
                     des_line = (sfb_start*3) + window + (freq*3);
                     ro[des_line/SSLIMIT][des_line%SSLIMIT] =
                                    xr[src_line/SSLIMIT][src_line%SSLIMIT];
               }
         }
      }
      else {
         for(sfb=0,sfb_start=0; sfb < 13; sfb++,sfb_start=sfBandIndex[sfreq].s[sfb]) {
               sfb_lines=sfBandIndex[sfreq].s[sfb+1] - sfb_start;              
               for(window=0; window<3; window++)
                  for(freq=0;freq<sfb_lines;freq++) {
                     src_line = sfb_start*3 + window*sfb_lines + freq;
                     des_line = (sfb_start*3) + window + (freq*3);
                     ro[des_line/SSLIMIT][des_line%SSLIMIT] =
                       xr[src_line/SSLIMIT][src_line%SSLIMIT];
               }
         }
      }
   }
   else {   /*long blocks */
      for (sb=0 ; sb < SBLIMIT ; sb++)
         for (ss=0 ; ss < SSLIMIT ; ss++)
            ro[sb][ss] = xr[sb][ss];
   }
}


void III_stereo(double xr[2][SBLIMIT][SSLIMIT], double lr[2][SBLIMIT][SSLIMIT], III_scalefac_t *scalefac, struct gr_info_s *gr_info, struct frame_params *fr_ps)
{
   int sfreq = fr_ps->header->sampling_frequency;
   int stereo = fr_ps->stereo;
   int ms_stereo = (fr_ps->header->mode == MPG_MD_JOINT_STEREO) &&
                   (fr_ps->header->mode_ext & 0x2);
   int i_stereo = (fr_ps->header->mode == MPG_MD_JOINT_STEREO) &&
                  (fr_ps->header->mode_ext & 0x1);
   int sfb;
   int i,j,sb,ss,ch,is_pos[576];
   double is_ratio[576];

   /* intialization */
   for ( i=0; i<576; i++ )
      is_pos[i] = 7;

   if ((stereo == 2) && i_stereo )
   {  if (gr_info->window_switching_flag && (gr_info->block_type == 2))
      {  if( gr_info->mixed_block_flag )
         {  int max_sfb = 0;

            for ( j=0; j<3; j++ )
            {  int sfbcnt;
               sfbcnt = 2;
               for( sfb=12; sfb >=3; sfb-- )
               {  int lines;
                  lines = sfBandIndex[sfreq].s[sfb+1]-sfBandIndex[sfreq].s[sfb];
                  i = 3*sfBandIndex[sfreq].s[sfb] + (j+1) * lines - 1;
                  while ( lines > 0 )
                  {  if ( xr[1][i/SSLIMIT][i%SSLIMIT] != 0.0 )
                     {  sfbcnt = sfb;
                        sfb = -10;
                        lines = -10;
                     }
                     lines--;
                     i--;
                  }
               }
               sfb = sfbcnt + 1;

               if ( sfb > max_sfb )
                  max_sfb = sfb;

               while( sfb<12 )
               {  sb = sfBandIndex[sfreq].s[sfb+1]-sfBandIndex[sfreq].s[sfb];
                  i = 3*sfBandIndex[sfreq].s[sfb] + j * sb;
                  for ( ; sb > 0; sb--)
                  {  is_pos[i] = (*scalefac)[1].s[j][sfb];
                     if ( is_pos[i] != 7 )
                        is_ratio[i] = tan( is_pos[i] * (PI / 12));
                     i++;
                  }
                  sfb++;
               }
               sb = sfBandIndex[sfreq].s[11]-sfBandIndex[sfreq].s[10];
               sfb = 3*sfBandIndex[sfreq].s[10] + j * sb;
               sb = sfBandIndex[sfreq].s[12]-sfBandIndex[sfreq].s[11];
               i = 3*sfBandIndex[sfreq].s[11] + j * sb;
               for ( ; sb > 0; sb-- )
               {  is_pos[i] = is_pos[sfb];
                  is_ratio[i] = is_ratio[sfb];
                  i++;
               }
             }
             if ( max_sfb <= 3 )
             {  i = 2;
                ss = 17;
                sb = -1;
                while ( i >= 0 )
                {  if ( xr[1][i][ss] != 0.0 )
                   {  sb = i*18+ss;
                      i = -1;
                   } else
                   {  ss--;
                      if ( ss < 0 )
                      {  i--;
                         ss = 17;
                      }
                   }
                }
                i = 0;
                while ( sfBandIndex[sfreq].l[i] <= sb )
                   i++;
                sfb = i;
                i = sfBandIndex[sfreq].l[i];
                for ( ; sfb<8; sfb++ )
                {  sb = sfBandIndex[sfreq].l[sfb+1]-sfBandIndex[sfreq].l[sfb];
                   for ( ; sb > 0; sb--)
                   {  is_pos[i] = (*scalefac)[1].l[sfb];
                      if ( is_pos[i] != 7 )
                         is_ratio[i] = tan( is_pos[i] * (PI / 12));
                      i++;
                   }
                }
            }
         } else
         {  for ( j=0; j<3; j++ )
            {  int sfbcnt;
               sfbcnt = -1;
               for( sfb=12; sfb >=0; sfb-- )
               {  int lines;
                  lines = sfBandIndex[sfreq].s[sfb+1]-sfBandIndex[sfreq].s[sfb];
                  i = 3*sfBandIndex[sfreq].s[sfb] + (j+1) * lines - 1;
                  while ( lines > 0 )
                  {  if ( xr[1][i/SSLIMIT][i%SSLIMIT] != 0.0 )
                     {  sfbcnt = sfb;
                        sfb = -10;
                        lines = -10;
                     }
                     lines--;
                     i--;
                  }
               }
               sfb = sfbcnt + 1;
               while( sfb<12 )
               {  sb = sfBandIndex[sfreq].s[sfb+1]-sfBandIndex[sfreq].s[sfb];
                  i = 3*sfBandIndex[sfreq].s[sfb] + j * sb;
                  for ( ; sb > 0; sb--)
                  {  is_pos[i] = (*scalefac)[1].s[j][sfb];
                     if ( is_pos[i] != 7 )
                        is_ratio[i] = tan( is_pos[i] * (PI / 12));
                     i++;
                  }
                  sfb++;
               }

               sb = sfBandIndex[sfreq].s[11]-sfBandIndex[sfreq].s[10];
               sfb = 3*sfBandIndex[sfreq].s[10] + j * sb;
               sb = sfBandIndex[sfreq].s[12]-sfBandIndex[sfreq].s[11];
               i = 3*sfBandIndex[sfreq].s[11] + j * sb;
               for ( ; sb > 0; sb-- )
               {  is_pos[i] = is_pos[sfb];
                  is_ratio[i] = is_ratio[sfb];
                  i++;
               }
            }
         }
      } else
      {  i = 31;
         ss = 17;
         sb = 0;
         while ( i >= 0 )
         {  if ( xr[1][i][ss] != 0.0 )
            {  sb = i*18+ss;
               i = -1;
            } else
            {  ss--;
               if ( ss < 0 )
               {  i--;
                  ss = 17;
               }
            }
         }
         i = 0;
         while ( sfBandIndex[sfreq].l[i] <= sb )
            i++;
         sfb = i;
         i = sfBandIndex[sfreq].l[i];
         for ( ; sfb<21; sfb++ )
         {  sb = sfBandIndex[sfreq].l[sfb+1] - sfBandIndex[sfreq].l[sfb];
            for ( ; sb > 0; sb--)
            {  is_pos[i] = (*scalefac)[1].l[sfb];
               if ( is_pos[i] != 7 )
                  is_ratio[i] = tan( is_pos[i] * (PI / 12));
               i++;
            }
         }
         sfb = sfBandIndex[sfreq].l[20];
         for ( sb = 576 - sfBandIndex[sfreq].l[21]; sb > 0; sb-- )
         {  is_pos[i] = is_pos[sfb];
            is_ratio[i] = is_ratio[sfb];
            i++;
         }
      }
   }

   for(ch=0;ch<2;ch++)
      for(sb=0;sb<SBLIMIT;sb++)
         for(ss=0;ss<SSLIMIT;ss++)
            lr[ch][sb][ss] = 0;

   if (stereo==2)
      for(sb=0;sb<SBLIMIT;sb++)
         for(ss=0;ss<SSLIMIT;ss++) {
            i = (sb*18)+ss;
            if ( is_pos[i] == 7 ) {
               if ( ms_stereo ) {
                  lr[0][sb][ss] = (xr[0][sb][ss]+xr[1][sb][ss])/1.41421356;
                  lr[1][sb][ss] = (xr[0][sb][ss]-xr[1][sb][ss])/1.41421356;
               }
               else {
                  lr[0][sb][ss] = xr[0][sb][ss];
                  lr[1][sb][ss] = xr[1][sb][ss];
               }
            }
            else if (i_stereo ) {
               lr[0][sb][ss] = xr[0][sb][ss] * (is_ratio[i]/(1+is_ratio[i]));
               lr[1][sb][ss] = xr[0][sb][ss] * (1/(1+is_ratio[i]));
            }
            else {
               printf(0,"Error in streo processing\n");
            }
         }
   else  /* mono , bypass xr[0][][] to lr[0][][]*/
      for(sb=0;sb<SBLIMIT;sb++)
         for(ss=0;ss<SSLIMIT;ss++)
            lr[0][sb][ss] = xr[0][sb][ss];

}


double Ci[8]={-0.6,-0.535,-0.33,-0.185,-0.095,-0.041,-0.0142,-0.0037};
void III_antialias(double xr[SBLIMIT][SSLIMIT], double hybridIn[SBLIMIT][SSLIMIT], struct gr_info_s *gr_info, struct frame_params *fr_ps)
{
   static int    init = 1;
   static double ca[8],cs[8];
   double        bu,bd;  /* upper and lower butterfly inputs */
   int           ss,sb,sblim;

   if (init) {
      int i;
      double    sq;
      for (i=0;i<8;i++) {
         sq=sqrt(1.0+Ci[i]*Ci[i]);
         cs[i] = 1.0/sq;
         ca[i] = Ci[i]/sq;
      }
      init = 0;
   }

   /* clear all inputs */

    for(sb=0;sb<SBLIMIT;sb++)
       for(ss=0;ss<SSLIMIT;ss++)
          hybridIn[sb][ss] = xr[sb][ss];

   if  (gr_info->window_switching_flag && (gr_info->block_type == 2) &&
       !gr_info->mixed_block_flag ) return;

   if ( gr_info->window_switching_flag && gr_info->mixed_block_flag &&
     (gr_info->block_type == 2))
      sblim = 1;
   else
      sblim = SBLIMIT-1;

   /* 31 alias-reduction operations between each pair of sub-bands */
   /* with 8 butterflies between each pair                         */

   for(sb=0;sb<sblim;sb++)
      for(ss=0;ss<8;ss++) {
         bu = xr[sb][17-ss];
         bd = xr[sb+1][ss];
         hybridIn[sb][17-ss] = (bu * cs[ss]) - (bd * ca[ss]);
         hybridIn[sb+1][ss] = (bd * cs[ss]) + (bu * ca[ss]);
         }
}


void inv_mdct(double in[18], double out[36], int block_type)
{
	int     i,m,N,p;
	double  tmp[12],sum;
	static  double  win[4][36];
	static  int init=0;
	static  double COS[4*36];

    if(init==0){

    /* type 0 */
      for(i=0;i<36;i++)
         win[0][i] = sin( PI/36 *(i+0.5) );

    /* type 1*/
      for(i=0;i<18;i++)
         win[1][i] = sin( PI/36 *(i+0.5) );
      for(i=18;i<24;i++)
         win[1][i] = 1.0;
      for(i=24;i<30;i++)
         win[1][i] = sin( PI/12 *(i+0.5-18) );
      for(i=30;i<36;i++)
         win[1][i] = 0.0;

    /* type 3*/
      for(i=0;i<6;i++)
         win[3][i] = 0.0;
      for(i=6;i<12;i++)
         win[3][i] = sin( PI/12 *(i+0.5-6) );
      for(i=12;i<18;i++)
         win[3][i] =1.0;
      for(i=18;i<36;i++)
         win[3][i] = sin( PI/36*(i+0.5) );

    /* type 2*/
      for(i=0;i<12;i++)
         win[2][i] = sin( PI/12*(i+0.5) ) ;
      for(i=12;i<36;i++)
         win[2][i] = 0.0 ;

      for (i=0; i<4*36; i++)
         COS[i] = cos(PI/(2*36) * i);

      init++;
    }

    for(i=0;i<36;i++)
       out[i]=0;

    if(block_type == 2){
       N=12;
       for(i=0;i<3;i++){
          for(p= 0;p<N;p++){
             sum = 0.0;
             for(m=0;m<N/2;m++)
                sum += in[i+3*m] * cos( PI/(2*N)*(2*p+1+N/2)*(2*m+1) );
             tmp[p] = sum * win[block_type][p] ;
          }
          for(p=0;p<N;p++)
             out[6*i+p+6] += tmp[p];
       }
    }
    else{
      N=36;
      for(p= 0;p<N;p++){
         sum = 0.0;
         for(m=0;m<N/2;m++)
           sum += in[m] * COS[((2*p+1+N/2)*(2*m+1))%(4*36)];
         out[p] = sum * win[block_type][p];
      }
    }
}


void III_hybrid(double fsIn[SSLIMIT], double tsOut[SSLIMIT], int sb, int ch, struct gr_info_s *gr_info, struct frame_params *fr_ps)
/* fsIn:freq samples per subband in */
/* tsOut:time samples per subband out */
{
   int ss;
   double rawout[36];
   static double prevblck[2][SBLIMIT][SSLIMIT];
   static int init = 1;
   int bt;

   if (init) {
      int i,j,k;

      for(i=0;i<2;i++)
         for(j=0;j<SBLIMIT;j++)
            for(k=0;k<SSLIMIT;k++)
               prevblck[i][j][k]=0.0;
      init = 0;
   }

   bt = (gr_info->window_switching_flag && gr_info->mixed_block_flag &&
          (sb < 2)) ? 0 : gr_info->block_type;

   inv_mdct( fsIn, rawout, bt);

   /* overlap addition */
   for(ss=0; ss<SSLIMIT; ss++) {
      tsOut[ss] = rawout[ss] + prevblck[ch][sb][ss];
      prevblck[ch][sb][ss] = rawout[ss+18];
   }
}


/* create in synthesis filter */
void create_syn_filter(double filter[64][SBLIMIT])
{
	register int i,k;

	for (i=0; i<64; i++)
		for (k=0; k<32; k++) {
			if ((filter[i][k] = 1e9*cos((double)((PI64*i+PI4)*(2*k+1)))) >= 0)
				filter[i][k] = (int)(filter[i][k]+0.5);
				//modf(filter[i][k]+0.5, &filter[i][k]);
			else
				filter[i][k] = (int)(filter[i][k]-0.5);
				//modf(filter[i][k]-0.5, &filter[i][k]);
			filter[i][k] *= 1e-9;
		}
}



/* read in synthesis window */
void read_syn_window(double window[HAN_SIZE])
{
	double gb_window[HAN_SIZE] = {0.0000000000, -0.0000152590, -0.0000152590, -0.0000152590,
		-0.0000152590, -0.0000152590, -0.0000152590, -0.0000305180,
		-0.0000305180, -0.0000305180, -0.0000305180, -0.0000457760,
		-0.0000457760, -0.0000610350, -0.0000610350, -0.0000762940,
		-0.0000762940, -0.0000915530, -0.0001068120, -0.0001068120,
		-0.0001220700, -0.0001373290, -0.0001525880, -0.0001678470,
		-0.0001983640, -0.0002136230, -0.0002441410, -0.0002593990,
		-0.0002899170, -0.0003204350, -0.0003662110, -0.0003967290,
		-0.0004425050, -0.0004730220, -0.0005340580, -0.0005798340,
		-0.0006256100, -0.0006866460, -0.0007476810, -0.0008087160,
		-0.0008850100, -0.0009613040, -0.0010375980, -0.0011138920,
		-0.0012054440, -0.0012969970, -0.0013885500, -0.0014801030,
		-0.0015869140, -0.0016937260, -0.0017852780, -0.0019073490,
		-0.0020141600, -0.0021209720, -0.0022430420, -0.0023498540,
		-0.0024566650, -0.0025787350, -0.0026855470, -0.0027923580,
		-0.0028991700, -0.0029907230, -0.0030822750, -0.0031738280,
		0.0032501220, 0.0033264160, 0.0033874510, 0.0034332280,
		0.0034637450, 0.0034790040, 0.0034790040, 0.0034637450,
		0.0034179690, 0.0033721920, 0.0032806400, 0.0031738280,
		0.0030517580, 0.0028839110, 0.0027008060, 0.0024871830,
		0.0022277830, 0.0019378660, 0.0016174320, 0.0012664790,
		0.0008697510, 0.0004425050, -0.0000305180, -0.0005493160,
		-0.0010986330, -0.0016937260, -0.0023345950, -0.0030059810,
		-0.0037231450, -0.0044860840, -0.0052948000, -0.0061187740,
		-0.0070037840, -0.0079193120, -0.0088653560, -0.0098419190,
		-0.0108489990, -0.0118865970, -0.0129394530, -0.0140228270,
		-0.0151214600, -0.0162353520, -0.0173492430, -0.0184631350,
		-0.0195770260, -0.0206909180, -0.0217895510, -0.0228576660,
		-0.0239105220, -0.0249328610, -0.0259094240, -0.0268402100,
		-0.0277252200, -0.0285339360, -0.0292816160, -0.0299377440,
		-0.0305328370, -0.0310058590, -0.0313873290, -0.0316619870,
		-0.0318145750, -0.0318450930, -0.0317382810, -0.0314788820,
		0.0310821530, 0.0305175780, 0.0297851560, 0.0288848880,
		0.0278015140, 0.0265350340, 0.0250854490, 0.0234222410,
		0.0215759280, 0.0195312500, 0.0172576900, 0.0148010250,
		0.0121154790, 0.0092315670, 0.0061340330, 0.0028228760,
		-0.0006866460, -0.0043945310, -0.0083160400, -0.0124206540,
		-0.0167083740, -0.0211791990, -0.0258178710, -0.0306091310,
		-0.0355529790, -0.0406341550, -0.0458374020, -0.0511322020,
		-0.0565338130, -0.0619964600, -0.0675201420, -0.0730590820,
		-0.0786285400, -0.0841827390, -0.0897064210, -0.0951690670,
		-0.1005401610, -0.1058197020, -0.1109466550, -0.1159210210,
		-0.1206970210, -0.1252593990, -0.1295623780, -0.1335906980,
		-0.1372985840, -0.1406707760, -0.1436767580, -0.1462554930,
		-0.1484222410, -0.1501159670, -0.1513061520, -0.1519622800,
		-0.1520690920, -0.1515960690, -0.1504974370, -0.1487731930,
		-0.1463623050, -0.1432647710, -0.1394500730, -0.1348876950,
		-0.1295776370, -0.1234741210, -0.1165771480, -0.1088562010,
		0.1003112790, 0.0909271240, 0.0806884770, 0.0695953370,
		0.0576171870, 0.0447845460, 0.0310821530, 0.0165100100,
		0.0010681150, -0.0152282710, -0.0323791500, -0.0503540040,
		-0.0691680910, -0.0887756350, -0.1091613770, -0.1303100590,
		-0.1522064210, -0.1747894290, -0.1980590820, -0.2219848630,
		-0.2465057370, -0.2715911870, -0.2972106930, -0.3233184810,
		-0.3498687740, -0.3768005370, -0.4040832520, -0.4316558840,
		-0.4594726560, -0.4874725340, -0.5156097410, -0.5438232420,
		-0.5720367430, -0.6002197270, -0.6282958980, -0.6562194820,
		-0.6839141850, -0.7113189700, -0.7383728030, -0.7650299070,
		-0.7912139890, -0.8168640140, -0.8419494630, -0.8663635250,
		-0.8900909420, -0.9130554200, -0.9351959230, -0.9564819340,
		-0.9768524170, -0.9962463380, -1.0146179200, -1.0319366460,
		-1.0481567380, -1.0632171630, -1.0771179200, -1.0897827150,
		-1.1012115480, -1.1113739010, -1.1202239990, -1.1277465820,
		-1.1339263920, -1.1387634280, -1.1422119140, -1.1442871090,
		1.1449890140, 1.1442871090, 1.1422119140, 1.1387634280,
		1.1339263920, 1.1277465820, 1.1202239990, 1.1113739010,
		1.1012115480, 1.0897827150, 1.0771179200, 1.0632171630,
		1.0481567380, 1.0319366460, 1.0146179200, 0.9962463380,
		0.9768524170, 0.9564819340, 0.9351959230, 0.9130554200,
		0.8900909420, 0.8663635250, 0.8419494630, 0.8168640140,
		0.7912139890, 0.7650299070, 0.7383728030, 0.7113189700,
		0.6839141850, 0.6562194820, 0.6282958980, 0.6002197270,
		0.5720367430, 0.5438232420, 0.5156097410, 0.4874725340,
		0.4594726560, 0.4316558840, 0.4040832520, 0.3768005370,
		0.3498687740, 0.3233184810, 0.2972106930, 0.2715911870,
		0.2465057370, 0.2219848630, 0.1980590820, 0.1747894290,
		0.1522064210, 0.1303100590, 0.1091613770, 0.0887756350,
		0.0691680910, 0.0503540040, 0.0323791500, 0.0152282710,
		-0.0010681150, -0.0165100100, -0.0310821530, -0.0447845460,
		-0.0576171870, -0.0695953370, -0.0806884770, -0.0909271240,
		0.1003112790, 0.1088562010, 0.1165771480, 0.1234741210,
		0.1295776370, 0.1348876950, 0.1394500730, 0.1432647710,
		0.1463623050, 0.1487731930, 0.1504974370, 0.1515960690,
		0.1520690920, 0.1519622800, 0.1513061520, 0.1501159670,
		0.1484222410, 0.1462554930, 0.1436767580, 0.1406707760,
		0.1372985840, 0.1335906980, 0.1295623780, 0.1252593990,
		0.1206970210, 0.1159210210, 0.1109466550, 0.1058197020,
		0.1005401610, 0.0951690670, 0.0897064210, 0.0841827390,
		0.0786285400, 0.0730590820, 0.0675201420, 0.0619964600,
		0.0565338130, 0.0511322020, 0.0458374020, 0.0406341550,
		0.0355529790, 0.0306091310, 0.0258178710, 0.0211791990,
		0.0167083740, 0.0124206540, 0.0083160400, 0.0043945310,
		0.0006866460, -0.0028228760, -0.0061340330, -0.0092315670,
		-0.0121154790, -0.0148010250, -0.0172576900, -0.0195312500,
		-0.0215759280, -0.0234222410, -0.0250854490, -0.0265350340,
		-0.0278015140, -0.0288848880, -0.0297851560, -0.0305175780,
		0.0310821530, 0.0314788820, 0.0317382810, 0.0318450930,
		0.0318145750, 0.0316619870, 0.0313873290, 0.0310058590,
		0.0305328370, 0.0299377440, 0.0292816160, 0.0285339360,
		0.0277252200, 0.0268402100, 0.0259094240, 0.0249328610,
		0.0239105220, 0.0228576660, 0.0217895510, 0.0206909180,
		0.0195770260, 0.0184631350, 0.0173492430, 0.0162353520,
		0.0151214600, 0.0140228270, 0.0129394530, 0.0118865970,
		0.0108489990, 0.0098419190, 0.0088653560, 0.0079193120,
		0.0070037840, 0.0061187740, 0.0052948000, 0.0044860840,
		0.0037231450, 0.0030059810, 0.0023345950, 0.0016937260,
		0.0010986330, 0.0005493160, 0.0000305180, -0.0004425050,
		-0.0008697510, -0.0012664790, -0.0016174320, -0.0019378660,
		-0.0022277830, -0.0024871830, -0.0027008060, -0.0028839110,
		-0.0030517580, -0.0031738280, -0.0032806400, -0.0033721920,
		-0.0034179690, -0.0034637450, -0.0034790040, -0.0034790040,
		-0.0034637450, -0.0034332280, -0.0033874510, -0.0033264160,
		0.0032501220, 0.0031738280, 0.0030822750, 0.0029907230,
		0.0028991700, 0.0027923580, 0.0026855470, 0.0025787350,
		0.0024566650, 0.0023498540, 0.0022430420, 0.0021209720,
		0.0020141600, 0.0019073490, 0.0017852780, 0.0016937260,
		0.0015869140, 0.0014801030, 0.0013885500, 0.0012969970,
		0.0012054440, 0.0011138920, 0.0010375980, 0.0009613040,
		0.0008850100, 0.0008087160, 0.0007476810, 0.0006866460,
		0.0006256100, 0.0005798340, 0.0005340580, 0.0004730220,
		0.0004425050, 0.0003967290, 0.0003662110, 0.0003204350,
		0.0002899170, 0.0002593990, 0.0002441410, 0.0002136230,
		0.0001983640, 0.0001678470, 0.0001525880, 0.0001373290,
		0.0001220700, 0.0001068120, 0.0001068120, 0.0000915530,
		0.0000762940, 0.0000762940, 0.0000610350, 0.0000610350,
		0.0000457760, 0.0000457760, 0.0000305180, 0.0000305180,
		0.0000305180, 0.0000305180, 0.0000152590, 0.0000152590,
		0.0000152590, 0.0000152590, 0.0000152590, 0.0000152590,
	};
	window = gb_window;
}

int SubBandSynthesis (double *bandPtr, int channel, short *samples)
{
	register int i,j,k;
	register double *bufOffsetPtr, sum;
	static int init = 1;
	typedef double NN[64][32];
	static NN *filter;
	typedef double BB[2][2*HAN_SIZE];
	static BB *buf;
	static int bufOffset[2] = {64,64};
	static double *window;
	int clip = 0;               /* count & return how many samples clipped */

	if (init) {
		buf = (BB *) mem_alloc(sizeof(BB),"BB");
		filter = (NN *) mem_alloc(sizeof(NN), "NN");
		create_syn_filter(*filter);
		window = (double *) mem_alloc(sizeof(double) * HAN_SIZE, "WIN");
		read_syn_window(window);
		init = 0;
	}
/*    if (channel == 0) */
	bufOffset[channel] = (bufOffset[channel] - 64) & 0x3ff;
	bufOffsetPtr = &((*buf)[channel][bufOffset[channel]]);

	for (i=0; i<64; i++) {
		sum = 0;
		for (k=0; k<32; k++)
			sum += bandPtr[k] * (*filter)[i][k];
		bufOffsetPtr[i] = sum;
	}
	/*  S(i,j) = D(j+32i) * U(j+32i+((i+1)>>1)*64)  */
	/*  samples(i,j) = MWindow(j+32i) * bufPtr(j+32i+((i+1)>>1)*64)  */
	for (j=0; j<32; j++) {
		sum = 0;
		for (i=0; i<16; i++) {
			k = j + (i<<5);
			sum += window[k] * (*buf) [channel] [( (k + ( ((i+1)>>1) <<6) ) +
												bufOffset[channel]) & 0x3ff];
		}
		{
			/*long foo = (sum > 0) ? sum * SCALE + 0.5 : sum * SCALE - 0.5; */
			long foo = sum * SCALE;
			if (foo >= (long) SCALE)      {samples[j] = SCALE-1; ++clip;}
			else if (foo < (long) -SCALE) {samples[j] = -SCALE;  ++clip;}
			else                           samples[j] = foo;
		}
	}
    return(clip);
}

void out_fifo(short pcm_sample[2][SSLIMIT][SBLIMIT], int num, struct frame_params *fr_ps, unsigned long *psampFrames)
{
	int i,j,l;
	int stereo = fr_ps->stereo;
	//int sblimit = fr_ps->sblimit;
	static long k = 0;

        for (i=0;i<num;i++) for (j=0;j<SBLIMIT;j++) {
            (*psampFrames)++;
            for (l=0;l<stereo;l++) {
                if (!(k%1600) && k) {
                    //fwrite(outsamp,2,1600,outFile);
                    k = 0;
                }
                //outsamp[k++] = pcm_sample[l][i][j];
            }
        }
}


void  buffer_CRC(Bit_stream_struc *bs, unsigned int *old_crc)
{
    *old_crc = getbits(bs, 16);
}


extern int bitrate[3][15];
extern double s_freq[4];
/* Return the number of slots for main data of current frame, */
int main_data_slots(struct frame_params fr_ps)
{
	int nSlots;

	nSlots = (144 * bitrate[2][fr_ps.header->bitrate_index])
			/ s_freq[fr_ps.header->sampling_frequency];
	if (fr_ps.header->padding) nSlots++;
	nSlots -= 4;
	if (fr_ps.header->error_protection)
		nSlots -= 2;
	if (fr_ps.stereo == 1)
		nSlots -= 17;
	else
		nSlots -=32;
	return(nSlots);
}
