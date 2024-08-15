/*
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "dynamic_hdr_vivid.h"
#include "get_bits.h"
//#include <string>
 //#include <iostream>
 //#include <sstream>

static const int32_t maxrgb_den = 4095;
static const int32_t color_saturation_gain_den = 128;
static const int32_t maximum_luminance_den = 4095;
static const int32_t base_param_m_p_den = 16383;
static const int32_t base_param_m_m_den = 10;
static const int32_t base_param_m_a_den = 1023;
static const int32_t base_param_m_b_den = 1023;
static const int32_t base_param_m_n_den = 10;
static const int32_t base_param_Delta_den = 127;

static int vivid_meda_dump(int framenum,  AVDynamicHDRVivid *pMeta, char *metafile)
{
    //ofstream out_specCombine;
    AVHDRVividColorTransformParams *pVidiParam = (AVHDRVividColorTransformParams *)&pMeta->params[0];
    //char buf[64][1024] = { 0 };
    FILE *fp = NULL;
    char stemp[256]={0};
    char sline[256]={0};
    int  param_cnt = 0;
    if (framenum == 1)
        fp = fopen(metafile, "w");
    else
        fp = fopen(metafile, "a+");

    unsigned int frame_num_int = (unsigned int)framenum;
    sprintf(stemp, "%d %d %d %d %d %d %d", frame_num_int,
            pMeta->system_start_code,
            pVidiParam->minimum_maxrgb.num,
            //framenum-1,
            pVidiParam->average_maxrgb.num,pVidiParam->variance_maxrgb.num,pVidiParam->maximum_maxrgb.num,pVidiParam->tone_mapping_mode_flag);
    // out_specCombine << frame_num_int << " " << pMeta->system_start_code;
    // out_specCombine << " " << pVidiParam->minimum_maxrgb.num;
    // out_specCombine << " " << pVidiParam->average_maxrgb.num;
    // out_specCombine << " " << pVidiParam->variance_maxrgb.num;
    // out_specCombine << " " << pVidiParam->maximum_maxrgb.num;

    // out_specCombine << " " << pVidiParam->tone_mapping_mode_flag;
    if (pVidiParam->tone_mapping_mode_flag)
    {
       // out_specCombine << " " << (pVidiParam->tone_mapping_param_num - 1);
        sprintf(stemp,"%s %d",stemp,pVidiParam->tone_mapping_param_num-1);
        for (unsigned int mode_i = 0; mode_i < pVidiParam->tone_mapping_param_num; mode_i++)
        {
            sprintf(stemp,"%s %d",stemp,pVidiParam->tm_params[mode_i].targeted_system_display_maximum_luminance);
            sprintf(stemp,"%s %d",stemp,pVidiParam->tm_params[mode_i].base_enable_flag);
            if (pVidiParam->tm_params[mode_i].base_enable_flag)
            {
                sprintf(stemp,"%s %d",stemp,pVidiParam->tm_params[mode_i].base_param_m_p.num);
                sprintf(stemp,"%s %d",stemp,pVidiParam->tm_params[mode_i].base_param_m_m.num);
                sprintf(stemp,"%s %d",stemp,pVidiParam->tm_params[mode_i].base_param_m_a.num);
                sprintf(stemp,"%s %d",stemp,pVidiParam->tm_params[mode_i].base_param_m_b.num);
                sprintf(stemp,"%s %d",stemp,pVidiParam->tm_params[mode_i].base_param_m_n.num);
                sprintf(stemp,"%s %d",stemp,pVidiParam->tm_params[mode_i].base_param_k1);
                sprintf(stemp,"%s %d",stemp,pVidiParam->tm_params[mode_i].base_param_k2);
                sprintf(stemp,"%s %d",stemp,pVidiParam->tm_params[mode_i].base_param_k3);
                sprintf(stemp,"%s %d",stemp,pVidiParam->tm_params[mode_i].base_param_Delta_enable_mode);
                sprintf(stemp,"%s %d",stemp,pVidiParam->tm_params[mode_i].base_param_Delta);
            }
            sprintf(stemp,"%s %d",stemp,pVidiParam->tm_params[mode_i].three_Spline_enable_flag);
            if (pVidiParam->tm_params[mode_i].three_Spline_enable_flag)
            {
                sprintf(stemp,"%s %d",stemp,pVidiParam->tm_params[mode_i].three_Spline_num- 1);

                for (unsigned int mode_j = 0; mode_j < pVidiParam->tm_params[mode_i].three_Spline_num; mode_j++)
                {
                    sprintf(stemp,"%s %d",stemp,pVidiParam->tm_params[mode_i].three_spline[mode_j].th_mode);

                    if ( pVidiParam->tm_params[mode_i].three_spline[mode_j].th_mode == 0|| pVidiParam->tm_params[mode_i].three_spline[mode_j].th_mode== 2)
                    {
                        sprintf(stemp,"%s %d",stemp,pVidiParam->tm_params[mode_i].three_spline[mode_j].th_enable_mb.num);
                    }
                    sprintf(stemp,"%s %d",stemp,pVidiParam->tm_params[mode_i].three_spline[mode_j].th_enable.num);
                    sprintf(stemp,"%s %d",stemp,pVidiParam->tm_params[mode_i].three_spline[mode_j].th_delta1.num);
                    sprintf(stemp,"%s %d",stemp,pVidiParam->tm_params[mode_i].three_spline[mode_j].th_delta2.num);
                    sprintf(stemp,"%s %d",stemp,pVidiParam->tm_params[mode_i].three_spline[mode_j].enable_strength.num);
                }
            }
        }
    }

    //out_specCombine << " " <<  pVidiParam->color_saturation_mapping_flag;// metadata.color_saturation_mapping_flag;
    sprintf(stemp,"%s %d",stemp,pVidiParam->color_saturation_mapping_flag);
    if (pVidiParam->color_saturation_mapping_flag)
    {
        sprintf(stemp,"%s %d",stemp,pVidiParam->color_saturation_num);
        for (unsigned int mode_i = 0; mode_i < pVidiParam->color_saturation_num; mode_i++)
        {
            sprintf(stemp,"%s %d",stemp,pVidiParam->color_saturation_gain[mode_i].num);
        }
    }
    sprintf(stemp,"%s\r\n",stemp);
    if(fp)
    {
        fprintf(fp,"%s",stemp);
        fclose(fp);
        fp = NULL;
    }
    return 0;
}

int ff_parse_itu_t_t35_to_dynamic_hdr_vivid(AVDynamicHDRVivid *s, const uint8_t *data,
                                             int size)
{
    GetBitContext gbc, *gb = &gbc;
    int ret;

    if (!s)
        return AVERROR(ENOMEM);

    ret = init_get_bits8(gb, data, size);
    if (ret < 0)
        return ret;

    if (get_bits_left(gb) < 8)
        return AVERROR_INVALIDDATA;

    s->system_start_code = get_bits(gb, 8);
    // T/UWA 005.1-2022, table 11
    if (s->system_start_code >= 0x01 && s->system_start_code <= 0x07) {
        s->num_windows = 1;

        if (get_bits_left(gb) < 12 * 4 * s->num_windows)
            return AVERROR_INVALIDDATA;
        for (int w = 0; w < s->num_windows; w++) {
            AVHDRVividColorTransformParams *params = &s->params[w];

            params->minimum_maxrgb  = (AVRational){get_bits(gb, 12), maxrgb_den};
            params->average_maxrgb  = (AVRational){get_bits(gb, 12), maxrgb_den};
            params->variance_maxrgb = (AVRational){get_bits(gb, 12), maxrgb_den};
            params->maximum_maxrgb  = (AVRational){get_bits(gb, 12), maxrgb_den};
            // printf("itu_t_t35_parse_to_dynamic_hdr_vivid: [%d,%d,%d,%d],den=%d\n",
            //        params->minimum_maxrgb.num,params->average_maxrgb.num,
            //        params->variance_maxrgb.num,params->maximum_maxrgb.num,params->variance_maxrgb.den);
        }

        if (get_bits_left(gb) < 2 * s->num_windows)
            return AVERROR_INVALIDDATA;
        for (int w = 0; w < s->num_windows; w++) {
            AVHDRVividColorTransformParams *params = &s->params[w];

            params->tone_mapping_mode_flag = get_bits(gb, 1);
            if (params->tone_mapping_mode_flag) {
                if (get_bits_left(gb) < 1 )
                    return AVERROR_INVALIDDATA;
                params->tone_mapping_param_num = get_bits(gb, 1) + 1;
                for (int i = 0; i < params->tone_mapping_param_num; i++) {
                    AVHDRVividColorToneMappingParams *tm_params = &params->tm_params[i];

                    if (get_bits_left(gb) < 13)
                        return AVERROR_INVALIDDATA;
                    tm_params->targeted_system_display_maximum_luminance = (AVRational){get_bits(gb, 12), maximum_luminance_den};
                    tm_params->base_enable_flag = get_bits(gb, 1);
                    if (tm_params->base_enable_flag) {
                        if (get_bits_left(gb) < (14 + 6 + 10 + 10 + 6 + 8 + 10))
                            return AVERROR_INVALIDDATA;
                        tm_params->base_param_m_p = (AVRational){get_bits(gb, 14), base_param_m_p_den};
                        tm_params->base_param_m_m = (AVRational){get_bits(gb,  6), base_param_m_m_den};
                        tm_params->base_param_m_a = (AVRational){get_bits(gb, 10), base_param_m_a_den};
                        tm_params->base_param_m_b = (AVRational){get_bits(gb, 10), base_param_m_b_den};
                        tm_params->base_param_m_n = (AVRational){get_bits(gb,  6), base_param_m_n_den};
                        tm_params->base_param_k1 = get_bits(gb, 2);
                        tm_params->base_param_k2 = get_bits(gb, 2);
                        tm_params->base_param_k3 = get_bits(gb, 4);
                        tm_params->base_param_Delta_enable_mode = get_bits(gb, 3);
                        tm_params->base_param_Delta = (AVRational){get_bits(gb, 7), base_param_Delta_den};
                    }
                    if (get_bits_left(gb) < 1)
                        return AVERROR_INVALIDDATA;
                    tm_params->three_Spline_enable_flag = get_bits(gb, 1);
                    if (tm_params->three_Spline_enable_flag) {
                        AVHDRVivid3SplineParams *three_spline;

                        if (get_bits_left(gb) < 1 + tm_params->three_Spline_num * (2 + 12 + 28 + 1))
                            return AVERROR_INVALIDDATA;
                        tm_params->three_Spline_num = get_bits(gb, 1) + 1;
                        if (tm_params->three_Spline_num > FF_ARRAY_ELEMS(tm_params->three_spline))
                            return AVERROR_INVALIDDATA;
                        for (int j = 0; j < tm_params->three_Spline_num; j++) {
                            three_spline = &tm_params->three_spline[j];
                            three_spline->th_mode = get_bits(gb, 2);
                            if (three_spline->th_mode == 0 || three_spline->th_mode == 2) {
                                if (get_bits_left(gb) < 8)
                                    return AVERROR_INVALIDDATA;
                                three_spline->th_enable_mb = (AVRational){get_bits(gb, 8),  255};
                            }
                            three_spline->th_enable = (AVRational){get_bits(gb, 12),  4095};
                            three_spline->th_delta1 = (AVRational){get_bits(gb, 10),  1023};
                            three_spline->th_delta2 = (AVRational){get_bits(gb, 10),  1023};
                            three_spline->enable_strength = (AVRational){get_bits(gb,  8),  255};
                        }
#if FF_API_HDR_VIVID_THREE_SPLINE
                        three_spline = &tm_params->three_spline[0];
FF_DISABLE_DEPRECATION_WARNINGS
                        tm_params->three_Spline_TH_mode = three_spline->th_mode;
                        tm_params->three_Spline_TH_enable_MB = three_spline->th_enable_mb;
                        tm_params->three_Spline_TH_enable = three_spline->th_enable;
                        tm_params->three_Spline_TH_Delta1 = three_spline->th_delta1;
                        tm_params->three_Spline_TH_Delta2 = three_spline->th_delta2;
                        tm_params->three_Spline_enable_Strength = three_spline->enable_strength;
FF_ENABLE_DEPRECATION_WARNINGS
#endif
                    }
                }
            }

            params->color_saturation_mapping_flag = get_bits(gb, 1);
            if (params->color_saturation_mapping_flag) {
                if (get_bits_left(gb) < 3 + params->color_saturation_num * 8)
                    return AVERROR_INVALIDDATA;

                params->color_saturation_num = get_bits(gb, 3);
                for (int i = 0; i < params->color_saturation_num; i++) {
                    params->color_saturation_gain[i] = (AVRational){get_bits(gb, 8), color_saturation_gain_den};
                }
            }
        }
    }
    // static int fram_num = 0;
    // fram_num++;
    // vivid_meda_dump(fram_num,s,"itu_t_t35_paserd_vivid.txt");
    return 0;
}